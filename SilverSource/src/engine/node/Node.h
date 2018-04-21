#pragma once

#include <string>
#include <vector>
#include <memory>
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "../facility/GameManager.h"

using std::weak_ptr;
using std::shared_ptr;

class GameManager;

class Node : public std::enable_shared_from_this<Node>, public sf::Transformable, public sf::Drawable
{
private:

	std::vector<shared_ptr<Node>> children;
	Node* parent;
	std::string name;

	int z = 0;


	bool dirty = false;
	bool alive = true;

	bool doUpdate = true;
	bool propagateUpdate = true;
	bool doDraw = true;

public:

	sf::Transform tform;

	bool isAlive();

	// Can be NULL
	Node* getParent();

	void setParent(shared_ptr<Node> node);
	void addChildren(shared_ptr<Node> node);
	void deleteChildren(shared_ptr<Node> node);
	int getChildrenSize();
	shared_ptr<Node> getChildren(int index);
	shared_ptr<Node> getChildren(std::string name);

	bool showEditor = false;

	std::string getName();
	void setName(std::string nName);

	std::string getFullPath();

	// Destroys the node, if soft is false it will
	// also destroy all children nodes, if it's not
	// it will transfer them to our parent if any,
	// if our parent is null it will destroy them
	void destroy(bool soft = false);

	int getZ();
	void setZ(int n);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void render(sf::RenderTarget& target, sf::RenderStates states) const {}

	void updateEvent(GameManager* game);

	// It's GUARANTEED that your parents will
	// have updated before this is called. If you have
	// no parents then you must be Root
	// dt is in seconds
	virtual void update(GameManager* game) {};

	// Return true if a z-reorder is required
	bool prepareDraw(sf::Transform parent);

	void doEditorBase();
	virtual void doEditor() {};

	Node(std::string name);
	~Node();
};

