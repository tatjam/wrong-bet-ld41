#pragma once
#include <vector>
#include <algorithm>
#include <iostream>

#include "../node/Node.h"


// NOTE: You cannot remove a node here, remove it 
// by calling node->destroy. Keep in mind the node won't
// deallocate until all references to it are dead. This can 
// easily cause memory leaking.



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

public:

	void setDirty();

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);


	template <class T, class ...Args>
	std::shared_ptr<T> addNode(std::shared_ptr<Node> parent, Args && ...args)
	{
		auto node = std::make_shared<T>(std::forward<Args>(args)...);
		lowAddNode(parent, node);
		return node;
	}


	GameManager();
	~GameManager();
};

