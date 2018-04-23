#pragma once
#include "../Node.h"

class NQuickSound : public Node
{
public:
	sf::Sound sound;

	virtual void update(GameManager* game) override;

	NQuickSound(std::string name, sf::SoundBuffer* buff);
	~NQuickSound();
};

