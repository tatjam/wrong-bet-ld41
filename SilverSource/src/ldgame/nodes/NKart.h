#pragma once
#include "../../engine/node/gfx/NSprite.h"

class NKart : public NSprite
{
public:
	NKart(std::string name, sf::Texture* tex);
	~NKart();
};

