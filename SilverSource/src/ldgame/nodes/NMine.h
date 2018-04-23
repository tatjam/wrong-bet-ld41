#pragma once
#include "../../engine/node/gfx/NParticle.h"
#include "../../engine/node/gfx/NSprite.h"
#include "../../engine/util/VectorUtil.h"
#include "NFlash.h"
#include "../../engine/node/snd/NQuickSound.h"

class NMine : public NSprite
{
private:
	float timer;
	bool ready;
	sf::Texture* proTex;
public:

	std::weak_ptr<Node> owner;

	bool isReady()
	{
		return ready && isAlive();
	}

	virtual void update(GameManager* game) override
	{
		timer -= game->dt;
		if (timer <= 0)
		{
			sprite.setTexture(*proTex);
			this->ready = true;
		}
	}

	void explode(GameManager* game)
	{
		auto flash = game->addNode<NFlash>(getParent()->shared_from_this(), "flash",
			game->assets.getTexture("particle/flash.png").value(), 0.1f);
		flash->center();
		flash->setPosition(getPosition());
		flash->setScale(2.0f, 2.0f);

		auto s = game->addNode<NQuickSound>(getParent()->shared_from_this(), "explSound",
			game->assets.getSound("sound/explosion0.wav").value());

		auto p = game->addNode<NParticle>(getParent()->shared_from_this(), "expl",
			100, game->assets.getTexture("particle/explode.png").value());
		p->setZ(1000);
		p->setPosition(getPosition());
		p->time = 0.4f;
		p->constAccel = sf::Vector2f(0.0f, 0.0f);
		p->initialSpeed = RangeVector2f({ -250.0f, -250.0f }, { 250.0f, 250.0f });
		p->drag = 500.0f;
		p->constAccel = sf::Vector2f(170.0f, 0.0f);
		p->spawnTime = RangeFloat(0.001f, 0.005f);
		p->endColor = RangeColor({ 20, 20, 20, 0 }, { 80, 80, 40, 0 });
		p->midColor = RangeColor({ 80, 80, 80, 128 }, { 100, 100, 80, 255 });
		p->colorChange = RangeFloat(0.1f, 0.2f);

		destroy();
	}

	NMine(std::string name, sf::Texture* preTex, sf::Texture* proTex, float proTime) : NSprite(name, preTex)
	{
		this->proTex = proTex;
		this->ready = false;
		this->timer = proTime;
	}
};