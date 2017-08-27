#pragma once
#ifndef H2HEADHUNTER_H
#define H2HEADHUNTER_H
#include "Hook.h"
#include <unordered_map>

class HeadHunterPlayer
{
public:
	wchar_t PlayerName[16];
	int level = 0;
};

class HeadHunter
{
public:
	void Initialize();
	void Deinitialize();
	bool PickUpHandler(int PlayerIndex, unsigned int ObjectDatum);
	void SpawnPlayer(int PlayerIndex);
	void PlayerDied(int unit_datum_index); // We need to start using PlayerIndex here for sanity.
	void spawn(unsigned int object_datum, int count, float x, float y, float z, float randomMultiplier);
	std::unordered_map<int, int> player_level;
	std::unordered_map<HeadHunterPlayer*, bool> HeadHunterPlayers;

};
#endif