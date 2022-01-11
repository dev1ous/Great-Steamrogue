#include "HitboxComponent.h"

HitboxComponent::HitboxComponent(sf::RectangleShape& _shape, float _width, float _height)
	:shape(_shape)
{
	this->nextPosition.left = 0.f;
	this->nextPosition.top = 0.f;

	this->hitBox.setSize(sf::Vector2f(_width, _height));
	this->hitBox.setFillColor(sf::Color::Transparent);
	//this->hitBox.setOutlineThickness(-1.f);
	//this->hitBox.setOutlineColor(sf::Color::Blue);

	this->hitBox.setOrigin(sf::Vector2f(
		this->hitBox.getGlobalBounds().width / 2,
		this->hitBox.getGlobalBounds().height / 2));

	this->hitBox.setPosition(this->shape.getPosition());
}

bool HitboxComponent::Intersect(const sf::FloatRect& _frect)
{
	return this->hitBox.getGlobalBounds().intersects(_frect);
}

void HitboxComponent::Update()
{
	this->hitBox.setPosition(this->shape.getPosition().x,
	this->shape.getPosition().y + shape.getGlobalBounds().height / 2 - hitBox.getGlobalBounds().height / 2);
}

void HitboxComponent::UpdateL()
{
	this->hitBox.setPosition(this->shape.getPosition());
}

void HitboxComponent::draw(sf::RenderWindow* _window)
{
	_window->draw(hitBox);
}
