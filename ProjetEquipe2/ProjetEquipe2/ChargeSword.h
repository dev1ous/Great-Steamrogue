#ifndef CHARGESWORD_H
#define CHARGESWORD_H

#include "SFML/Graphics.hpp"

class ChargeSword
{
	sf::IntRect rectSword;
	sf::Sprite m_shape;
	sf::Vector2f m_pos;
	sf::Vector2f m_origin;;
	float timer{ 0.f };
public:
	ChargeSword() = default;
	ChargeSword(const std::string&, sf::Vector2f,sf::Vector2f);
	~ChargeSword() = default;

	sf::Sprite getShape() { return m_shape; }

	void setRectSword(int addRect) { rectSword.left += addRect; }
	void draw(sf::RenderWindow*);
	void AnimSword(const float&);
};
#endif

