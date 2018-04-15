#include "AssetManager.h"

optional<sf::Texture*>  AssetManager::loadTexture(std::string name)
{
	sf::Texture out;
	if (!out.loadFromFile("res/" + name))
	{
		return {};
	}
	else
	{
		textures[name] = out;
		return &textures[name];
	}
}

optional<sf::Texture*> AssetManager::getTexture(std::string name)
{
	auto it = textures.find(name);
	if (it != textures.end())
	{
		return std::experimental::make_optional(&it->second);
	}
	else
	{
		return loadTexture(name);
	}
}

AssetManager::AssetManager()
{
}


AssetManager::~AssetManager()
{
}
