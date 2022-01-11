#include "MovementComponent.h"

MovementComponent::MovementComponent(sf::RectangleShape& _shape, float _maxVelocity, float _acceleration)
	:shape(_shape), maxVelocity(_maxVelocity), acceleration(_acceleration), gravity(200.f)
{
}

void MovementComponent::Move(const float _x, const float _y, const float& _dt)
{
	if (_x != 0)
		velocity.x = acceleration * _x;
	if (_y != 0)
		velocity.y = acceleration * _y;
}

void MovementComponent::GravityUpdate(const float& _dt)
{
	this->velocity.y += this->gravity * 10 * _dt;
}

void MovementComponent::Update(const float& _dt)
{
	if (velocity.x > maxVelocity) {
		velocity.x = maxVelocity;
	}

	if (velocity.x < -maxVelocity) {
		velocity.x = -maxVelocity;
	}
}

bool MovementComponent::isIdle()
{
	if (velocity.x == 0.f && velocity.y == 0)
		return true;

	return false;
}

bool MovementComponent::isMovingLeft()
{
	if (velocity.x < 0.f)
		return true;

	return false;
}

bool MovementComponent::isMovingRight()
{
	if (velocity.x > 0.f)
		return true;

	return false;
}

bool MovementComponent::isMovingTop()
{
	if (velocity.y < 0.f)
		return true;

	return false;
}

bool MovementComponent::isMovingBottom()
{
	if (velocity.y > 0.f)
		return true;

	return false;
}
