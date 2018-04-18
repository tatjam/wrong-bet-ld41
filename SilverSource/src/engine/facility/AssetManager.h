#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <unordered_map>
#include "../util/optional.h"
#include <iostream>

using std::experimental::optional; 

class AssetManager
{
private:
	std::unordered_map<std::string, sf::Texture> textures;
	std::unordered_map<std::string, sf::SoundBuffer> sounds;

	optional<sf::Texture*> loadTexture(std::string name);
	optional<sf::SoundBuffer*> loadSound(std::string name);

public:

	optional<sf::Texture*> getTexture(std::string name);
	optional<sf::SoundBuffer*> getSound(std::string name);

	AssetManager();
	~AssetManager();
};

