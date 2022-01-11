#pragma once
#include "SFML/Graphics.hpp"
#include "Animator.h"
#include "HitboxComponent.h"
#include "MovementComponent.h"
#include "CacheSystem.h"
#include "Functions.h"

class Entity
{
protected:
	sf::RectangleShape m_shape;

	HitboxComponent* m_hitboxComponent;
	MovementComponent* m_movementComponent;

	std::map<std::string, Animator>anim;

	sf::Vector2f m_position;

	virtual void InitShape(float _posX, float _posY, float _width, float _height, sf::Color _fillColor,
		sf::Color _outlineColor = sf::Color(sf::Color::Transparent), float _oulineThickness = 0.f);

	void CreateHitboxComponent(float _width, float _height);
	void CreateMovementComponent(float _maxVelocity, float _acceleration);

public:
	Entity();
	virtual ~Entity();

	template<class ...Args>
	void Update(Args ...args, const float& _dt);

	virtual void Render(sf::RenderWindow* _target);

	//get
	inline Animator& GetAnim(std::string _key) { return anim[_key]; }
	const sf::FloatRect getNextPositionBounds(const float& dt) const;
	inline sf::RectangleShape& GetShape() { return m_shape; }
	inline sf::Vector2f GetPos() { return m_shape.getPosition(); }
	inline sf::FloatRect GetBounds() { return m_shape.getGlobalBounds(); }

	//set
	inline void SetPos(const float _x, const float _y) { this->m_shape.setPosition(sf::Vector2f(_x, _y)); }

};

template<class ...Args>
inline void Entity::Update(Args ...args, const float& _dt)
{
	Update(args..., _dt);
}