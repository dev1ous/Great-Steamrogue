#ifndef CHARGESHIELD_H
#define CHARGESHIELD_H

#include "SFML/Graphics.hpp"

class ChargeShield
{
	sf::IntRect rectShield;
	sf::Sprite m_shape;
	sf::Vector2f m_pos;
	sf::Vector2f m_origin;
	float timer{ 0.f };
public:
	ChargeShield(const std::string&, sf::Vector2f, sf::Vector2f);
	~ChargeShield() = default;
	sf::Sprite getShape() { return m_shape; }
	void draw(sf::RenderWindow*);
	void AnimShield(const float&);
};
#endif

