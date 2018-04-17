#include "Node.h"
#include "../facility/GameManager.h"

void Node::setParent(shared_ptr<Node> node)
{
	if (parent != NULL)
	{
		parent->deleteChildren(shared_from_this());
	}

	parent = node.get();

	// Rename ourselves
	setName(name);
}

void Node::addChildren(shared_ptr<Node> node)
{
	children.push_back(node);
	node->setParent(shared_from_this());
}

void Node::deleteChildren(shared_ptr<Node> node)
{
	for (size_t i = 0; i < children.size(); i++)
	{
		if (children[i] == node)
		{
			children.erase(children.begin() + i);
		}
	}
}

bool Node::isAlive()
{
	return alive;
}

void Node::destroy(bool soft)
{
	if (soft && parent != NULL)
	{
		for (size_t i = 0; i < children.size(); i++)
		{
			parent->addChildren(children[i]);
			i--;
		}
	}
	else
	{
		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->destroy();
			// We must adjust i
			i--;
		}
	}

	// Make sure we delete ourselves from our parent too
	if (parent != NULL)
	{
		// We want to get removed from the drawlist
		parent->deleteChildren(shared_from_this());
	}

	alive = false;
}

int Node::getChildrenSize()
{
	return children.size();
}

shared_ptr<Node> Node::getChildren(int index)
{
	if(index >= 0 && index < getChildrenSize())
	{
		return children[index];
	}
	else
	{
		// Empty weak_ptr (Interesting syntax to say)
		return {};
	}
	
}

shared_ptr<Node> Node::getChildren(std::string name)
{
	for (auto child : children)
	{
		if (child->getName() == name)
		{
			return child;
		}
	}

	// Empty weak_ptr
	return {};
}

Node* Node::getParent()
{
	return parent;
}

std::string Node::getName()
{
	return name;
}

void Node::setName(std::string nName)
{
	// Make our name valid checking across brothers
	if (parent != NULL)
	{
		for (auto child : parent->children)
		{
			// Check for equal names
			if (child->getName() == nName && child != shared_from_this())
			{
				setName(nName + "_");
				return;
			}
		}
	}

	name = nName;
}

int Node::getZ()
{
	return z;
}

void Node::setZ(int n)
{
	z = n;
	dirty = true;
}


void Node::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RenderStates nStates = states.transform * tform;

	render(target, nStates);

	for (auto child : children)
	{
		// NOTE: We keep the original states
		target.draw(*child, states);
	}
}

bool Node::prepareDraw(sf::Transform parent)
{
	tform = parent * getTransform();

	bool result = dirty;

	// Give to children
	for (auto child : children)
	{
		bool r2 = child->prepareDraw(tform);
		if (r2)
		{
			result = true;
		}
	}

	dirty = false;

	return result;
}


Node::Node(std::string name)
{

	parent = NULL;

	setName(name);

	setZ(0);

}

Node::~Node()
{
	// Doesn't make much sense to call it as for 
	// ~Node to be called we must be already destroyed
	//destroy();
}
