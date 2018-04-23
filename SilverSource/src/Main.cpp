#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#include "ldgame/facility/Team.h"
#include "ldgame/nodes/NKart.h"
#include "ldgame/scenes/SKart.h"
#include "engine/facility/AssetManager.h"
#include "engine/facility/GameManager.h"

#include "engine/scene/Scene.h"

#include <SFML/Graphics.hpp>

#include "ldgame/gui.h"
#include "ldgame/GodAttack.h"
#include "ldgame/GameRace.h"
#include "ldgame/GameMenu.h"

int main()
{
	srand(time(NULL));

	sf::RenderWindow window(sf::VideoMode(768, 512), "SFML works!");
	window.setFramerateLimit(80);

	GameManager game = GameManager(&window);
	sf::Font flipps = sf::Font(); flipps.loadFromFile("res/gui/pixely.ttf");


	GameMenu menu = GameMenu();
	GameRace race = GameRace();

	menu.init(&game);

	// Preload some stuff
	game.assets.getSound("sound/explosion0.wav");
	game.assets.getSound("sound/crash.wav");
	game.assets.getSound("sound/softcrash.wav");
	game.assets.getSound("sound/lightcrash.wav");
	game.assets.getSound("sound/rocket.wav");
	game.assets.getSound("sound/splat.wav");
	game.assets.getSound("sound/shoot.wav");
	game.assets.getSound("sound/metalimpact.wav");

	window.resetGLStates();


	sf::View guiView = window.getDefaultView();
	
	bool inRace = false;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) { window.close(); }
			if (event.type == sf::Event::Resized)
			{
				game.handleResize(sf::Vector2f(event.size.width, event.size.height));
				guiView = sf::View({ 0.0f, 0.0f }, { (float)event.size.width,(float)event.size.height });
				guiView = sf::View(sf::FloatRect(0.0f, 0.0f, event.size.width, event.size.height));
			}
			if (event.type == sf::Event::MouseWheelMoved)
			{
				if (inRace)
				{
					race.handleScrollwheel(event.mouseWheel.delta, &game);
				}
			}
		}



		game.update();
		window.setView(game.view);

		if (inRace)
		{
			if (race.update(&game, window))
			{
				race.finish();
				menu.init(&game, true);
				inRace = false;
			}
		}
		else
		{

			if (menu.update(&game, window))
			{
				menu.finish();
				race.init(&game, menu.selectedMap);
				inRace = true;
			}
		}
		
		window.clear(sf::Color(50, 50, 50));

		if (inRace)
		{
			race.draw(&game, window, &flipps, guiView);
		}
		else
		{
			menu.draw(&game, window, &flipps, guiView);
		}
		
		window.display();

	}

	return 0;

}