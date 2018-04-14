#include <iostream>
#include <string>

#include "node/gfx/NSprite.h"
#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");

	auto O = std::make_shared<Node>("O");
	auto A = std::make_shared<NSprite>("A");
	auto B = std::make_shared<NSprite>("B");
	auto C = std::make_shared<NSprite>("C");

	O->addChildren(A);
	A->addChildren(B);
	O->addChildren(C);

	A->move(25.0f, 0.0f);
	A->setOrigin(5.0f, 5.0f);

	B->move(0.0f, 25.0f);
	C->move(50.0f, 0.0f);

	O->move(25.0f, 25.0f);


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
		O->rotate(0.01f);
		A->rotate(0.05f);

		window.display();
	}

	return 0;

}