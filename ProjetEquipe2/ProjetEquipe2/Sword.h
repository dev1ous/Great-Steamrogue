#ifndef SWORD_H
#define SWORD_H

#include "SFML/Graphics.hpp"
#include "AI.h"
#include "Player.h"

class Sword
{
	sf::IntRect swordRect;
	sf::Sprite m_sword;
	sf::Vector2f m_pos;
	sf::Vector2f m_origin;
	float timer{ 0.f };
	[[maybe_unused]] int damage = 10;

public:
	Sword(const std::string&, sf::Vector2f,sf::Vector2f);
	~Sword() = default;

	/*int DamageInflicted(AI*,Player*);*/
	void handleEvents(const sf::Event&,const float&);
	void updateAnim(const float&);
	void updateCol(std::vector<AI>&,std::vector<Tiles>&);
	void draw(sf::RenderWindow*);
};
#endif
