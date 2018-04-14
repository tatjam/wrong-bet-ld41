#pragma once

#include <vector>
#include <memory>
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"


using std::weak_ptr;
using std::shared_ptr;


class Node : public std::enable_shared_from_this<Node>, public sf::Transformable, public sf::Drawable
{
private:

	std::vector<shared_ptr<Node>> children;
	Node* parent;
	std::string name;

public:

	void addChildren(shared_ptr<Node> node);
	void deleteChildren(shared_ptr<Node> node);
	int getChildrenSize();
	weak_ptr<Node> getChildren(int index);
	weak_ptr<Node> getChildren(std::string name);


	std::string getName();
	void setName(std::string nName);


	void destroy();


	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void render(sf::RenderTarget& target, sf::RenderStates states) const {}


	Node(std::string name);
	~Node();
};

