#include "NKart.h"

void NKart::checkMines(NTrack* track)
{
	for (auto mine : track->mines)
	{
		auto minel = mine.lock();
		if (minel)
		{
			float dist = getDist(minel->getPosition(), getPosition());
			if (minel->isReady() && dist <= 30.0f)
			{
				minel->explode(game);
				sf::Vector2f force = getPosition() - minel->getPosition() * 0.06f;
				speed += force;
			}
		}
	}
}

void NKart::checkHealers(NTrack* track, GameManager* game)
{
	bool healt = false;
	for (auto healer : track->healers)
	{
		float dist = getDist(getPosition(), track->toRealPos(healer.first));
		if (dist < 256.0f)
		{
			float heal = 60.0f / (dist / 50.0f);
			health += heal * game->dt;
			healt = true;
		}
	}

	if (healt && rand() % 1000 >= 960)
	{
		std::string name = "crosses";
		name.append(std::to_string(game->frame));

		auto p = game->addNode<NParticle>({}, name,
			3, game->assets.getTexture("particle/cross.png").value());

		p->setPosition(getPosition());
		p->maxLife = RangeFloat(0.6f, 1.2f);
		p->time = 0.4f;
		p->initialSpeed = RangeVector2f(this->speed + sf::Vector2f(-50.0f, -50.0f),
			this->speed + sf::Vector2f(50.0f, -50.0f));
		p->drag = 500.0f;
		p->constAccel = sf::Vector2f(0.0f, 0.0f);
		p->spawnTime = RangeFloat(0.001f, 0.005f);
		p->endColor = RangeColor({ 20, 20, 20, 0 }, { 80, 80, 40, 0 });
		p->midColor = RangeColor({ 80, 80, 80, 128 }, { 100, 100, 80, 255 });
		p->colorChange = RangeFloat(0.8f, 1.8f);
		p->initialAngSpeed = RangeFloat(-20.0f, 20.0f);
	}

}

void NKart::checkMolotov(NTrack* track, GameManager* game)
{
	for (auto fire : track->fires)
	{
		auto firel = fire.lock();
		if (firel)
		{
			if (getDist(firel->getPosition(), getPosition()) <= 30.0f)
			{
				health -= 5.0f * game->dt;
			}
		}
	}
}

void NKart::checkBullets(NTrack* track)
{
	for (auto bullet : track->bullets)
	{
		auto bulletl = bullet.lock();
		if (bulletl)
		{
			float dist = getDist(getPosition(), bulletl->getPosition());
			if (bulletl->owner.lock() != shared_from_this() && dist <= 40.0f)
			{
				bulletl->destroy();
				health -= 15.0f;
				auto s = game->addNode<NQuickSound>(shared_from_this(), "gunshot",
					game->assets.getSound("sound/metalimpact.wav").value());
				s->sound.setPitch(RangeFloat(0.7f, 1.5f)());

				auto p = game->addNode<NParticle>(getParent()->shared_from_this(), "expl",
					10, game->assets.getTexture("particle/metal.png").value());
				p->setZ(1000);
				p->setPosition(getPosition());
				p->time = 0.4f;
				p->initialSpeed = RangeVector2f(this->speed - sf::Vector2f(100.0f, 100.0f), 
					this->speed + sf::Vector2f(100.0f, 100.0f));
				p->drag = 500.0f;
				p->constAccel = sf::Vector2f(0.0f, 0.0f);
				p->spawnTime = RangeFloat(0.001f, 0.005f);
				p->endColor = RangeColor({ 20, 20, 20, 0 }, { 80, 80, 40, 0 });
				p->midColor = RangeColor({ 80, 80, 80, 128 }, { 100, 100, 80, 255 });
				p->colorChange = RangeFloat(0.8f, 1.8f);
				p->initialAngSpeed = RangeFloat(-20.0f, 20.0f);
			}
		}
	}
}


