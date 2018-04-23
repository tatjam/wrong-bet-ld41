#pragma once
#include <vector>
#include <algorithm>
#include <iostream>

#include "SFML/Audio.hpp"
#include "../node/Node.h"
#include "AssetManager.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui-SFML.h"



// NOTE: You cannot remove a node here, remove it 
// by calling node->destroy. Keep in mind the node won't
// deallocate until all references to it are dead. This can 
// easily cause memory leaking.


class Node;

class GameManager
{
private:

	std::weak_ptr<Node> root;

	std::vector<std::shared_ptr<Node>> nodes;

	// Used for Z-Ordering, rebuilt only
	// when something changes (set dirty)
	// Is kept ordered by the code
	std::vector<std::weak_ptr<Node>> renderList;

	bool dirty = true;

	void rebuildRenderList();

	void lowAddNode(std::shared_ptr<Node> parent, std::shared_ptr<Node> node);

	sf::Clock dtc;

	std::vector<sf::Sound> activeSounds;

	sf::RenderWindow* target;

	bool uiToolbar;
	bool uiToolbarHeld = false;

	void doUiToolbar();

	bool firstUpdate;


public:

	AssetManager assets;

	uint64_t frame;
	float dt;
	float time;

	std::weak_ptr<Node> getRoot();

	sf::View view;

	void setDirty();

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

	void handleResize(sf::Vector2f newSize);

	void update();

	template <class T, class ...Args>
	std::shared_ptr<T> addNode(std::shared_ptr<Node> parent, Args && ...args)
	{
		auto node = std::make_shared<T>(std::forward<Args>(args)...);
		lowAddNode(parent, node);
		return node;
	}

	template <class T, class ...Args>
	std::shared_ptr<Node> addScene(std::string name, std::shared_ptr<Node> parent, Args && ...args)
	{
		std::shared_ptr<Node> out;
		std::make_shared<T>(this, name, parent, out, std::forward<Args>(args)...);
		return out;
	}


	GameManager(sf::RenderWindow* target);
	~GameManager();
};

