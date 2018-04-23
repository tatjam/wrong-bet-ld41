#include "NTrack.h"

sf::Vector2f NTrack::toRealPos(sf::Vector2u pixel)
{
	sf::Vector2f asFloat = (sf::Vector2f)pixel;
	asFloat.x *= realScale;
	asFloat.y *= realScale;
	return asFloat;
}
sf::Vector2u NTrack::toPixelPos(sf::Vector2f world)
{
	return sf::Vector2u(world.x / realScale, world.y / realScale);
}

void NTrack::update(GameManager* game)
{
	auto it = forces.begin();
	while (it != forces.end())
	{
		if (it->second == 0.0f)
		{
			it = forces.erase(it);
		}
		else
		{
			it->second = moveTo(it->second, 0.0f, game->dt * 100.0f);
			it++;
		}
	}

	auto it2 = bullets.begin();
	while (it2 != bullets.end())
	{
		if (!it2->lock()){it2 = bullets.erase(it2);}
		else{it2++;}
	}

	auto it3 = healers.begin();
	while (it3 != healers.end())
	{
		if (it3->second == 0.0f)
		{
			it3 = healers.erase(it3);
		}
		else
		{
			it3->second = moveTo(it3->second, 0.0f, game->dt);
			it3++;
		}
	}

	auto it4 = mines.begin();
	while (it4 != mines.end())
	{
		if (!it4->lock()) { it4 = mines.erase(it4); }
		else { it4++; }
	}

	auto it5 = fires.begin();
	while (it5 != fires.end())
	{
		if (!it5->lock()) { it5 = fires.erase(it5); }
		else { it5++; }
	}

}


TrackType NTrack::getType(sf::Vector2u pixel)
{
	if (pixel.x < 0 || pixel.x >= track->getSize().x
		|| pixel.y < 0 || pixel.y >= track->getSize().y)
	{
		return FIRM;
	}
	else
	{
		sf::Color p = track->getPixel(pixel.x, pixel.y);
		if (p.r == 0 && p.g == 255 && p.b == 0)
		{
			return WALL_Y;
		}
		else if (p.r == 0 && p.g == 255 && p.b > 0 && p.b < 255)
		{
			return PLACED_WALL_Y;
		}
		else if (p.r == 255 && p.g == 0 && p.b == 0)
		{
			return WALL_X;
		}
		else if (p.r == 255 && p.g == 0 && p.b > 0 && p.b < 255)
		{
			return PLACED_WALL_X;
		}
		else if (p.r == 0 && p.g == 0 && p.b == 255)
		{
			return MUD;
		}
		else
		{
			return FIRM;
		}
	}
}



std::pair<uint8_t, size_t> NTrack::getNextWaypoint(uint8_t level)
{
	uint8_t oLevel = 0;
	if (level < waypoints.size() - 1)
	{
		oLevel = level + 1;
	}

	size_t r = rand() % waypoints[oLevel].size();
	
	return std::make_pair(oLevel, r);
}

void NTrack::extractWaypoints()
{
	waypoints.resize(256);

	for (size_t x = 0; x < track->getSize().x; x++)
	{
		for (size_t y = 0; y < track->getSize().y; y++)
		{
			sf::Color pix = track->getPixel(x, y);
			if (pix.r == pix.g && pix.r == pix.b)
			{
				waypoints[255 - pix.r].push_back(sf::Vector2u(x, y));
			}
		}
	}

	// Delete empty levels
	auto it = waypoints.begin();
	while (it != waypoints.end())
	{
		if (it->size() == 0)
		{
			it = waypoints.erase(it);
		}
		else
		{
			it++;
		}
	}

}

sf::Vector2f NTrack::getSpawnPos(size_t i)
{
	i %= waypoints[0].size();
	return toRealPos(waypoints[0][i]);
}

NTrack::NTrack(std::string name, sf::Image* track) : Node(name)
{
	this->track = track;
	extractWaypoints();
}


NTrack::~NTrack()
{
}
