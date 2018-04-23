#pragma once
#include "facility/Team.h"

class GameRace
{
public:

	std::shared_ptr<NTrack> track;
	std::shared_ptr<Node> root;

	Team player, enemy;

	// God stuff
	float mana = 0.0f;
	int selected = 1;

	GodAttack god = GodAttack();
	bool rheld = false;

	std::shared_ptr<NKart> selectedKart;


	sf::Music* music;

	int trackNum = 0;

	float fade = 0.0f;
	int status;

	int difficulty;

	void init(GameManager* game, int trackNum, int difficulty)
	{
		status = 0;
		fade = 0.0f;
		music = new sf::Music();
		music->openFromFile("res/sound/music/game.wav");
		music->play();
		music->setVolume(30.f);
		music->setLoop(true);

		player.selected = 0;
		enemy.selected = -1;

		std::string trackStr = "track";
		trackStr.append(std::to_string(trackNum));

		sf::Image* trackImg = game->assets.getImage(trackStr + ".png").value();
		sf::Texture* trackTex = game->assets.getTexture(trackStr + "c" + ".png").value();
		sf::Texture* trackTexO = game->assets.getTexture(trackStr + "o" + ".png").value();

		sf::Texture* guiLeftPanelTex = game->assets.getTexture("gui/leftpanel.png").value();

		root = game->addNode<Node>({}, "Root");
		root->setScale(1.0f, 1.0f);

		track = game->addNode<NTrack>({}, "track", trackImg);
		track->realScale = 3.0f;

		auto trackSprt = game->addNode<NSprite>(track, "tracksprt", trackTex);
		trackSprt->setScale(track->realScale, track->realScale);
		trackSprt->setZ(-10000);
		trackSprt->setOrigin(65 + 256, 65 + 256);
		auto trackSprtO = game->addNode<NSprite>(track, "trackover", trackTexO);
		trackSprtO->setScale(track->realScale, track->realScale);
		trackSprtO->setZ(10000);
		trackSprtO->sprite.setColor(sf::Color(255, 255, 255, 255));
		trackSprtO->setOrigin(65 + 256, 65 + 256);

		player.spawn(game, track, difficulty);
		enemy.spawn(game, track, difficulty);

		this->difficulty = difficulty;
		this->mana = 0.0f;

	}

	void handleScrollwheel(int delta, GameManager* game)
	{
		float zoomSpeed = 12.0f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)
			|| sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			zoomSpeed *= 2.0f;
		}

		if (delta > 0)
		{
			game->view.zoom(1.0f + zoomSpeed * game->dt);
		}
		else
		{
			game->view.zoom(1.0f - zoomSpeed * game->dt);
		}
	}

	bool update(GameManager* game, sf::RenderWindow& window)
	{
		if (status == 0)
		{
			int playerAlive = player.getAlive();
			int enemyAlive = enemy.getAlive();

			if (playerAlive <= 0)
			{
				// Trigger defeat
				status = 1;
				music->stop();
			}

			if (enemyAlive <= 0)
			{
				// Trigger victory
				status = 2;
				music->stop();
			}

			if (player.selected < 0)
			{
				if (enemy.selected > 0)
				{
					selectedKart = enemy.karts[enemy.selected].lock();
				}
			}
			else
			{
				selectedKart = player.karts[player.selected].lock();
			}


			if (selectedKart)
			{
				game->view.setCenter(selectedKart->getPosition());
			}

			float baseSpeed = 500.0f;
			float zoomSpeed = 1.0f;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)
				|| sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				baseSpeed *= 2.0f;
				zoomSpeed *= 2.0f;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				selectedKart = {};
				player.selected = -1; enemy.selected = -1;
				game->view.move(0.0f, -baseSpeed * game->dt);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				selectedKart = {};
				player.selected = -1; enemy.selected = -1;
				game->view.move(-baseSpeed * game->dt, 0.0f);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				selectedKart = {};
				player.selected = -1; enemy.selected = -1;
				game->view.move(0.0f, baseSpeed * game->dt);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				selectedKart = {};
				player.selected = -1; enemy.selected = -1;
				game->view.move(baseSpeed * game->dt, 0.0f);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
			{
				game->view.zoom(1.0f + zoomSpeed * game->dt);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				game->view.zoom(1.0f - zoomSpeed * game->dt);
			}

			/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
			{
				// Trigger defeat
				status = 1;
				music->stop();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
			{
				// Trigger victory
				status = 2;
				music->stop();
			}*/

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				return true;
			}

			god.godUpdate(game, track);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && selected >= 0)
			{
				if (!rheld)
				{
					god(selected, game, track, window.mapPixelToCoords(sf::Mouse::getPosition(window)), &mana, selectedKart);
					rheld = true;
				}
			}
			else
			{
				rheld = false;
			}

			if (mana < 1.0f)
			{
				// Every 20 seconds the mana bar refills
				float increase = 0.07f;
				if (difficulty == 0) { increase = 0.12f; }
				if (difficulty == 1) { increase = 0.07f; }
				if (difficulty == 2) { increase = 0.05f; }
				mana += game->dt * increase;
				if (mana > 1.0f) { mana = 1.0f; }
			}
		}
		else if (status == 1 || status == 2)
		{
			fade += game->dt * 0.12f;
			if (fade >= 1.0f)
			{
				// Back to menu
				return true;
			}


			if (status == 1)
			{
				if (music->getStatus() == sf::Music::Stopped)
				{
					music->openFromFile("res/sound/music/gameover.wav");
					music->play();
					music->setLoop(false);
				}
			}
			else if (status == 2)
			{
				if (music->getStatus() == sf::Music::Stopped)
				{
					music->openFromFile("res/sound/music/victory.wav");
					music->play();
					music->setLoop(false);
				}
			}
		}

		return false;
	}

	void draw(GameManager* game, sf::RenderWindow& window, sf::Font* font, sf::View guiView)
	{
		game->draw(window);
		if (status == 0)
		{
			god.godDraw(game, window, track);

			window.setView(guiView);

			drawGUI(game, window, &player, &enemy, font, mana, &selected);
		}
		else
		{
			window.setView(guiView);

			sf::RectangleShape shader = sf::RectangleShape((sf::Vector2f)window.getSize());
			shader.setFillColor(sf::Color(0, 0, 0, fade * 255));


			window.draw(shader);

			sf::Text texter = sf::Text();
			texter.setFont(*font);
			texter.setCharacterSize(42);
			texter.setString(status == 1 ? "Defeat" : "Victory");
			sf::Color baseColor = (status == 1) ? sf::Color(255, 200, 200) : sf::Color(255, 255, 255);
			texter.setPosition(window.getSize().x / ((fade + 0.5f) * 2.0f), window.getSize().y / 2.0f);
			float fade2step = 1.0f - std::abs(1.5 * fade * fade - 0.5f);
			texter.setFillColor(sf::Color(baseColor.r, baseColor.g, baseColor.b, std::min(fade2step * 255, 255.0f)));
			window.draw(texter);

		}

	}

	void finish()
	{
		player.karts.clear();
		enemy.karts.clear();

		track = {};
		root->destroy();
		music->stop();
	}

	GameRace() : player(true), enemy(false)
	{
		delete music;
	}
};