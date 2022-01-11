#include "Movable.h"

void Movable::Move(const float _dt)
{
	m_position.x += m_movementComponent->GetVelocity().x * _dt;
	m_position.y += m_movementComponent->GetVelocity().y * _dt;

	this->m_shape.setPosition(this->m_position);
}
