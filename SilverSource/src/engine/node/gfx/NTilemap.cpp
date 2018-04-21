#include "NTilemap.h"

void NTilemap::build()
{
	verts.clear();
	verts.setPrimitiveType(sf::Quads);
	verts.resize(mapSize.x * mapSize.y * 4);

	for (size_t i = 0; i < tiles.size(); i++)
	{
		buildTile(i);
	}
}

void NTilemap::buildTile(size_t i)
{
	Tile t = tiles[i];
	sf::Vertex* quad = &verts[i * 4];

	size_t x = i % mapSize.x;
	size_t y = i / mapSize.x;

	size_t tx = t.tId % tileCount.x;
	size_t ty = t.tId / tileCount.y;

	if (t.tId >= 0)
	{
		quad[0].position = sf::Vector2f((x + 0) * tileSize.x, (y + 0) * tileSize.y);
		quad[1].position = sf::Vector2f((x + 1) * tileSize.x, (y + 0) * tileSize.y);
		quad[2].position = sf::Vector2f((x + 1) * tileSize.x, (y + 1) * tileSize.y);
		quad[3].position = sf::Vector2f((x + 0) * tileSize.x, (y + 1) * tileSize.y);

		quad[0].texCoords = sf::Vector2f((tx + 0) * tileSize.x, (ty + 0) * tileSize.y);
		quad[1].texCoords = sf::Vector2f((tx + 1) * tileSize.x, (ty + 0) * tileSize.y);
		quad[2].texCoords = sf::Vector2f((tx + 1) * tileSize.x, (ty + 1) * tileSize.y);
		quad[3].texCoords = sf::Vector2f((tx + 0) * tileSize.x, (ty + 1) * tileSize.y);
	}
	else
	{
		quad[0].position = sf::Vector2f(); quad[1].position = sf::Vector2f();
		quad[2].position = sf::Vector2f(); quad[3].position = sf::Vector2f();
	}
}

void NTilemap::render(sf::RenderTarget& target, sf::RenderStates states) const
{

	states.texture = tex;
	
	target.draw(verts, states);

	states.texture = NULL; // ? Maybe not needed ?
}

bool NTilemap::load(std::string tdata)
{
	std::vector<std::string> lines = splitByDelimiter(tdata, '\n');
	std::vector<std::vector<std::string>> toksByLines;
	for (std::string line : lines)
	{
		std::vector<std::string> vec = splitByDelimiter(line, ',');
		toksByLines.push_back(vec);
	}

	mapSize.x = toksByLines[0].size();
	mapSize.y = toksByLines.size();

	// Load the data 
	try
	{
		for (std::vector<std::string> row : toksByLines)
		{
			for (std::string col : row)
			{
				Tile t; t.tId = std::stoi(col);
				tiles.push_back(t);
			}
		}
	}
	catch (std::invalid_argument e)
	{
		std::cout << "[Ntilemap::load -> ERROR] Invalid Argument error" << std::endl;
		return false;
	}

	build();

	return true;
}

optional<Tile> NTilemap::get(size_t i)
{
	return (i >= tiles.size()) ? 
		std::experimental::optional<Tile>() 
		: tiles[i];
}

optional<Tile> NTilemap::get(size_t x, size_t y)
{
	return get(y * mapSize.x + x);
}

bool NTilemap::set(size_t x, size_t y, Tile data)
{
	return set(y * mapSize.x + x, data);
}

bool NTilemap::set(size_t i, Tile data)
{
	if (i <= tiles.size())
	{
		tiles[i] = data;
		return true;
	}
	else
	{
		return false;
	}
}

NTilemap::NTilemap(std::string name, sf::Texture* tex, sf::Vector2u tileSize) : Node(name)
{
	this->tex = tex;
	this->verts = sf::VertexArray();
	this->mapSize = sf::Vector2u(0, 0);
	this->tileCount = tileSize;
	this->tileSize.x = tex->getSize().x / tileSize.x;
	this->tileSize.y = tex->getSize().y / tileSize.y;
}


NTilemap::~NTilemap()
{
}
