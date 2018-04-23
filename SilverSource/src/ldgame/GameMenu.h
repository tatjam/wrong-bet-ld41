#pragma once

class GameMenu
{
public:

	sf::Music* music;
	float fadeIn;
	float fadeIn2;

	int selectedMap = 0;

	void init(GameManager* game, bool reset = false)
	{
		music = new sf::Music();

		music->openFromFile("res/sound/music/menu.wav");
		music->play();
		music->setVolume(65.0f);
		// We loop manually
		music->setLoop(false);

		fadeIn = 1.0f;
		fadeIn2 = 1.0f;

		if (reset)
		{
			fadeIn = 0.0f;
			fadeIn2 = 0.0f;
		}

		selectedMap = 0;

	}

	bool update(GameManager* game, sf::RenderWindow& window)
	{
		fadeIn = moveTo(fadeIn, 0.0f, game->dt * 0.25f);
		fadeIn2 = moveTo(fadeIn2, 0.0f, game->dt * 0.20f);

		if (music->getStatus() == sf::Music::Stopped)
		{
			music->play();
			music->setPlayingOffset(sf::seconds(10.0f));
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			float yscale = window.getSize().y / 450.0f;
			float xscale = window.getSize().x / 800.0f;

			auto m = sf::Mouse::getPosition(window);
			int mx = m.x; int my = m.y;

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				// Map selectors
				float xPos = window.getSize().x - ((1.0f - fadeIn2) * window.getSize().x)
					+ window.getSize().x * 0.45f;

				float pxPos = xPos + (42 * (xscale + 1) * 2.36f);
				float pyPos = window.getSize().y * 0.7f;


				if (mx >= pxPos && mx <= pxPos + (80 * xscale))
				{
					if (my >= pyPos  && my <= pyPos + (40 * xscale))
					{
						return true;
					}
				}

				for (int i = 0; i < 3; i++)
				{
					xPos += (42 + 10) * (xscale + 1.0f);
					float yPos = window.getSize().y * 0.50f;

					if(mx >= xPos && mx <= xPos + (42 * (xscale + 1)))
					{
						if (my >= yPos && my <= yPos + (42 * (xscale + 1)))
						{
							selectedMap = i;
						}
					}

				}

				


				fadeIn = 0.0f;
				fadeIn2 = 0.0f;
			}
		}

		return false;
	}

	void draw(GameManager* game, sf::RenderWindow& window, sf::Font* font, sf::View guiView)
	{
		game->draw(window);

		window.setView(guiView);
		
		float yscale = window.getSize().y / 450.0f;
		float xscale = window.getSize().x / 800.0f;

		sf::Sprite drawer;
		drawer.setScale(xscale, yscale);
		drawer.setTexture(*game->assets.getTexture("menu/back.png").value(), true);
		window.draw(drawer);

		drawer.setTexture(*game->assets.getTexture("menu/god.png").value(), true);
		drawer.setScale(window.getSize().y / 450.0f, window.getSize().y / 450.0f);
		drawer.setPosition((-fadeIn * window.getSize().x * 0.90f) - window.getSize().x * 0.10f, 0.0f);
		window.draw(drawer);

		drawer.setTexture(*game->assets.getTexture("menu/title.png").value(), true);
		drawer.setScale(window.getSize().y / 450.0f, window.getSize().y / 450.0f);
		drawer.setPosition(window.getSize().x - ((1.0f - fadeIn2) * window.getSize().x), 0.0f);
		window.draw(drawer);

		sf::RectangleShape rect(sf::Vector2f(window.getSize().x, window.getSize().y));
		rect.setFillColor(sf::Color(0, 0, 0, fadeIn * 255));

		sf::Text text = sf::Text();
		text.setCharacterSize(10 * xscale);
		text.setFont(*font);
		text.setString("Made by Tatjam for LD-41. Drums by Tajampi");
		text.setOutlineColor(sf::Color(0, 0, 0, 255));
		text.setOutlineThickness(1.0f);
		text.setPosition(window.getSize().x * 0.25f, window.getSize().y * 0.9f);
		window.draw(text);


		float xPos = window.getSize().x - ((1.0f - fadeIn2) * window.getSize().x)
			+ window.getSize().x * 0.45f;

		drawer.setTexture(*game->assets.getTexture("menu/play.png").value(), true);
		drawer.setScale(window.getSize().y / 450.0f, window.getSize().y / 450.0f);
		drawer.setPosition(xPos + (42 * (xscale + 1) * 2.36f), window.getSize().y * 0.7f);
		window.draw(drawer);


		for (int i = 0; i < 3; i++)
		{

			xPos += (42 + 10) * (xscale + 1.0f);

			sf::Texture* tex = (i == selectedMap)
				? game->assets.getTexture("gui/mapchoose/on.png").value()
				: game->assets.getTexture("gui/mapchoose/off.png").value();

			sf::Texture* mtex = NULL;
			if (i == 0) { mtex = game->assets.getTexture("gui/mapchoose/night.png").value(); }
			if (i == 1) { mtex = game->assets.getTexture("gui/mapchoose/forest.png").value(); }
			if (i == 2) { mtex = game->assets.getTexture("gui/mapchoose/night.png").value(); }

			drawer.setTexture(*tex, true);
			drawer.setPosition(xPos, window.getSize().y * 0.50f);
			drawer.setScale(xscale + 1.0f, xscale + 1.0f);

			window.draw(drawer);

			drawer.setTexture(*mtex, true);
			drawer.move(5 * (xscale + 1), 5 * (xscale + 1));

			window.draw(drawer);

		}


		window.draw(rect);
	}

	void finish()
	{
		music->stop();
		music->setPlayingOffset(sf::seconds(0.0f));

	}
		
	~GameMenu()
	{
		delete music;
	}

	GameMenu()
	{

	}
};