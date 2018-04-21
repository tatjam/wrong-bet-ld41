#include "GameManager.h"
#include "../node/NodeSort.h"


void GameManager::setDirty()
{
	dirty = true;
}

void GameManager::update()
{
	auto dtt = dtc.restart();
	dt = dtt.asSeconds();

	frame++;
	time += dt;

	ImGui::SFML::Update(*target, dtt);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::BackSlash))
	{
		auto rootL = root.lock();
		if (rootL) { rootL->showEditor = true; }
	}

	// Update timers
	if (!root.expired())
	{
		root.lock()->updateEvent(this);
	}
}

void GameManager::rebuildRenderList()
{
	auto iter = renderList.begin();

	// Remove invalid data
	for (iter; iter != renderList.end(); )
	{
		if (!iter->lock())
		{
			iter = renderList.erase(iter);
		}
		else
		{
			iter++;
		}
	}


	std::sort(renderList.begin(), renderList.end(), nodeSort);
}

void GameManager::lowAddNode(std::shared_ptr<Node> parent, std::shared_ptr<Node> node)
{
	//nodes.push_back(node);
	renderList.push_back(node);
	setDirty();

	if(parent)
	{ 
		parent->addChildren(node);
	}
	else
	{
		if (!root.expired())
		{
			root.lock()->addChildren(node);
		}
		else
		{
			std::cout << "[INFO: GameManager -> loadAddNode] Could not find root, setting as root node" << std::endl;
			root = node;
		}
	}
}

std::weak_ptr<Node> GameManager::getRoot()
{
	return root;
}


void GameManager::draw(sf::RenderTarget& target, sf::RenderStates states)
{
	// TODO: Maybe don't do this?
	target.setView(view);

	// Prepare transforms
	if (root.expired())
	{
		// Root node deleted, WHAT TO DO?
		// std::cout << " [SceneManager.cpp] ROOT NODE DELETED" << std::endl;
	}
	else
	{
		bool dirty = root.lock()->prepareDraw(sf::Transform::Identity);
		if (dirty)
		{
			rebuildRenderList();
		}
	}

	auto iter = renderList.begin();

	for (iter; iter != renderList.end(); ) 
	{
		if (iter->expired())
		{
			// Remove it from the list
			iter = renderList.erase(iter);
		}
		else
		{
			std::shared_ptr<Node> s = iter->lock();
			if (!s->isAlive())
			{
				// Remove it if it's dead too, no need to draw dead stuff
				iter = renderList.erase(iter);
			}
			else
			{
				// TODO: It could also make sense to ignore nodes
				// which don't have a parent AND ARE NOT THE SCENE'S ROOT
				// As they are "outside the scene"
				s->draw(target, states);
				iter++;
			}
		}
	}

	ImGui::SFML::Render(target);
}

void GameManager::handleResize(sf::Vector2f siz)
{
	this->view.setSize(siz.x, siz.y);
}

GameManager::GameManager(sf::RenderWindow* target)
{
	assets = AssetManager();
	ImGui::SFML::Init(*target);

	this->target = target;
	this->dt = 0.0f;
	this->time = 0.0f;
	this->frame = 0;

	sf::Vector2f siz = (sf::Vector2f)target->getSize();
	this->view = sf::View(sf::Vector2f(siz.x / 2.0f, siz.y / 2.0), siz);

}


GameManager::~GameManager()
{
}
