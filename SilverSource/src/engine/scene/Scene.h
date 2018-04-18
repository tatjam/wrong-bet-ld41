#pragma once
#include "../node/Node.h"

// Example scene, simply adds a little
// structure. Showcases proper usage of the constructor
// but feel free to do wathever you want
//
// Constructor arguments:
//	game: The GameManager, duh
//	name: The name that we should assign to the root node (Optionally)
//	parent: Node to make ourselves child of (Optionally)
//	outRoot: Write the root node to this (Optionally)
// Node that while pretty much all is optional it's RECOMMENDED
class Scene
{
public:

	Scene(GameManager* game, std::string name, std::shared_ptr<Node> parent, std::shared_ptr<Node>& outRoot)
	{
		auto root = game->addNode<Node>(parent, name);
		auto child0 = game->addNode<Node>(root, "child0");
		auto child1 = game->addNode<Node>(root, "child1");

		outRoot = root;
	}
};