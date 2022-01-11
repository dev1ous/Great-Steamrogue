#ifndef MONTRES_H
#define MONTRES_H

#include "SFML/Graphics.hpp"
#include "Player.h"

class Montres
{
public:
	Montres(const std::string&, sf::Vector2f, sf::Vector2f, float);
	~Montres() = default;
	sf::CircleShape getShapeMontre() { return montre; }
	sf::Vector2f getPosMontre() { return m_pos; }
	sf::Vector2f getOriginMontre() { return m_origin; }
	void drawMontre(sf::RenderWindow*);
	void setPosMontre(sf::Vector2f pos) { montre.setPosition(pos); }
	bool CircleRect(Player*);
private:
	sf::CircleShape montre;
	sf::Vector2f m_pos;
	sf::Vector2f m_origin;
	[[maybe_unused]] float radius;
};
#endif
