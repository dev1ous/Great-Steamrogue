#include "Canon.h"

Canon::Canon(std::string _filepath, sf::Vector2f _pos, int _id)
	:angle(0), speed(200.f)
{
	this->m_shape.setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>(_filepath));

	this->m_shape.setSize(sf::Vector2f(
		this->m_shape.getTextureRect().width,
		this->m_shape.getTextureRect().height
	));

	if (_id == 1)
	{
		this->m_shape.setOrigin(sf::Vector2f(
			this->m_shape.getTextureRect().width - 11.f,
			this->m_shape.getTextureRect().height / 2
		));
	}
	else if (_id == 2)
	{
		this->m_shape.setOrigin(sf::Vector2f(
			this->m_shape.getTextureRect().width / 2,
			this->m_shape.getTextureRect().height / 2
		));
	}

	this->m_position = _pos;
	this->m_shape.setPosition(this->m_position);
}

void Canon::SetSpeed(float _minAngle, float _maxAngle, const sf::RectangleShape* _playerShape, float _target, float _dt)
{
	if (_playerShape->getPosition().y < m_shape.getPosition().y &&
		_playerShape->getPosition().x > m_shape.getPosition().x)
		this->angle = _minAngle;
	else if (_playerShape->getPosition().y < m_shape.getPosition().y &&
		_playerShape->getPosition().x < m_shape.getPosition().x)
		this->angle = _maxAngle;

	if (this->angle < _target)
		this->angle += this->speed * _dt;
	else if (this->angle >= _target - 2 || this->angle <= _target + 2)
		this->angle = _target;
	else
		this->angle -= this->speed * _dt;
}
