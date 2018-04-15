#include <iostream>
#include <string>

#include "node/gfx/NSprite.h"
#include "facility/AssetManager.h"

#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	AssetManager assets = AssetManager();

	sf::Texture* tex = assets.getTexture("test.png").value();
	sf::Texture* tex2 = assets.getTexture("test.png").value();

	auto O = std::make_shared<Node>("origin");
	auto A = std::make_shared<NSprite>("A", tex, 0);
	auto B = std::make_shared<NSprite>("B", tex, 0);
	auto C = std::make_shared<NSprite>("C", tex2, 0);

	O->addChildren(A);
	O->addChildren(C);
	A->addChildren(B);

	A->move(30.0f, 0.0f);
	B->move(0.0f, 30.0f);
	C->move(60.0f, 60.0f);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color(50, 50, 50));

		window.draw(*O);

		window.display();
	}

	return 0;

}