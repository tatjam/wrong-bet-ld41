#include "AssetManager.h"

optional<sf::SoundBuffer*> AssetManager::getSound(std::string name)
{
	auto it = sounds.find(name);
	if (it != sounds.end())
	{
		return std::experimental::make_optional(&it->second);
	}
	else
	{
		return loadSound(name);
	}
}

optional<sf::SoundBuffer*> AssetManager::loadSound(std::string name)
{
	std::cout << "[INFO: AssetManager -> loadSound] Loading: " << name << "...";

	sf::SoundBuffer out;
	if (!out.loadFromFile("res/" + name))
	{
		return {};
	}
	else
	{
		std::cout << "OK" << std::endl;;
		sounds[name] = out;
		return &sounds[name];
	}
}

optional<sf::Texture*>  AssetManager::loadTexture(std::string name)
{
	std::cout << "[INFO: AssetManager -> loadTexture] Loading: " << name << "...";

	sf::Texture out;
	if (!out.loadFromFile("res/" + name))
	{
		return {};
	}
	else
	{
		std::cout << "OK" << std::endl;;
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

optional<sf::Image*>  AssetManager::loadImage(std::string name)
{
	std::cout << "[INFO: AssetManager -> loadImage] Loading: " << name << "...";

	sf::Image out;
	if (!out.loadFromFile("res/" + name))
	{
		return {};
	}
	else
	{
		std::cout << "OK" << std::endl;;
		images[name] = out;
		return &images[name];
	}
}

optional<sf::Image*> AssetManager::getImage(std::string name)
{
	auto it = images.find(name);
	if (it != images.end())
	{
		return std::experimental::make_optional(&it->second);
	}
	else
	{
		return loadImage(name);
	}
}


AssetManager::AssetManager()
{
}


AssetManager::~AssetManager()
{
}
