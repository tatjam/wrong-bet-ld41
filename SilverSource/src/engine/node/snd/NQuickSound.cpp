#include "NQuickSound.h"

void NQuickSound::update(GameManager* game)
{
	if (sound.getStatus() == sf::Sound::Stopped)
	{
		destroy();
	}
}

NQuickSound::NQuickSound(std::string name, sf::SoundBuffer* buff) : Node(name)
{
	this->sound.setBuffer(*buff);
	this->sound.play();
}


NQuickSound::~NQuickSound()
{
}
