#ifndef BULLETPLAYER_H
#define BULLETPLAYER_H

#include "SFML/Graphics.hpp"
#include "AI.h"

class BulletPlayer
{
	sf::RectangleShape m_bullet;
	sf::Vector2f m_pos;
	sf::Vector2f m_ori;
	[[maybe_unused]] float timerTirPlayer{ 0.f };
	float speed{ 0.f };
	std::map<std::string, Animator> animBul;
public:
	BulletPlayer(const std::string&, sf::Vector2f, sf::Vector2f);
	~BulletPlayer() = default;

	sf::RectangleShape& getBShape() { return m_bullet; }
	bool Check_Collid(sf::Image& _collid, sf::Vector2f _pos);
	void MoveSpeed(const float&);
	void setSpeed(float,float);
	void update(const float&);
	void draw(sf::RenderWindow*);
};
#endif

