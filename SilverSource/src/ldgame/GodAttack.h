#pragma once
#include "facility/Team.h"

struct GodAttackArty
{
	sf::Vector2f target;
	bool swooshPlayed;
};

struct GodAttackMolotov
{
	sf::Vector2f target;
	float initialTime;
};

class GodAttack
{

public:

	std::vector<std::pair<float, GodAttackArty>> explosions;
	std::vector<std::pair<float, GodAttackMolotov>> molotovs;

	void godDraw(GameManager* game, sf::RenderTarget& target, std::shared_ptr<NTrack>& track)
	{
		sf::Sprite crosshair;
		crosshair.setTexture(*game->assets.getTexture("particle/crosshair.png").value());
		crosshair.setOrigin((sf::Vector2f)crosshair.getTexture()->getSize() * 0.5f);
		crosshair.setColor(sf::Color(255, 128, 128, 255));
		// Crosshairs
		for (auto molotov : molotovs)
		{
			if (molotov.first > 0.0f)
			{
				float size = molotov.first / molotov.second.initialTime;
				crosshair.setScale((size + 0.1f) * 0.5f, (size + 0.1f) * 0.5f);
				crosshair.setPosition(molotov.second.target);
				target.draw(crosshair);
			}
		}
	}

	void godUpdate(GameManager* game, std::shared_ptr<NTrack>& track)
	{
		auto it = explosions.begin();
		while (it != explosions.end())
		{
			if (!it->second.swooshPlayed && it->first > 0.0f && it->first < 0.5f)
			{
				// SWOOSH
				auto s = game->addNode<NQuickSound>({}, "woosh",
					game->assets.getSound("sound/arty.wav").value());
				it->second.swooshPlayed = true;

			}
			if (it->first <= 0.0f)
			{
				explosionCallback(game, track, it->second.target);
				it = explosions.erase(it);
			}
			else
			{
				it->first -= game->dt;
				it++;
			}
		}

		auto it2 = molotovs.begin();
		while (it2 != molotovs.end())
		{
			if (it2->first <= 0.0f)
			{
				molotovCallback(game, track, it2->second.target);
				it2 = molotovs.erase(it2);
			}
			else
			{
				it2->first -= game->dt;
				it2++;
			}
		}
	}

	static float getCost(int num)
	{
		if (num == 0)
		{
			// Oil
			return 0.2f;
		}
		else if (num == 1)
		{
			//Mine
			return 0.35f;
		}
		else if (num == 2)
		{
			//HE Bomb
			return 0.5f;
		}
		else if (num == 3)
		{
			// Healer
			return 0.56f;
		}
		else if (num == 4)
		{
			// Gun
			return 0.4f;
		}
		else
		{
			// Molotov
			return 0.3f;
		}
	}

	void operator()(int num, GameManager* game, std::shared_ptr<NTrack>& track, sf::Vector2f mousePos, 
		float* mana, std::shared_ptr<NKart> selected)
	{
		float cost = getCost(num);

		if (num == 0)
		{
			if (*mana >= cost)
			{
				// Obstacle
				sf::Vector2u start = track->toPixelPos(mousePos);
				start -= sf::Vector2u(10, 10);
				sf::Vector2u end = start + sf::Vector2u(20, 20);
				for (size_t x = start.x; x < end.x; x++)
				{
					for (size_t y = start.y; y < end.y; y++)
					{
						track->track->setPixel(x, y, sf::Color(255, 0, 10));
					}
				}

				auto s = game->addNode<NQuickSound>({}, "launchSound", 
					game->assets.getSound("sound/splat.wav").value());

				auto spill = game->addNode<NSprite>({}, "oilspill",
					game->assets.getTexture("particle/oilspill.png").value());
				spill->setPosition(mousePos);
				spill->setRotation(RangeFloat(0.0f, 360.0f)());
				spill->setScale(track->realScale, track->realScale);
				spill->center();
				spill->setZ(-500);

				s->sound.setPitch(RangeFloat(0.8f, 1.2f)());

				*mana -= cost;
			}
			
		}
		else if (num == 1)
		{
			if (*mana >= cost)
			{
				// Mine
				auto mine = game->addNode<NMine>({}, "mine",
					game->assets.getTexture("particle/mine.png").value(),
					game->assets.getTexture("particle/mineready.png").value(),
					RangeFloat(1.0f, 3.0f)());
				mine->setPosition(mousePos);
				mine->setRotation(RangeFloat(0.0f, 360.0f)());
				mine->center();
				mine->setZ(-190);

				track->mines.push_back(mine);

				*mana -= cost;
			}
		}
		else if (num == 2)
		{
			if (*mana >= cost)
			{
				// HE Bomb
				explosions.push_back(std::make_pair(RangeFloat(1.2f, 4.5f)(), GodAttackArty{ mousePos, false }));
				game->addNode<NQuickSound>({}, "launchSound", game->assets.getSound("sound/rocket.wav").value());
				*mana -= cost;
			}
		}
		else if (num == 3)
		{
			if (*mana >= cost)
			{
				track->healers.push_back(std::make_pair(track->toPixelPos(mousePos), 3.0f));

				auto flash = game->addNode<NFlash>({}, "healcrosshair",
					game->assets.getTexture("particle/glow.png").value(), 3.0f);
				flash->center();
				flash->setPosition(mousePos);
				flash->setScale(2.0f, 2.0f);

				flash->sprite.setColor(sf::Color(70, 255, 70, 80));

				*mana -= cost;
			}
		}
		else if (num == 4)
		{
			if (*mana >= cost)
			{
				// Attach gun
				if (selected)
				{
					selected->gunAttached = true;
					*mana -= cost;
				}
			}
		}
		else
		{
			if (*mana >= cost)
			{
				// Molotov
				float time = RangeFloat(1.2f, 4.5f)();
				molotovs.push_back(std::make_pair(time, GodAttackMolotov{ mousePos, time }));
				*mana -= cost;
			}
		}
	}

