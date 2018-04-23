#pragma once

#include "../../engine/node/gfx/NSprite.h"

class NFlash : public NSprite
{
private:
	float timer;

public:

	virtual void render(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		sf::RenderStates bStates = states;
		bStates.blendMode = sf::BlendAdd;
		NSprite::render(target, bStates);
	}

	virtual void update(GameManager* game) override
	{
		this->timer -= game->dt;
		if (this->timer <= 0.0f)
		{
			destroy();
		}
	}


	NFlash(std::string name, sf::Texture* tex, float maxLife = 0.1f) : NSprite(name, tex)
	{
		this->timer = maxLife;
		setZ(900000);
	}
};