void NKart::applyForce(sf::Vector2f pos, float power)
{
	float dist = getDist(getPosition(), pos);
	if (dist <= 0.05f)
	{
		dist = 0.05f;
	}

	sf::Vector2f force = getPosition() - pos;
	force *= power * 20.0f;
	force /= std::powf(dist, 1.2f);

	speed += force;

	//std::cout << "pre: " << health;
	if (dist <= 1200.0f)
	{
		health -= (power * 20.0f) / (dist / 16.0f);
		//health -= 1000.0f;
	}
	

	//std::cout << "post: " << health << std::endl;
}



void NKart::update(GameManager* game)
{
	if (roar.getBuffer() == NULL)
	{
		roar.setBuffer(*game->assets.getSound("sound/car.wav").value());
		roar.setLoop(true);
		roar.setVolume(15.0f);
	}

	//freqPlus = 0.5f;
	/*if (rand() % 1000 >= 800)
	{
		freqPlus += game->dt * RangeFloat(0.2f, 0.5f)();
		if (freqPlus >= RangeFloat(0.8f, 2.5f)())
		{
			freqPlus = RangeFloat(-0.2f, 0.2f)();
		}
	}*/

	NTrack* parentTrack = (NTrack*)getParent();
	NSprite* wheels = (NSprite*)getChildren("wheels").get();

	if (gunAttached && parentTrack)
	{
		if (fireTime <= 0.0f)
		{
			fireTime = RangeFloat(0.6f, 2.0f)();
			auto fl = game->addNode<NFlash>(shared_from_this(), "muzzle",
				game->assets.getTexture("particle/muzzle.png").value());
			fl->sprite.setColor(sf::Color(255, 255, 255, 100));
			fl->center();
			fl->setPosition(getOrigin());

			auto s = game->addNode<NQuickSound>(shared_from_this(), "gunshot",
				game->assets.getSound("sound/shoot.wav").value());
			s->sound.setPitch(RangeFloat(0.7f, 1.5f)());

			auto bullet = game->addNode<NBullet>({}, "bullet",
				game->assets.getTexture("particle/bullet.png").value(), normalizevec(speed), 1000.0f);
			bullet->setPosition(getPosition());
			bullet->sprite.setColor(RangeColor({ 255, 255, 255 }, { 200, 200, 255 })());
			bullet->owner = shared_from_this();
			bullet->center();
			bullet->setOrigin(bullet->getOrigin() + sf::Vector2f(10.0f, 0.0f));

			parentTrack->bullets.push_back(bullet);

			shots++;
			if (shots >= RangeInt(9, 15)())
			{
				gunAttached = false;
				shots = 0;
			}
		}
		else
		{
			fireTime -= game->dt;
		}
	}

	target = getTarget(parentTrack);
	if (parentTrack != NULL)
	{
		checkBullets(parentTrack);
		checkHealers(parentTrack, game);
		checkMines(parentTrack);
		checkMolotov(parentTrack, game);

		if (getDist(target, getPosition()) < 160.0f)
		{
			auto p = parentTrack->getNextWaypoint(curLevel);
			curLevel = p.first;
			curWaypoint = p.second;
			if (curLevel == 0)
			{
				lap++;
			}
		}
	}

	float wantedHeading = getAngleBetween(getPosition(), (sf::Vector2f) target);


	float friction = getFriction(parentTrack);


	roar.setPitch(freqPlus + 1.0f);
	// Check for INCOMING wall
	int wall = parentTrack->getType(parentTrack->toPixelPos(getPosition() + this->speed * game->dt * 100.0f));
	if (wall == WALL_X || wall == WALL_Y)
	{
		this->motor = 300.0f;
		freqPlus = moveTo(freqPlus, 0.2f, game->dt * 0.3f);
		roar.setVolume(7.0f);
		if (roar.getStatus() == sf::Sound::Playing && rand() % 1000 >= 980)
			roar.stop();
		//roar.stop();
	}
	else
	{
		freqPlus = moveTo(freqPlus, 0.8f, game->dt * 0.3f);
		this->motor = 800.0f;
		//roar.setPitch(freqPlus + 1.2f);
		roar.setVolume(18.0f);
		if(roar.getStatus() == sf::Sound::Stopped && rand() % 1000 >= 995)
			roar.play();
	}

	sf::Vector2f wantedSpeed = this->motor * rotateVec({ 1.0f, 0.0f }, this->heading);
	this->speed = moveToVec(this->speed, wantedSpeed, reactibility * game->dt);
	this->heading = moveAngleTo(this->heading, wantedHeading, handling * game->dt);


	if(parentTrack != NULL)
	{
		for (auto force : parentTrack->forces)
		{
			applyForce(parentTrack->toRealPos(force.first), force.second);
		}

		int wall = parentTrack->getType(parentTrack->toPixelPos(getPosition() + this->speed * 0.05f));
		float baseDamage = isPlayer ? 0.04f : 0.02f;
		if (wall == PLACED_WALL_X || wall == PLACED_WALL_Y)
		{
			if(magnitudevec(speed) >= 100.f)
				speed -= speed * 8.0f * game->dt;
		}

		if(wall == WALL_X)
		{
			health -= baseDamage * magnitudevec(speed);
			if (magnitudevec(speed) >= 500.0f)
			{
				auto s = game->addNode<NQuickSound>(getParent()->shared_from_this(), "explSound",
					game->assets.getSound("sound/softcrash.wav").value());
				s->sound.setPitch(RangeFloat(0.9f, 1.1f)());
				s->sound.setVolume(50.0f);
			}
			else if (magnitudevec(speed) >= 300.0f)
			{
				auto s = game->addNode<NQuickSound>(getParent()->shared_from_this(), "explSound",
					game->assets.getSound("sound/lightcrash.wav").value());
				s->sound.setPitch(RangeFloat(0.5f, 1.3f)());
				s->sound.setVolume(70.0f);
			}

			// Bounce away from wall
			this->speed.x = -this->speed.x * 0.3f;
			if (std::abs(this->speed.x) < 40.0f)
			{
				if (this->speed.x < 0) { this->speed.x -= 70.0f; }
				else { this->speed.x += 70.0f; }
			}
			if(this->speed.y >= 80.0f)
				this->speed.y *= 0.2f;
			stuckTime += game->dt;
		}
		else if(wall == WALL_Y)
		{
			health -= baseDamage * magnitudevec(speed);

			if (magnitudevec(speed) >= 100.0f)
			{
				auto s = game->addNode<NQuickSound>(getParent()->shared_from_this(), "explSound",
					game->assets.getSound("sound/softcrash.wav").value());
				s->sound.setPitch(RangeFloat(0.9f, 1.1f)());
				s->sound.setVolume(50.0f);
			}
			else if (magnitudevec(speed) >= 300.0f)
			{
				auto s = game->addNode<NQuickSound>(getParent()->shared_from_this(), "explSound",
					game->assets.getSound("sound/lightcrash.wav").value());
				s->sound.setPitch(RangeFloat(0.5f, 1.3f)());
				s->sound.setVolume(70.0f);
			}

			this->speed.y = -this->speed.y * 0.3f;
			if (std::abs(this->speed.y) < 40.0f)
			{
				if (this->speed.y < 0) { this->speed.y -= 70.0f; }
				else { this->speed.y += 70.0f; }
			}
			if(this->speed.x >= 80.0f)
				this->speed.x *= 0.2f;
			stuckTime += game->dt;
		}
		else
		{
			stuckTime = 0.0f;
		}
	}




	if (stuckTime >= 10.0f || health <= 0.0f)
	{
		// BOOM!
		explode();
	}

	auto smoke = getChildren("smoke");
	if (smoke)
	{
		if (health <= 50.0f)
		{
			// Toggle smoke effect
			smoke->doDraw = true;
			smoke->doUpdate = true;
		}
		else
		{
			smoke->doDraw = false;
			smoke->doUpdate = false;
		}
	}

	this->speed = reduceVec(this->speed, friction * game->dt);

	move(this->speed * game->dt);
	float rot = (std::abs(this->speed.x) <= 1.0f && std::abs(this->speed.y) <= 1.0f) 
		? prevRot : getAngleVec(this->speed);

	setRotation(rot);
	prevRot = rot;

	if (wheels != NULL)
	{
		float angle = getAngleBetween(getPosition(), getPosition() + wantedSpeed) - getRotation();
		angle = (angle < 0) ? std::max(angle, -60.0f) : std::min(angle, 60.0f);
		wheels->setRotation(angle / 2.0f);
	}

}

