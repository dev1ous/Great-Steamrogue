#ifndef SHIELD_H
#define SHIELD_H

#include "SFML/Graphics.hpp"
#include "Animator.h"
#include "Player.h"

class Shield
{
	sf::RectangleShape m_shield;
	sf::Vector2f m_pos;
	sf::Vector2f m_origin;
	std::map<std::string, Animator > animation;
	bool leHoquetageDeLaurine{ false };
	bool leHoquetementDeLaurine{ false };
public:
	Shield() = default;
	Shield(const std::string&, sf::Vector2f, sf::Vector2f);
	~Shield() = default;
	
	bool inLife{ false };
	std::map<std::string, Animator>& getAnimShield() { return animation; }
	sf::RectangleShape& getShapeShield() { return m_shield; }
	bool getLife() { return inLife; }
	void setLife(bool ok) { inLife = ok; }
	void update(const float&);
	void updateEndAnim(const float&);
	void drawShield(sf::RenderWindow*, Player*);
};
#endif

