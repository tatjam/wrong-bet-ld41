#pragma once
#include "../Node.h"

class NSprite : public Node
{
private:

	sf::RectangleShape sprite;

public:


	virtual void render(sf::RenderTarget& target, sf::RenderStates states) const;

	NSprite(std::string name);
	~NSprite();
};

