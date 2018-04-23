#pragma once
#include "facility\Team.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "GodAttack.h"

void drawTeamRoster(Team* playerTeam, Team* enemyTeam, GameManager* game, sf::RenderWindow& target, 
	sf::Font* font, bool right, float scale)
{
	sf::Texture* guiLeftPanelTex = game->assets.getTexture("gui/leftpanel.png").value();
	sf::Texture* portraitGood = game->assets.getTexture("gui/portrait/oko.png").value();
	sf::Texture* portraitDamaged = game->assets.getTexture("gui/portrait/medo.png").value();
	sf::Texture* portraitDead = game->assets.getTexture("gui/portrait/bado.png").value();

	sf::Sprite guiDrawer;
	guiDrawer.setScale(scale, scale);
	int mx = sf::Mouse::getPosition(target).x;
	int my = sf::Mouse::getPosition(target).y;

	sf::Text guiText; guiText.setFont(*font);

	for (size_t i = 0; i < playerTeam->karts.size(); i++)
	{

		if (right)
		{
			guiDrawer.setPosition(target.getSize().x - (portraitGood->getSize().x + 4.0f) * scale,
				std::roundf(i * (portraitGood->getSize().y + 7.0f) * scale +
					target.getSize().y * 0.5f - (guiLeftPanelTex->getSize().y * scale) * 0.33f) - 2.0f * scale);
		}
		else
		{
			guiDrawer.setPosition(4.0f * scale,
				std::roundf(i * (portraitGood->getSize().y + 7.0f) * scale +
					target.getSize().y * 0.5f - (guiLeftPanelTex->getSize().y * scale) * 0.33f) - 2.0f * scale);
		}

		std::weak_ptr<NKart> k = playerTeam->karts[i];
		auto kl = k.lock();
		if (kl && kl->isAlive())
		{
			if (kl->health >= 50.0f)
			{
				guiDrawer.setTexture(*portraitGood, true);
			}
			else
			{
				guiDrawer.setTexture(*portraitDamaged, true);
			}

			std::stringstream ss;
			ss << std::setw(2) << std::setfill('0') << kl->lap;
			std::string s = ss.str();

			if (right)
			{
				guiText.setPosition(guiDrawer.getPosition() + sf::Vector2f(-14 * scale, 1 * scale));
			}
			else
			{
				guiText.setPosition(guiDrawer.getPosition() + sf::Vector2f(25 * scale, 1 * scale));
			}

			guiText.setString(s);
			guiText.setOutlineColor({ 0, 0, 0 });
			guiText.setOutlineThickness(scale);
			guiText.setCharacterSize(5 * scale);

			target.draw(guiText);

		}
		else
		{
			// No signal
			guiDrawer.setTexture(*portraitDead, true);
		}

		if (i == 5)
		{
			guiDrawer.move(0.0f, -scale);
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (!right)
			{
				if (mx <= 41 * scale && mx >= 31 * scale)
				{
					if (my >= guiDrawer.getPosition().y + 12 * scale
						&& my <= guiDrawer.getPosition().y + 22 * scale)
					{
						playerTeam->selected = i;
						enemyTeam->selected = -1;
					}
				}
			}
			else
			{
				if (mx >= target.getSize().x - 41 * scale && mx <= target.getSize().x - 31 * scale)
				{
					if (my >= guiDrawer.getPosition().y + 12 * scale
						&& my <= guiDrawer.getPosition().y + 22 * scale)
					{
						playerTeam->selected = i;
						enemyTeam->selected = -1;
					}
				}
			}
		}

		target.draw(guiDrawer);

	}
}

void drawGUI(GameManager* game, sf::RenderWindow& target, Team* playerTeam, Team* enemyTeam, sf::Font* font,
	float mana, int* selected)
{
	sf::Texture* guiLeftPanelTex = game->assets.getTexture("gui/leftpanel.png").value();
	sf::Texture* guiTopPanelTex = game->assets.getTexture("gui/toppanel.png").value();
	sf::Texture* guiBottomPanelTex = game->assets.getTexture("gui/bottompanel.png").value();

	sf::Sprite guiDrawer;
	sf::Text guiText; guiText.setFont(*font);

	// GUI and other here
	int scale = (int)(target.getSize().y / 300.0f);
	guiDrawer.setScale(scale, scale);

	guiDrawer.setTexture(*guiLeftPanelTex);
	sf::Vector2f or = (sf::Vector2f)guiLeftPanelTex->getSize(); or .y /= 2.0f; or .x = 0.0f;
	guiDrawer.setOrigin(or );
	guiDrawer.setPosition(0.0f, std::roundf(target.getSize().y * 0.5f));

	target.draw(guiDrawer);

	guiDrawer.setOrigin(or );
	guiDrawer.setPosition(target.getSize().x, std::roundf(target.getSize().y * 0.5f));
	guiDrawer.setScale(-scale, scale);
	target.draw(guiDrawer);
	guiDrawer.setScale(scale, scale);

	guiDrawer.setTexture(*guiTopPanelTex, true);
	or = (sf::Vector2f)guiTopPanelTex->getSize(); or .y = 2.0f; or .x /= 2.0f;
	guiDrawer.setOrigin(or );
	guiDrawer.setPosition(std::roundf(target.getSize().x * 0.5f), 0.0f);
	target.draw(guiDrawer);

	drawTeamRoster(playerTeam, enemyTeam, game, target, font, false, scale);
	drawTeamRoster(enemyTeam, playerTeam, game, target, font, true, scale);

	// Bottom panel
	guiDrawer.setTexture(*guiBottomPanelTex, true);
	or = (sf::Vector2f)guiBottomPanelTex->getSize(); or .y = 0.0f; or .x /= 2.0f;
	guiDrawer.setOrigin(or );
	guiDrawer.setPosition(std::roundf(target.getSize().x * 0.5f),
		std::roundf(target.getSize().y - guiBottomPanelTex->getSize().y * scale));
	target.draw(guiDrawer);

	sf::RectangleShape guiRect;
	guiRect.setSize(sf::Vector2f(mana * 178.0f * scale, 3.0f * scale));
	guiRect.setFillColor(sf::Color(85, 150, 196));
	guiRect.setPosition(guiDrawer.getPosition());
	guiRect.move(-89.3f * scale, 47 * scale);
	target.draw(guiRect);

	guiRect.setFillColor(sf::Color(255, 205, 103));
	guiRect.setSize(sf::Vector2f(GodAttack::getCost(*selected) * 178.0f * scale, 1.0f * scale));
	target.draw(guiRect);

	int mx = sf::Mouse::getPosition(target).x;
	int my = sf::Mouse::getPosition(target).y;
	int base = target.getSize().x / 2.0f - (guiBottomPanelTex->getSize().x / 2.0f) * scale - (20.0f * scale);

	int step = 37 * scale;
	for (int i = 0; i < 6; i++)
	{
		base += step;

		if (*selected == i)
		{
			sf::RectangleShape shap = sf::RectangleShape(sf::Vector2f(32 * scale, 32 * scale));
			shap.setPosition(base + (5.0f * scale), target.getSize().y - (36.0f - 1.0f) * scale);
			shap.setFillColor(sf::Color(255, 255, 255, 80));
			target.draw(shap);

		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (mx > base && mx < base + step)
			{
				if (my > target.getSize().y - (guiBottomPanelTex->getSize().y + 64.0f) * scale)
				{
					*selected = i;
				}
			}
		}
	}

}

