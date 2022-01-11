#include "Entity.h"

void Entity::InitShape(float _posX, float _posY, float _width, float _height, sf::Color _fillColor, sf::Color _outlineColor, float _oulineThickness)
{
}

void Entity::CreateHitboxComponent(float _width, float _height)
{
	m_hitboxComponent = new HitboxComponent(m_shape, _width, _height);
}

void Entity::CreateMovementComponent(float _maxVelocity, float _acceleration)
{
	m_movementComponent = new MovementComponent(m_shape, _maxVelocity, _acceleration);
}

Entity::Entity()
	:m_hitboxComponent(nullptr), m_movementComponent(nullptr), m_position(sf::Vector2f(0, 0)) {}

Entity::~Entity()
{
	delete m_hitboxComponent;
	delete m_movementComponent;
}
const sf::FloatRect Entity::getNextPositionBounds(const float& dt) const
{
	if (m_hitboxComponent && m_movementComponent)
		return m_hitboxComponent->GetNextPosition(m_movementComponent->GetVelocity() * dt);
	return sf::FloatRect(-1, -1, -1, -1);
}

void Entity::Render(sf::RenderWindow* _target)
{
	_target->draw(m_shape);

	if (m_hitboxComponent)
		m_hitboxComponent->draw(_target);
}