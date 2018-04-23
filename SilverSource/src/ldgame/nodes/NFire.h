#pragma once
#include "../../engine/node/gfx/NSprite.h"
#include "../../engine/util/VectorUtil.h"


class NFire : public NSprite
{
private:
	float timer;
	float rotSpeed;
public:

	std::weak_ptr<Node> owner;

	virtual void render(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		sf::RenderStates bStates = states;
		bStates.blendMode = sf::BlendAdd;
		NSprite::render(target, bStates);

		bStates.blendMode = sf::BlendAlpha;
		NSprite::render(target, bStates);
	}

	virtual void update(GameManager* game) override
	{
		timer -= game->dt;
		if (timer <= 0.0f)
		{
			destroy();
		}

		rotate(game->dt * rotSpeed);
	}

	NFire(std::string name, sf::Texture* tex, float time, float rotSpeed) : NSprite(name, tex)
	{
		this->timer = time;
		this->rotSpeed = rotSpeed;
	}
};