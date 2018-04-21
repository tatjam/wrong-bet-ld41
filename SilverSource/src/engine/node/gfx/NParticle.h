#pragma once
#include "../Node.h"
#include "../../util/RangeValues.h"
#include "../../util/ColorUtil.h"
#include "../../util/VectorUtil.h"

struct Particle
{
	sf::Vector2f pos;
	sf::Vector2f vel;
	float angle, angSpeed, life, colorChange, startLife;
	sf::Color color, startColor, midColor, endColor;
};

static const Particle defaultParticle = Particle
{
	{0.0f, 0.0f}, {0.0f, 0.0f}, 0.0f, 0.0f, 0.0f
};

// We use this instead of many nodes to
// save lots of computation related to z-depth
// and traversing the Node tree slowdown
class NParticle : public Node
{
private:

	std::vector<Particle> particles;

	void updateParticle(Particle* p, float dt);
	void spawnParticle(Particle* p);

	size_t maxParticles = 10;

	sf::Texture* tex;

	float spawnTimer = 0.0f;
	
public:

	bool active = true;
	// Every how many seconds does a particle spawn
	RangeFloat spawnTime;
	// If we move the node, will particles move?
	bool local = false;
	RangeFloat maxLife;

	RangeColor startColor;
	RangeColor midColor;
	RangeColor endColor;
	// 0->1, where is the mid point of the lerp?
	RangeFloat colorChange;

	RangeVector2f initialSpeed;
	RangeFloat initialAngSpeed;

	float drag;
	float angularDrag;

	sf::Vector2f constAccel;


	size_t getMaxParticles() { return maxParticles; }

	// Relatively expensive operation
	void setMaxParticles(size_t n);

	virtual void update(GameManager* game) override;
	virtual void render(sf::RenderTarget& target, sf::RenderStates states) const override;

	NParticle(std::string name, size_t particles, sf::Texture* tex);
	~NParticle();
};