	void molotovCallback(GameManager* game, std::shared_ptr<NTrack>& track, sf::Vector2f pos)
	{
		for (size_t i = 0; i < 32; i++)
		{
			sf::Vector2f spos = RangeVector2f(pos - sf::Vector2f(50.0f, 50.0f),
				pos + sf::Vector2f(50.0f, 50.0f))();
			std::string name = "fire";
			// In order to make insertion faster we eveat
			// name collisions!
			name.append(std::to_string(game->frame));
			name.append(std::to_string(i));

			auto n = game->addNode<NFire>({}, name, game->assets.getTexture("particle/fire.png").value(),
				RangeFloat(1.5f, 5.0f)(), RangeFloat(-60.0f, 60.0f)());
			n->setPosition(spos);
			n->center();
			n->setZ(9999999);

			track->fires.push_back(n);
		}
	}

	void explosionCallback(GameManager* game, std::shared_ptr<NTrack>& track, sf::Vector2f pos)
	{
		track->forces.push_back(std::make_pair(track->toPixelPos(pos), 10.0f));

		// Cool looking explosion!
		auto flash = game->addNode<NFlash>({}, "godflash",
			game->assets.getTexture("particle/flash.png").value(), 0.1f);
		flash->center();
		flash->setPosition(pos);
		flash->setScale(2.0f, 2.0f);

		flash->sprite.setColor(sf::Color(255, 128, 70, 255));

		auto p = game->addNode<NParticle>({}, "expl",
			100, game->assets.getTexture("particle/explode.png").value());
		p->setZ(1000);
		p->setPosition(pos);
		p->time = 0.4f;
		p->constAccel = sf::Vector2f(0.0f, 0.0f);
		p->initialSpeed = RangeVector2f({ -250.0f, -250.0f }, { 250.0f, 250.0f });
		p->drag = 500.0f;
		p->constAccel = sf::Vector2f(170.0f, 0.0f);
		p->spawnTime = RangeFloat(0.001f, 0.005f);
		p->endColor = RangeColor({ 20, 20, 20, 0 }, { 80, 80, 40, 0 });
		p->midColor = RangeColor({ 80, 80, 80, 128 }, { 100, 100, 80, 255 });
		p->colorChange = RangeFloat(0.1f, 0.2f);

		auto p2 = game->addNode<NParticle>({}, "explDebris",
			25, game->assets.getTexture("particle/debris.png").value());
		p2->setZ(1100);
		p2->setPosition(pos);
		p2->time = 0.6f;
		p2->initialSpeed = RangeVector2f({ -50.0f, -70.0f }, { 50.0f, 20.0f });
		p2->spawnTime = RangeFloat(0.001f, 0.005f);
		p2->constAccel = sf::Vector2f(0.0f, 0.0f);
		p2->drag = 15.0f;

		auto s = game->addNode<NQuickSound>({}, "explSound",
			game->assets.getSound("sound/explosion0.wav").value());

		// Permanent sprite
		auto crater = game->addNode<NSprite>({}, "crater",
			game->assets.getTexture("particle/crater.png").value());
		crater->setPosition(pos);
		crater->center();
		crater->setScale(2.0f, 2.0f);
		crater->setZ(-200);
	}
};