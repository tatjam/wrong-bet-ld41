#include "GameManager.h"
#include "../node/NodeSort.h"


void GameManager::setDirty()
{
	dirty = true;
}

void GameManager::update()
{
	if (firstUpdate)
	{
		dtc.restart();
		firstUpdate = false;
	}

	auto dtt = dtc.restart();
	dt = dtt.asSeconds();

	frame++;
	time += dt;

	ImGui::SFML::Update(*target, dtt);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::BackSlash)
		&& !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
	{
		auto rootL = root.lock();
		if (rootL) { rootL->showEditor = true; }
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::BackSlash) 
		&& sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
	{
		if (!uiToolbarHeld)
		{
			uiToolbar = !uiToolbar;
			uiToolbarHeld = true;
		}
	}
	else
	{
		uiToolbarHeld = false;
	}
	
	// This is due to our shitty 
	// variable time-step physics engine
	if (dt >= 0.2f)
	{
		std::cout << "[WARN: GameManager -> update] DeltaTime very high (" << dt << "), not updating!" << std::endl;
	}
	else
	{
		// Update timers
		if (!root.expired())
		{
			root.lock()->updateEvent(this);
		}
	}

	if (uiToolbar)
	{
		doUiToolbar();
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
	node->game = this;

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
	this->uiToolbar = false;
	this->firstUpdate = true;

	sf::Vector2f siz = (sf::Vector2f)target->getSize();
	this->view = sf::View(sf::Vector2f(siz.x / 2.0f, siz.y / 2.0), siz);

}

void GameManager::doUiToolbar()
{
	if (ImGui::BeginMainMenuBar())
	{
		ImGui::LabelText("", "FPS: %f", 1.0f / dt);
		ImGui::EndMainMenuBar();
	}
}

GameManager::~GameManager()
{
}
