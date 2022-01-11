#pragma once
#include "SFML/Graphics.hpp"

class HitboxComponent
{
	sf::RectangleShape& shape;
	sf::RectangleShape hitBox;
	sf::FloatRect nextPosition;
public:
	HitboxComponent(sf::RectangleShape& _shape, float _width, float _height);
	~HitboxComponent() = default;

	bool Intersect(const sf::FloatRect& _frect);
	void Update();
	void UpdateL();
	void draw(sf::RenderWindow* _window);

	inline const sf::RectangleShape GetHitBox() const { return this->hitBox; }
	inline sf::Vector2f GetPosition() { return this->hitBox.getPosition(); }
	inline sf::FloatRect GetGlobalBounds() { return this->hitBox.getGlobalBounds(); }
	
	sf::FloatRect GetNextPosition(const sf::Vector2f& _velocity)
	{
		this->nextPosition.left = this->hitBox.getPosition().x + _velocity.x;
		this->nextPosition.top = this->hitBox.getPosition().y + _velocity.y;

		return this->nextPosition;
	}
	
	inline void setPosition(const float _x, const float _y)
	{
		this->hitBox.setPosition(_x, _y);
		this->shape.setPosition(_x, _y);
	}

	inline void SetSize(const float _x, const float _y)
	{
		this->hitBox.setSize(sf::Vector2f(_x, _y));
	}

	inline void SetOrigin(const float _x, const float _y) { this->hitBox.setOrigin(sf::Vector2f(_x, _y)); }
};

