#pragma once
#include "../nodes/NKart.h"

class SKart
{
public:
	SKart(GameManager* game, std::string name, std::shared_ptr<Node> parent, std::shared_ptr<Node>& outRoot, bool blue,
		int difficulty)
	{
		sf::Texture* kart = ((blue) ? game->assets.getTexture("kart/blue.png").value()
			: game->assets.getTexture("kart/red.png").value());


		auto root = game->addNode<NKart>(parent, name, kart, blue, difficulty);
		auto wheels = game->addNode<NSprite>(root, "wheels", game->assets.getTexture("kartwheels.png").value());
		auto smoke = game->addNode<NParticle>(root, "smoke", 100, game->assets.getTexture("particle/smoke.png").value());
		root->center();
		root->motor = 800.0f;
		//root->setScale(2.0f, 2.0f);

		smoke->setZ(100);
		smoke->constAccel = sf::Vector2f(0.0f, -2.0f);
		smoke->initialSpeed = RangeVector2f({ -25.0f, -25.0f }, { 25.0f, 25.0f });
		smoke->maxLife = RangeFloat(0.6f, 1.2f);
		smoke->spawnTime = RangeFloat(0.05f, 0.08f);
		smoke->midColor = RangeColor(sf::Color(50, 50, 50, 60), sf::Color(50, 50, 50, 100));
		smoke->endColor = RangeColor(sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0));

		wheels->center();
		wheels->setPosition(32.0f, 16.0f);
		wheels->setZ(-10);

		outRoot = root;
	}

};

