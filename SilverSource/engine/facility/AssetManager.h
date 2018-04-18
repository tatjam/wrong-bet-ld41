#pragma once
#include "SFML/Graphics.hpp"

#include <unordered_map>
#include "../util/optional.h"

using std::experimental::optional; 

class AssetManager
{
private:
	std::unordered_map<std::string, sf::Texture> textures;

	optional<sf::Texture*> loadTexture(std::string name);

public:

	optional<sf::Texture*> getTexture(std::string name);


	AssetManager();
	~AssetManager();
};

