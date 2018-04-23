#pragma once
#include "../nodes/NKart.h"
#include "../scenes/SKart.h"

class Team
{
private:
	bool player;
public:
	int totalLaps;

	int selected;
	std::vector<std::weak_ptr<NKart>> karts;

	void spawn(GameManager* game, std::shared_ptr<NTrack> track, int difficulty);

	int getAlive();

	Team(bool player);
	~Team();
};

