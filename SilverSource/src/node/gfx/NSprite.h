#pragma once
#include "NGraphic.h"

class NSprite : public NGraphic
{
private:

public:

	sf::Sprite sprite;

	virtual void render(sf::RenderTarget& target, sf::RenderStates states) const;


	NSprite(std::string name);
	NSprite(std::string name, sf::Texture* tex);
	NSprite(std::string name, sf::Texture* tex, int z);
	~NSprite();
};

