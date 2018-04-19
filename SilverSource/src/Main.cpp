#include <iostream>
#include <string>

#include "engine/node/gfx/NParticle.h"
#include "engine/node/gfx/NSprite.h"
#include "engine/facility/AssetManager.h"
#include "engine/facility/GameManager.h"

#include "engine/scene/Scene.h"

#include <SFML/Graphics.hpp>

int main()
{
	srand(time(NULL));

	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	window.setFramerateLimit(80);

	GameManager game = GameManager(&window);

	sf::Texture* tex = game.assets.getTexture("test.png").value();
	sf::Texture* tex2 = game.assets.getTexture("test2.png").value();
	sf::Texture* tex3 = game.assets.getTexture("test3.png").value();
	sf::Texture* smokeTex = game.assets.getTexture("smoke.png").value();

	auto O = game.addNode<Node>({}, "Root");


	int frame = 0;
	
	{
		auto A = game.addNode<NSprite>({}, "A", tex);
		auto B = game.addNode<NSprite>(A, "B", tex);
		auto C = game.addNode<NSprite>(A, "B", tex);
		auto D = game.addNode<NParticle>(A, "B", 100, smokeTex);

		A->setPosition(100.0f, 0.0f); A->sprite.setColor(sf::Color::Red);
		B->setPosition(0.0f, 50.0f); B->sprite.setColor(sf::Color::Blue);
		C->setPosition(10.0f, 50.0f); C->sprite.setColor(sf::Color::Green);
		D->setPosition(10.0f, 100.0f);

	}

	window.resetGLStates();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
				window.close();
		}

		game.update();

		window.clear(sf::Color(50, 50, 50));

		game.draw(window);

		window.display();

	}

	return 0;

}