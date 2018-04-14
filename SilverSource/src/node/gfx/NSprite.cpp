#include "NSprite.h"

void NSprite::render(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

NSprite::NSprite(std::string name)
	: Node(name)
{
	sprite = sf::RectangleShape(sf::Vector2f(10.0f, 10.0f));
	sprite.setFillColor(sf::Color(name[0] * 50, name[0] * 50, name[0] * 50));
}


NSprite::~NSprite()
{
}
