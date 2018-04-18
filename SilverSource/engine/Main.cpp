#include <iostream>
#include <string>

#include "node/gfx/NSprite.h"
#include "facility/AssetManager.h"
#include "facility/GameManager.h"

#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	AssetManager assets = AssetManager();
	GameManager game = GameManager();

	sf::Texture* tex = assets.getTexture("test.png").value();
	sf::Texture* tex2 = assets.getTexture("test2.png").value();
	sf::Texture* tex3 = assets.getTexture("test3.png").value();

	sf::Clock dtc;
	sf::Time dtt;
	float dt = 0.0f;

	auto O = game.addNode<Node>({}, "Root");

	int frame = 0;
	
	{
		auto A = game.addNode<NSprite>({}, "A", tex);
		auto B = game.addNode<NSprite>(A, "B", tex);
		auto C = game.addNode<NSprite>(A, "B", tex);
		auto D = game.addNode<NSprite>(A, "B", tex);

		A->setPosition(100.0f, 0.0f); A->sprite.setColor(sf::Color::Red);
		B->setPosition(0.0f, 50.0f); B->sprite.setColor(sf::Color::Blue);
		C->setPosition(10.0f, 50.0f); C->sprite.setColor(sf::Color::Green);
		D->setPosition(20.0f, 50.0f); D->sprite.setColor(sf::Color::White);


	}


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		game.update();

		window.clear(sf::Color(50, 50, 50));

		game.draw(window);

		window.display();

		dtt = dtc.restart();
		dt = dtt.asSeconds();

		if(frame % 10 == 0)
			std::cout << "FPS: " << 1.0f / dt << std::endl;

		frame++;
	}

	return 0;

}