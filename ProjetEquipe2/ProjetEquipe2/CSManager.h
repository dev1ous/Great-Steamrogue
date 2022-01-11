#ifndef CSMANAGER_H
#define CSMANAGER_H

#include "ChargeSword.h"
#include "Player.h"
#include "ChargeSwordHUD.h"

class Player;

class CSManager
{
	ChargeSwordHUD m_charge;
	using cs = std::vector<ChargeSword>;
	cs charge_sword;

	sf::Sound sound_c;
	sf::SoundBuffer buffer_c;
public:
	CSManager();
	~CSManager() = default;

	int getNbrCharge() { return NbrCharge; }

	void Clear();
	void AddCharge(sf::Vector2f _pos);
	void GrabChargeSword(Player*);
	void draw(sf::RenderWindow*);
	void update(const float& dt);
	void drawChargeHUD(sf::RenderWindow*);
	void updateChargeHUD(const float& dt);

	int NbrCharge{ 0 };
};
#endif

