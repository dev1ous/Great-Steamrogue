#pragma once
#include "Entity.h"

class Canon: public Entity
{
private:
	sf::Vector2f spawnBullet;

	float angle;
	float speed;

public:
	Canon(std:: string _filepath, sf::Vector2f _pos, int _id);
	~Canon() = default;

	void SetSpeed(float _minAngle, float _maxAngle, const sf::RectangleShape* _playerShape, float _target, float _dt);

	inline const float& GetAngle() const { return this->angle; }
	inline sf::Vector2f GetOrigin() { return this->m_shape.getOrigin(); }

	inline void SetAngle(float _angle) { this->angle = _angle; }
	inline void SetRotation(float _angle) {	this->m_shape.setRotation(_angle);	}
};