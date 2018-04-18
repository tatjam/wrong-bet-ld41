#include "NSprite.h"

void NSprite::render(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

NSprite::NSprite(std::string name) : Node(name)
{
	sprite = sf::Sprite();
}

NSprite::NSprite(std::string name, sf::Texture* tex) : NSprite(name, tex, 0) {}

NSprite::NSprite(std::string name, sf::Texture* tex, int z) : Node(name)
{
	sprite = sf::Sprite();
	sprite.setTexture(*tex, true);

	setZ(z);
}


NSprite::~NSprite()
{
}
