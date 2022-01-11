#include "Bullet.h"
#include "CacheSystem.h"
#include <iostream>
#include "OptionState.h"

Bullet::Bullet(sf::Vector2f _pos, sf::RectangleShape _playerShape, float _angle)
:playerShape(_playerShape), speed(400), angle (_angle)
{
	this->m_shape.setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>("drone_tir"));

	this->anim.emplace("SHOOT", Animator(sf::IntRect(0, 0, 65, 13), 9, .1f));

	this->m_shape.setSize(sf::Vector2f(
		this->m_shape.getTextureRect().width,
		this->m_shape.getTextureRect().height
	));

	this->m_shape.setOrigin(sf::Vector2f(
		this->m_shape.getTextureRect().width / 2,
		this->m_shape.getTextureRect().height / 2
	));

	this->m_position = _pos;
	this->m_shape.setPosition(this->m_position);

	this->direction = this->playerShape.getPosition() - this->m_shape.getPosition();

	this->normailizeDir.x = this->direction.x / sqrt(pow(this->direction.x, 2) + pow(this->direction.y, 2));
	this->normailizeDir.y = this->direction.y / sqrt(pow(this->direction.x, 2) + pow(this->direction.y, 2));

	this->m_shape.setRotation(this->angle);

	bufferBullet.loadFromFile("../Ressources/Sound/bullet.wav");
	soundBullet.setBuffer(bufferBullet);
	soundBullet.play();
}

void Bullet::move(const float _dt)
{	
	soundBullet.setVolume(OptionState::volume);
	this->m_shape.move(this->normailizeDir * this->speed * _dt);
	m_position = this->m_shape.getPosition();

	this->anim["SHOOT"].Animation(this->m_shape, _dt);
}

void Bullet::draw(sf::RenderWindow* window)
{
	window->draw(m_shape);
}

bool Bullet::BulletCol(Player* _playerShape)
{
	if(_playerShape->GetHitbox().intersects(m_shape.getGlobalBounds()))
		return true;

	return false;
}

bool Bullet::BulletColShield(Shield* shield)
{
	float testX = m_position.x;
	float testY = m_position.y;

	if (m_position.x < shield->getShapeShield().getGlobalBounds().left)
		testX = shield->getShapeShield().getGlobalBounds().left;
	else if (m_position.x > shield->getShapeShield().getGlobalBounds().left + shield->getShapeShield().getGlobalBounds().width)
		testX = shield->getShapeShield().getGlobalBounds().left + shield->getShapeShield().getGlobalBounds().width;

	if (m_position.y < shield->getShapeShield().getGlobalBounds().top)
		testY = shield->getShapeShield().getGlobalBounds().top;
	else if (m_position.y > shield->getShapeShield().getGlobalBounds().top + shield->getShapeShield().getGlobalBounds().height)
		testY = shield->getShapeShield().getGlobalBounds().top + shield->getShapeShield().getGlobalBounds().height;

	float distX = m_position.x - testX;
	float distY = m_position.y - testY;
	float distance = sqrt((distX * distX) + (distY * distY));

	if (distance <= this->m_shape.getSize().x)
		return true;
	return false;
}
