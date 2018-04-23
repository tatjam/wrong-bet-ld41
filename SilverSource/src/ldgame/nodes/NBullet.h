#pragma once
#include "../../engine/node/gfx/NSprite.h"
#include "../../engine/util/VectorUtil.h"


class NBullet : public NSprite
{
private:
	sf::Vector2f dir;
	float speed;
public:

	std::weak_ptr<Node> owner;

	virtual void update(GameManager* game) override
	{
		move(dir * speed * game->dt);
		speed = moveTo(speed, 0.0f, game->dt * 600.0f);
		sf::Color col = sprite.getColor();
		col.r = moveTo(col.r, 0.0f, game->dt * 200.0f);
		col.g = moveTo(col.g, 0.0f, game->dt * 200.0f);
		col.b = moveTo(col.b, 0.0f, game->dt * 200.0f);
		sprite.setColor(col);

		if (speed <= 1.0f)
		{
			destroy();
		}
	}

	virtual void render(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		sf::RenderStates bStates = states;
		bStates.blendMode = sf::BlendAdd;
		NSprite::render(target, bStates);
	}

	NBullet(std::string name, sf::Texture* tex, sf::Vector2f dir, float speed) : NSprite(name, tex)
	{
		this->dir = dir;
		this->speed = speed;
		setRotation(getAngleVec(dir));
	}
};