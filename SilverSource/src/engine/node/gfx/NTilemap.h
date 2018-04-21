#pragma once

#include <sstream>
#include <string>

#include "../Node.h"
#include "../../util/optional.h"
#include "../../util/StringUtil.h"

using std::experimental::optional;

struct Tile
{
	// Negative tIds = nothing drawn (4 "null" vertices)
	int tId;
	// Hmm maybe add more but for now this works
};

class NTilemap : public Node
{
private:
	sf::VertexArray verts;
	std::vector<Tile> tiles;

	sf::Vector2u mapSize;

	// How many tiles in the tileset?
	sf::Vector2u tileCount;
	// Size of a tile in pixels, deduced from tileCount
	sf::Vector2u tileSize;

	bool dirty = false;

	// Builds ALL stuff
	void build();

	void buildTile(size_t i);

public:

	sf::Texture* tex;

	// Load tilemap data from a tilemap file, the tilemap
	// file is a csv file consisting of the tId
	// Returns false if it could not load properly
	bool load(std::string tdata);

	optional<Tile> get(size_t i);
	optional<Tile> get(size_t x, size_t y);

	// Returns false if invalid coordinates
	bool set(size_t x, size_t y, Tile data);
	bool set(size_t i, Tile data);

	virtual void render(sf::RenderTarget& target, sf::RenderStates states) const override;

	NTilemap(std::string name, sf::Texture* tex, sf::Vector2u tileCount);
	~NTilemap();
};

