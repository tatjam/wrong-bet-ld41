#include "NParticle.h"




void NParticle::setMaxParticles(size_t n)
{
	maxParticles = n;
	particles.resize(n, defaultParticle);
}

void NParticle::updateParticle(Particle* p, float dt)
{
	p->pos += p->vel * dt;
	p->angle += p->angSpeed * dt;
	p->life -= dt;

	if (p->life > 0.0f)
	{
		// Calculate color change
		float lifeOfTotal = 1.0f - (p->life / p->startLife);

		if (lifeOfTotal < p->colorChange)
		{
			float pl = 1.0f / p->colorChange;
			p->color = lerpColor(p->startColor, p->midColor, lifeOfTotal * pl);
		}
		else
		{
			float pl2 = 1.0f / (1.0 - p->colorChange);
			p->color = lerpColor(p->midColor, p->endColor, (lifeOfTotal - p->colorChange) * pl2);
		}
	}
	else
	{
		p->color = p->endColor;
	}
}

void NParticle::spawnParticle(Particle* p)
{
	p->life = 10.0f;
	p->startLife = 10.0f;
	p->pos = { 0.0f, 0.0f };
	p->vel = initialSpeed();
	p->angSpeed = initialAngSpeed();

	p->color = startColor();
	p->startColor = startColor();
	p->midColor = midColor();
	p->endColor = endColor();
	p->colorChange = colorChange();

}



void NParticle::update(GameManager* game)
{
	int alive = 0;
	int toSpawn = 0;
	spawnTimer -= game->dt;
	if (spawnTimer <= 0.0f)
	{
		toSpawn = std::round(std::abs(spawnTimer) / spawnTime + 1.0f);
		spawnTimer = spawnTime;
	}

	if (active)
	{
		for (size_t i = 0; i < particles.size(); i++)
		{
			if (particles[i].life > 0.0f)
			{
				updateParticle(&particles[i], game->dt);
				alive++;
			}
			else
			{
				for (size_t j = 0; j < toSpawn; j++)
				{
					spawnParticle(&particles[i]);
					toSpawn--;
				}
			}
		}
	}

	std::cout << "Alive: " << alive << std::endl;
}

void NParticle::render(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Sprite drawer = sf::Sprite();
	drawer.setTexture(*tex);

	for (size_t i = 0; i < particles.size(); i++)
	{
		if (particles[i].life > 0.0f)
		{
			drawer.setPosition(particles[i].pos);
			drawer.setRotation(particles[i].angle);
			drawer.setColor(particles[i].color);
			target.draw(drawer, states);
		}
	}
}


NParticle::NParticle(std::string name, size_t particles, sf::Texture* tex) : Node(name),
	initialSpeed(sf::Vector2f(-10.0f, -10.0f), sf::Vector2f(10.0f, 10.0f)),
	initialAngSpeed(-50.0f, 50.0f),
	startColor(sf::Color(255, 255, 255), sf::Color(255, 255, 255)),
	midColor(sf::Color(255, 0, 0), sf::Color(255, 0, 0)),
	endColor(sf::Color(255, 0, 255, 0), sf::Color(255, 0, 255, 0)),
	colorChange(0.4f, 0.6f),
	maxLife(4.0f, 5.0f)
{
	setMaxParticles(particles);

	this->tex = tex;
	this->spawnTime = 0.1f;
	this->active = true;

	setZ(-100);
}


NParticle::~NParticle()
{

}
