#include "Team.h"

void Team::spawn(GameManager* game, std::shared_ptr<NTrack> track)
{
	for (size_t i = 0; i < 6; i++)
	{
		std::string name = (player) ? "plkart_" : "aikart_";
		name.append(std::to_string(i + 1));
		auto nkart = std::static_pointer_cast<NKart>(game->addScene<SKart>(name, track, player));
		if (player)
		{
			nkart->setPosition(track->getSpawnPos(i + 6));
		}
		else
		{
			nkart->setPosition(track->getSpawnPos(i + 6));
		}

		karts.push_back(nkart);
	}
}

int Team::getAlive()
{
	int out = 0;
	for (auto k : karts)
	{
		auto kl = k.lock();
		if (kl)
		{
			if (kl->isAlive())
			{
				out++;
			}
		}
	}

	return out;
}

Team::Team(bool player)
{
	this->player = player;
	this->selected = 0;
}


Team::~Team()
{
}
