#include "Node.h"

void Node::addChildren(shared_ptr<Node> node)
{
	children.push_back(node);
	node->parent = this;
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

void Node::destroy()
{
	for (shared_ptr<Node> child : children)
	{
		child->parent = NULL;
	}

	// Make sure we delete ourselves from our parent too
	if (parent != NULL)
	{
		parent->deleteChildren(shared_from_this());
	}
}

int Node::getChildrenSize()
{
	return children.size();
}

weak_ptr<Node> Node::getChildren(int index)
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

weak_ptr<Node> Node::getChildren(std::string name)
{

}


std::string Node::getName()
{
	return name;
}

void Node::setName(std::string nName)
{
	name = nName;
}

Node::Node()
{
}


Node::~Node()
{
}
