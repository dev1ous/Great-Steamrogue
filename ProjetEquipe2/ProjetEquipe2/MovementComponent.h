#pragma once
#include "SFML/Graphics.hpp"

class MovementComponent
{
	[[maybe_unused]] sf::RectangleShape& shape;
	float maxVelocity;
	sf::Vector2f velocity;
	float acceleration;
	float gravity;

public:
	MovementComponent(sf::RectangleShape& _shape, float _maxVelocity, float _acceleration);
	~MovementComponent() = default;

	void Move(const float _x, const float _y, const float& _dt);
	void GravityUpdate(const float& _dt);
	void Update(const float& _dt);

	bool isIdle();
	bool isMovingLeft();
	bool isMovingRight();
	bool isMovingTop();
	bool isMovingBottom();

	inline void StopVelocity() { velocity = sf::Vector2f(0.f, 0.f); }
	inline void StopVelocityX() { velocity.x = 0.f; }
	inline void StopVelocityY() { velocity.y = 0.f; }

	inline sf::Vector2f& GetVelocity() { return velocity; }
	inline float& GetGravity() { return this->gravity; }

	//set
	inline void SetVelocity(sf::Vector2f _velocity) { this->velocity = _velocity; }
};

