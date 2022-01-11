#ifndef CSHIELDMANAGER_H
#define CSHIELDMANAGER_H

#include "ChargeShield.h"
#include "Player.h"
#include "Shield.h"

class CShieldManager
{
	using cs_manager = std::vector<ChargeShield>;
	cs_manager m_shieldManager;

	sf::Sound sound_cs;
	sf::SoundBuffer buf_cs;
public:
	CShieldManager();
	~CShieldManager() = default;

	void Clear();
	void AddShield(sf::Vector2f _pos);
	void GrabChargeSword(Player*, Shield*);
	void draw(sf::RenderWindow*);
	void update(const float&);
};
#endif

