#include <windows.h>
#include "H2MOD_HeadHunter.h"
#include "H2MOD.h"
#include "xliveless.h"
#include "h2mod.pb.h"

extern bool isHost;

void HeadHunter::spawn(unsigned int object_datum, int count, float x, float y, float z, float randomMultiplier) {

	for (int i = 0; i < count; i++) {
		try {
			char* nObject = new char[0xC4];
			DWORD dwBack;
			VirtualProtect(nObject, 0xC4, PAGE_EXECUTE_READWRITE, &dwBack);

			if (object_datum) {
				unsigned int player_datum = h2mod->get_unit_datum_from_player_index(0);
				call_object_placement_data_new(nObject, object_datum, player_datum, 0);
				*(float*)(nObject + 0x1C) = h2mod->get_player_x(0, true) * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				*(float*)(nObject + 0x20) = h2mod->get_player_y(0, true) * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				*(float*)(nObject + 0x24) = (h2mod->get_player_z(0, true) + 5.0f) * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				TRACE_GAME("object_datum = %08X, x=%d, y=%d, z=%d", object_datum, *(float*)(nObject + 0x1C), *(float*)(nObject + 0x20), *(float*)(nObject + 0x24));
				unsigned int object_gamestate_datum = call_object_new(nObject);
				call_add_object_to_sync(object_gamestate_datum);
			}
			delete[] nObject;
		}
		catch (...) {
			TRACE_GAME_N("Error running spawn command");
		}
	}
}

void HeadHunter::Initialize() {
	TRACE_GAME("[H2Mod-HeadHunter] : Initialize()");

	if (this->HeadHunterPlayers.size() > 0) {
		for (auto it = this->HeadHunterPlayers.begin(); it != this->HeadHunterPlayers.end(); ++it)
		{
			delete[] it->first;
		}
		this->HeadHunterPlayers.clear();
	}

	if (h2mod->NetworkPlayers.size() > 0)
	{
		for (auto it = h2mod->NetworkPlayers.begin(); it != h2mod->NetworkPlayers.end(); ++it)
		{
			HeadHunterPlayer* nPlayer = new HeadHunterPlayer;
			wcscpy(&nPlayer->PlayerName[0], it->first->PlayerName);
			this->HeadHunterPlayers[nPlayer] = true;
		}
	}

	HeadHunterPlayer* host = new HeadHunterPlayer;
	wcscpy(&host->PlayerName[0], h2mod->get_local_player_name());
	this->HeadHunterPlayers[host] = true;
}

void HeadHunter::PlayerDied(int unit_datum_index) {
	wchar_t* playername = h2mod->get_player_name_from_index(unit_datum_index);

	int unit_object = call_get_object(unit_datum_index, 3);
	if (unit_object) {
		float x = h2mod->get_player_x(h2mod->get_player_index_from_unit_datum(unit_datum_index), 0);
		float y = h2mod->get_player_y(h2mod->get_player_index_from_unit_datum(unit_datum_index), 0);
		float z = h2mod->get_player_z(h2mod->get_player_index_from_unit_datum(unit_datum_index), 0);

		int level = *(DWORD*)(0x30005960 + h2mod->get_player_index_from_unit_datum(unit_datum_index) * 0x20);

		this->spawn(Weapon::ball, level + 1, x, y, z, 1.0f);
	}
}
void HeadHunter::SpawnPlayer(int PlayerIndex)
{
	wchar_t* pName = h2mod->get_player_name_from_index(PlayerIndex);

	*(DWORD*)(0x30005960 + PlayerIndex * 0x20) = 0;

	TRACE_GAME("[H2Mod-HeadHunter]: SpawnPlayer(%i) : %ws ", PlayerIndex, pName);

}
