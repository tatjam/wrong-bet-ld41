#include "Node.h"

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

void Node::updateEvent(GameManager* game)
{
	if(doUpdate)
		update(game);
	
	if (showEditor)
	{
		doEditorBase();
	}

	if (propagateUpdate)
	{
		for (auto child : children)
		{
			child->updateEvent(game);
		}
	}
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

	if(doDraw)
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

std::string Node::getFullPath()
{
	std::string out;

	Node* n =  this;
	while (n != NULL)
	{
		out.insert(0, n->name);
		n = n->parent;
		if (n != NULL)
		{
			out.insert(0, "/");
		}
	}
	return out;
}

void Node::doEditorBase()
{
	bool open;

	std::string parName = getFullPath();

	ImGui::Begin(std::string("Node (" + parName + ")").c_str(), &open, sf::Vector2f(256.0f, 256.0f));

	if (!open)
	{
		showEditor = false;
	}


	float pos[2]; pos[0] = getPosition().x; pos[1] = getPosition().y;
	ImGui::InputFloat2("pos", pos, 2);
	setPosition(pos[0], pos[1]);

	float org[2]; org[0] = getOrigin().x; org[1] = getOrigin().y;
	ImGui::InputFloat2("origin", org, 2);
	setOrigin(org[0], org[1]);

	float scl[2]; scl[0] = getScale().x; scl[1] = getScale().y;
	ImGui::InputFloat2("scl", scl, 2);
	setScale(scl[0], scl[1]);

	float rot; rot = getRotation();
	ImGui::InputFloat("rot", &rot, 2);
	setRotation(rot);

	bool update = doUpdate; 
	bool propagate = propagateUpdate;
	ImGui::Checkbox("update", &update);
	ImGui::SameLine();
	ImGui::Checkbox("propagate", &propagate);
	doUpdate = update;
	propagateUpdate = propagate;


	bool draw = doDraw;
	ImGui::Checkbox("draw", &draw);
	doDraw = draw;

	int z = getZ();
	ImGui::InputInt("z", &z);
	if (z != getZ()) { setZ(z); }

	ImGui::Separator();

	if (ImGui::Button("Parent"))
	{
		if (parent != NULL)
		{
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			{
				showEditor = false;
			}

			parent->showEditor = true;
		}
	}

	ImGui::BeginChildFrame(1, sf::Vector2f(0.0f, 50.0f));

	// Children 
	for (size_t i = 0; i < children.size(); i++)
	{
		ImGui::PushID(std::to_string(i).c_str());
		if (ImGui::Button(children[i]->name.c_str()))
		{
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			{
				showEditor = false;
			}

			children[i]->showEditor = true;
		}
		ImGui::SameLine();
		ImGui::PopID();
	}

	ImGui::EndChildFrame();





	doEditor();

	ImGui::End();
}

Node::Node(std::string name)
{

	parent = NULL;

	setName(name);

	setZ(0);

	this->showEditor = false;
}

Node::~Node()
{
	// Doesn't make much sense to call it as for 
	// ~Node to be called we must be already destroyed
	//destroy();
}