void NKart::explode()
{
	if (isAlive())
	{
		auto flash = game->addNode<NFlash>(getParent()->shared_from_this(), "flash",
			game->assets.getTexture("particle/flash.png").value(), 0.1f);
		flash->center();
		flash->setPosition(getPosition());
		flash->setScale(2.0f, 2.0f);

		if (rand() % 1000 >= 600)
		{
			flash->sprite.setColor(sf::Color(255, 128, 70, 255));

			auto p = game->addNode<NParticle>(getParent()->shared_from_this(), "expl",
				100, game->assets.getTexture("particle/explode.png").value());
			p->setZ(1000);
			p->setPosition(getPosition());
			p->time = 0.4f;
			p->constAccel = sf::Vector2f(0.0f, 0.0f);
			p->initialSpeed = RangeVector2f({ -250.0f, -250.0f }, { 250.0f, 250.0f });
			p->drag = 500.0f;
			p->constAccel = sf::Vector2f(170.0f, 0.0f);
			p->spawnTime = RangeFloat(0.001f, 0.005f);
			p->endColor = RangeColor({ 20, 20, 20, 0 }, { 80, 80, 40, 0 });
			p->midColor = RangeColor({ 80, 80, 80, 128 }, { 100, 100, 80, 255 });
			p->colorChange = RangeFloat(0.1f, 0.2f);

			auto p2 = game->addNode<NParticle>(getParent()->shared_from_this(), "explDebris",
				25, game->assets.getTexture("particle/debris.png").value());
			p2->setZ(1100);
			p2->setPosition(getPosition());
			p2->time = 0.6f;
			p2->initialSpeed = RangeVector2f({ -50.0f, -70.0f }, { 50.0f, 20.0f });
			p2->spawnTime = RangeFloat(0.001f, 0.005f);
			p2->constAccel = sf::Vector2f(0.0f, 0.0f);
			p2->drag = 15.0f;

			auto s = game->addNode<NQuickSound>(getParent()->shared_from_this(), "explSound",
				game->assets.getSound("sound/explosion0.wav").value());

			// Permanent sprite
			auto crater = game->addNode<NSprite>(getParent()->shared_from_this(), "crater",
				game->assets.getTexture("particle/crater.png").value());
			crater->setPosition(getPosition());
			crater->center();
			crater->setScale(2.0f, 2.0f);
			crater->setZ(-200);
		}
		else
		{
			flash->sprite.setColor(sf::Color(255, 200, 200, 50));

			auto p = game->addNode<NParticle>(getParent()->shared_from_this(), "expl",
				20, game->assets.getTexture("particle/explode.png").value());
			p->setZ(1000);
			p->setPosition(getPosition());
			p->time = 0.4f;
			p->constAccel = sf::Vector2f(0.0f, 0.0f);
			p->initialSpeed = RangeVector2f({ -250.0f, -250.0f }, { 250.0f, 250.0f });
			p->drag = 500.0f;
			p->constAccel = sf::Vector2f(170.0f, 0.0f);
			p->spawnTime = RangeFloat(0.001f, 0.005f);
			p->endColor = RangeColor({ 20, 20, 20, 0 }, { 80, 80, 40, 0 });
			p->midColor = RangeColor({ 80, 80, 80, 128 }, { 100, 100, 80, 255 });
			p->colorChange = RangeFloat(0.1f, 0.2f);

			auto p2 = game->addNode<NParticle>(getParent()->shared_from_this(), "explDebris",
				25, game->assets.getTexture("particle/debris.png").value());
			p2->setZ(1100);
			p2->setPosition(getPosition());
			p2->time = 0.6f;
			p2->initialSpeed = RangeVector2f(this->speed - sf::Vector2f(50.0f, 50.0f), 
				this->speed + sf::Vector2f(50.0f, 50.0f));
			p2->spawnTime = RangeFloat(0.001f, 0.005f);
			p2->midColor = RangeColor({ 200, 200, 200 }, { 255, 255, 255 });
			p2->endColor = RangeColor({ 200, 200, 200, 0 }, { 255, 255, 255, 0 });
			p2->drag = 1000.0f;
			p2->angularDrag = 500.0f;
			p2->initialAngSpeed = RangeFloat(-100.0f, 100.0f);

			// Less epic breakdown of the car, leaving car debris behind
			auto crater = game->addNode<NSprite>(getParent()->shared_from_this(), "cardebris",
				game->assets.getTexture("particle/cardebris.png").value());
			crater->setPosition(getPosition());
			crater->center();
			crater->setScale(1.0f, 1.0f);
			crater->setZ(-200);

			auto s = game->addNode<NQuickSound>(getParent()->shared_from_this(), "explSound",
				game->assets.getSound("sound/crash.wav").value());

		}

		destroy();
	}
}

