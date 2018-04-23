#pragma once
#include "../../engine/node/Node.h"
#include "../../engine/util/VectorUtil.h"
#include "NBullet.h"
#include "NMine.h"
#include "NFire.h"

enum TrackType
{
	FIRM,
	WALL_X,
	WALL_Y,
	PLACED_WALL_X,
	PLACED_WALL_Y,
	MUD,
};

class NTrack : public Node
{
private:


	
	void extractWaypoints();

public:

	sf::Image* track;
	
	std::vector<std::pair<sf::Vector2u, float>> forces;
	std::vector<std::weak_ptr<NBullet>> bullets;
	std::vector<std::pair<sf::Vector2u, float>> healers;
	std::vector<std::weak_ptr<NMine>> mines;
	std::vector<std::weak_ptr<NFire>> fires;

	virtual void update(GameManager* game) override;

	float realScale = 5.0f;

	sf::Vector2f toRealPos(sf::Vector2u pixel);
	sf::Vector2u toPixelPos(sf::Vector2f world);

	TrackType getType(sf::Vector2u pixel);

	std::pair<uint8_t, size_t> getNextWaypoint(uint8_t level);
	sf::Vector2f getSpawnPos(size_t i);

	std::vector<std::vector<sf::Vector2u>> waypoints;

	NTrack(std::string name, sf::Image* track);
	~NTrack();
};

