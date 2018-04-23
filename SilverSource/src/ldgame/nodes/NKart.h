#pragma once
#include "../../engine/node/gfx/NSprite.h"
#include "../../engine/node/gfx/NParticle.h"
#include "../../engine/node/snd/NQuickSound.h"
#include "../../engine/util/VectorUtil.h"
#include "NFlash.h"
#include "NTrack.h"
#include "NBullet.h"

// A kart must be children of a track to do
// senseful stuff
class NKart : public NSprite
{
private:

	sf::Sound roar;

	float getFriction(NTrack* track);
	sf::Vector2f getTarget(NTrack* track);

	float prevRot;

	uint8_t curLevel;
	size_t curWaypoint;
	sf::Vector2f target;

	float stuckTime = 0.0f;


	float fireTime = 0.0f;

	float freqPlus = 0.0f;

	bool isPlayer;

	void applyForce(sf::Vector2f pos, float power);
	void checkBullets(NTrack* track);
	void checkHealers(NTrack* track, GameManager* game);
	void checkMines(NTrack* track);
	void checkMolotov(NTrack* track, GameManager* game);

public:

	sf::Vector2f speed;
	float heading;
	float motor;
	float handling;
	float reactibility;
	float health;
	int lap = 0;

	bool gunAttached;
	int shots;

	void explode();

	virtual void doEditor() override;
	virtual void update(GameManager* game);

	virtual void destroy()
	{
		roar.stop();
		NSprite::destroy();
	}

	NKart(std::string name, sf::Texture* tex, bool isPlayer, int difficulty);
	~NKart();
};

