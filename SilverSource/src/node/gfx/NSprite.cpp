#include "NSprite.h"

void NSprite::render(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

NSprite::NSprite(std::string name) : NGraphic(name, 0)
{
	sprite = sf::Sprite();
}

NSprite::NSprite(std::string name, sf::Texture* tex) : NSprite(name, tex, 0) {}

NSprite::NSprite(std::string name, sf::Texture* tex, int z) : NGraphic(name, z)
{
	sprite = sf::Sprite();
	sprite.setTexture(*tex, true);
}



NSprite::~NSprite()
{
}
