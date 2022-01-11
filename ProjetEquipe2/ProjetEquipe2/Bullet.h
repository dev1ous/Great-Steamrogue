#ifndef BULLET_H
#define BULLET_H

#include "SFML/Graphics.hpp"
#include "Movable.h"
#include "Shield.h"

class Bullet:public Movable
{
	const sf::RectangleShape playerShape;
	sf::CircleShape m_bullet;

	sf::Vector2f direction;
	sf::Vector2f normailizeDir;

	float speed;
	float angle;

	sf::Sound soundBullet;
	sf::SoundBuffer bufferBullet;
public:
	Bullet(sf::Vector2f _pos, sf::RectangleShape _playerShape, float _angle);
	~Bullet() = default;

	void move(const float _dt);
	void draw(sf::RenderWindow*);

	bool BulletCol(Player* _playerShape);
	bool BulletColShield(Shield*);

	inline const sf::Vector2f& GetPos() const { return this->m_shape.getPosition(); }

	inline void SetRotation(float _angle) { this->m_shape.setRotation(_angle); }
};
#endif