float NKart::getFriction(NTrack* track)
{
	if (track != NULL)
	{
		if (track->getType(track->toPixelPos(getPosition())) == MUD)
		{
			return 80.0f;
		}
	}
	return 1.0f;
}

sf::Vector2f NKart::getTarget(NTrack* track)
{
	if (track != NULL)
	{
		if (track->waypoints.size() >= curLevel)
		{
			if (track->waypoints[curLevel].size() >= curWaypoint)
			{
				return track->toRealPos(track->waypoints[curLevel][curWaypoint]);
			}
		}
	}
	return sf::Vector2f(0, 0);
}


NKart::NKart(std::string name, sf::Texture* tex, bool isPlayer, int difficulty) : NSprite(name, tex)
{
	this->isPlayer = isPlayer;

	this->heading = 0.0f;
	this->speed = sf::Vector2f(0.0f, 0.0f);
	//this->showEditor = true;
	this->prevRot = 0.0f;
	this->motor = 0.0f;
	this->curLevel = 0;
	this->curWaypoint = 0;

	if (difficulty == 0)
	{
		this->handling = 530.0f;
		this->reactibility = 520.0f;
		this->health = isPlayer ? 110.0f : 100.0f;
	}
	else if (difficulty == 1)
	{
		this->handling = isPlayer ? 530.0f : 550.0f;
		this->reactibility = isPlayer ? 520.0f : 550.0f;
		this->health = isPlayer ? 100.0f : 120.0f;
	}
	else
	{
		this->handling = isPlayer ? 520.0f : 550.0f;
		this->reactibility = isPlayer ? 520.0f : 560.0f;
		this->health = isPlayer ? 80.0f : 140.0f;
	}

	this->gunAttached = false;
	this->shots = 0;

}


NKart::~NKart()
{
	roar.stop();
}

void NKart::doEditor()
{
	float spd[2]; spd[0] = this->speed.x; spd[1] = this->speed.y;
	ImGui::InputFloat2("speed", spd, 2);
	this->speed.x = spd[0]; this->speed.y = spd[1];


	NTrack* parentTrack = (NTrack*)getParent();
	sf::Vector2f target = getTarget(parentTrack);

	float trg[2]; trg[0] = target.x; trg[1] = target.y;
	ImGui::InputFloat2("target", trg, 2);

	float hToTarg = getAngleBetween(getPosition(), (sf::Vector2f) target);
	ImGui::InputFloat("headToTarget", &hToTarg, 2);

	ImGui::InputFloat("heading", &heading, 2);
	ImGui::InputFloat("motor", &motor, 2);
	ImGui::InputFloat("handling", &handling, 2);
	ImGui::InputFloat("reactability", &reactibility, 2);
}
