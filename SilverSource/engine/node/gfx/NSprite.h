#pragma once
#include "../Node.h"

class NSprite : public Node
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

