#include "AI.h"

void AI::InitShape(std::string _filepath, float _posX, float _posY)
{
	this->m_shape.setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>(_filepath));

	this->m_shape.setSize(sf::Vector2f(
		this->m_shape.getTextureRect().width,
		this->m_shape.getTextureRect().height
	));

	this->m_shape.setOrigin(sf::Vector2f(
		this->m_shape.getTextureRect().width / 2,
		this->m_shape.getTextureRect().height / 2
	));

	this->m_position = sf::Vector2f(_posX, _posY);
	this->m_shape.setPosition(this->m_position);

	dead_ai_buffer.loadFromFile("../Ressources/Sound/robotDead.wav");
	dead_ai_sound.setBuffer(dead_ai_buffer);
	dead_ai_sound.setVolume(20.f);
}

void AI::InitComponent()
{
	this->CreateHitboxComponent(this->m_shape.getTextureRect().width, this->m_shape.getTextureRect().height);
	this->CreateMovementComponent(10000.f, 2.f);
}

void AI::InitGroundDetection()
{
	this->groundDetection.push_back(
		func::rectangle(sf::Vector2f(
			this->m_shape.getPosition().x + this->m_shape.getGlobalBounds().width / 2 + 50.f,
			this->m_shape.getPosition().y + this->m_shape.getGlobalBounds().height / 2
		),
			sf::Vector2f(10, 10), sf::Color(sf::Color::Red), sf::Color(sf::Color::Transparent), 0.f));

	this->groundDetection.push_back(
		func::rectangle(sf::Vector2f(
			this->m_shape.getPosition().x - this->m_shape.getGlobalBounds().width / 2 - 50.f,
			this->m_shape.getPosition().y + this->m_shape.getGlobalBounds().height / 2
		),
			sf::Vector2f(10, 10), sf::Color(sf::Color::Red), sf::Color(sf::Color::Transparent), 0.f));

	this->groundDetection.push_back(
		func::rectangle(sf::Vector2f(
			this->m_shape.getPosition().x + this->m_shape.getGlobalBounds().width / 2,
			this->m_shape.getPosition().y
		),
			sf::Vector2f(10, 10), sf::Color(sf::Color::Red), sf::Color(sf::Color::Transparent), 0.f));

	this->groundDetection.push_back(
		func::rectangle(sf::Vector2f(
			this->m_shape.getPosition().x - this->m_shape.getGlobalBounds().width / 2 - 25,
			this->m_shape.getPosition().y
		),
			sf::Vector2f(10, 10), sf::Color(sf::Color::Red), sf::Color(sf::Color::Transparent), 0.f));
}

AI::AI(float _posX, float _posY, Player& _player)
	:hp(3), player(&_player), movementSpeed(0.f), explode(false), receiveAttack(false), timerReceiveAttack(0.f) {}

bool AI::DetectPlayer(sf::RectangleShape _shape)
{
	if (func::isCollide(_shape, this->player->GetShape()))
		return true;

	return false;
}

bool AI::DetectPlayer(sf::CircleShape _shape)
{
	float testX = m_position.x;
	float testY = m_position.y;

	if (m_position.x < this->player->GetShape().getGlobalBounds().left)
		testX = this->player->GetShape().getGlobalBounds().left;
	else if (m_position.x > this->player->GetShape().getGlobalBounds().left + this->player->GetShape().getGlobalBounds().width)
		testX = this->player->GetShape().getGlobalBounds().left + this->player->GetShape().getGlobalBounds().width;

	if (m_position.y < this->player->GetShape().getGlobalBounds().top)
		testY = this->player->GetShape().getGlobalBounds().top;
	else if (m_position.y > this->player->GetShape().getGlobalBounds().top + this->player->GetShape().getGlobalBounds().height)
		testY = this->player->GetShape().getGlobalBounds().top + this->player->GetShape().getGlobalBounds().height;

	float distX = m_position.x - testX;
	float distY = m_position.y - testY;
	float distance = sqrt((distX * distX) + (distY * distY));

	if (distance <= _shape.getRadius())
		return true;

	return false;
}

bool AI::DetectPlayerD(sf::CircleShape _shape)
{
	float testX = m_position.x;
	float testY = m_position.y;

	if (m_position.x < this->player->GetShape().getGlobalBounds().left)
		testX = this->player->GetShape().getGlobalBounds().left;
	else if (m_position.x > this->player->GetShape().getGlobalBounds().left + this->player->GetShape().getGlobalBounds().width)
		testX = this->player->GetShape().getGlobalBounds().left + this->player->GetShape().getGlobalBounds().width;

	if (m_position.y < this->player->GetShape().getGlobalBounds().top)
		testY = this->player->GetShape().getGlobalBounds().top;
	else if (m_position.y > this->player->GetShape().getGlobalBounds().top + this->player->GetShape().getGlobalBounds().height)
		testY = this->player->GetShape().getGlobalBounds().top + this->player->GetShape().getGlobalBounds().height;

	float distX = m_position.x - testX;
	float distY = m_position.y - testY;
	float distance = sqrt((distX * distX) + (distY * distY));

	if (distance <= _shape.getRadius() * 2)
		return true;

	return false;
}

bool AI::isDead()
{
	if (this->hp <= 0) {
		return true;
		dead_ai_sound.play();
	}

	return false;
}

bool AI::hasExplode()
{
	if (this->explode) {
		return true;
	}

	return false;
}

void AI::Gravity(sf::Image& _collid, const float& _dt)
{
	if (!Get_NextPosDown(_collid, _dt))
		this->m_movementComponent->GravityUpdate(_dt);
	else
		this->m_movementComponent->StopVelocityY();
}

void AI::Update(sf::Image& _collid, Shield*, std::vector<BulletPlayer>* bulletP,const float& _dt)
{
	this->m_movementComponent->Update(_dt);
	this->m_hitboxComponent->Update();

	this->m_hitboxComponent->SetSize(
		this->m_shape.getTextureRect().width,
		this->m_shape.getTextureRect().height
	);

	this->m_hitboxComponent->SetOrigin(
		this->m_shape.getTextureRect().width / 2.f,
		this->m_shape.getTextureRect().height / 2.f);

	this->Gravity(_collid, _dt);

	this->ReceiveDamage(_dt);
}

void AI::UpdateGroundDetection()
{
	this->groundDetection[0].setPosition(
		sf::Vector2f(
			this->m_shape.getPosition().x + this->m_shape.getGlobalBounds().width / 2 + 25.f,
			this->m_shape.getPosition().y + this->m_shape.getGlobalBounds().height / 2 - this->groundDetection[0].getGlobalBounds().height / 2 + 10
		));

	this->groundDetection[1].setPosition(
		sf::Vector2f(
			this->m_shape.getPosition().x - this->m_shape.getGlobalBounds().width / 2 - 25.f,
			this->m_shape.getPosition().y + this->m_shape.getGlobalBounds().height / 2 - this->groundDetection[1].getGlobalBounds().height / 2 + 10
		));

	this->groundDetection[2].setPosition(
		sf::Vector2f(
			this->m_shape.getPosition().x + this->m_shape.getGlobalBounds().width / 2 + this->groundDetection[2].getGlobalBounds().width / 2 + 25,
			this->m_shape.getPosition().y + this->m_shape.getGlobalBounds().height / 4
		));

	this->groundDetection[3].setPosition(
		sf::Vector2f(
			this->m_shape.getPosition().x - this->m_shape.getGlobalBounds().width / 2 - this->groundDetection[3].getGlobalBounds().width / 2 - 25,
			this->m_shape.getPosition().y + this->m_shape.getGlobalBounds().height / 4
		));
}

void AI::ReceiveDamage(const float& _dt)
{
	timerReceiveAttack += _dt;
	
	if (func::isCollide(this->player->GetSword(), this->m_shape) &&
		this->timerReceiveAttack >= .5f)
		{
			this->receiveAttack = true;
			this->hp -= 1;
			this->timerReceiveAttack = 0.f;
		}
}

bool AI::isCollide(sf::Image& _collid, sf::RectangleShape _shape)
{
	if (sf::FloatRect(0, 0, _collid.getSize().y, _collid.getSize().x).intersects(_shape.getGlobalBounds()))
		return true;

	return false;
}

bool AI::Check_Colid(sf::Image& _collid, sf::Vector2f _pos)
{
	if (sf::FloatRect(0, 0, _collid.getSize().x, _collid.getSize().y).contains(_pos))
		return _collid.getPixel(_pos.x, _pos.y) == sf::Color::Black;

	return true;
}

bool AI::Get_NextPosLeft(sf::Image& _collid, const float& _dt)
{
	sf::Vector2f nextpos = m_hitboxComponent->GetPosition();
	nextpos.x -= m_hitboxComponent->GetGlobalBounds().width / 2;
	nextpos.y -= m_hitboxComponent->GetGlobalBounds().height / 2;

	bool find = false;
	for (int i = 1; i < (int)m_hitboxComponent->GetGlobalBounds().height; i++) {
		for (float y = 0; y < -m_movementComponent->GetVelocity().x * _dt + 0.1; y += 0.1) {
			if (Check_Colid(_collid, sf::Vector2f(nextpos.x - y, nextpos.y + i))) {
				if (!find) {
					m_position = sf::Vector2f((nextpos.x - y) + m_hitboxComponent->GetGlobalBounds().width / 2, m_position.y);
				}
				find = true;
			}
		}
	}

	return find;
}

bool AI::Get_NextPosRight(sf::Image& _collid, const float& _dt)
{
	sf::Vector2f nextpos = m_hitboxComponent->GetPosition();
	nextpos.x += m_hitboxComponent->GetGlobalBounds().width / 2;
	nextpos.y -= m_hitboxComponent->GetGlobalBounds().height / 2;

	bool find = false;
	for (int i = 1; i < (int)m_hitboxComponent->GetGlobalBounds().height; i++) {
		for (float y = 0; y < m_movementComponent->GetVelocity().x * _dt + 0.1; y += 0.1) {
			if (Check_Colid(_collid, sf::Vector2f(nextpos.x + y, nextpos.y + i))) {
				if (!find)
					m_position = sf::Vector2f((nextpos.x + y) - m_hitboxComponent->GetGlobalBounds().width / 2, m_position.y);
				find = true;
			}
		}
	}

	return find;
}

bool AI::Get_NextPosUp(sf::Image& _collid, const float& _dt)
{
	sf::Vector2f nextpos = m_hitboxComponent->GetPosition();
	nextpos.y -= m_hitboxComponent->GetGlobalBounds().height / 2;
	nextpos.x -= m_hitboxComponent->GetGlobalBounds().width / 2;

	for (int i = 1; i < (int)m_hitboxComponent->GetGlobalBounds().width; i++) {
		for (float y = 0; y < -m_movementComponent->GetVelocity().y * _dt + 0.1; y += 0.1) {
			if (Check_Colid(_collid, sf::Vector2f(nextpos.x + i, nextpos.y - y))) {
				m_position = sf::Vector2f(m_position.x, (nextpos.y - y) + m_hitboxComponent->GetGlobalBounds().height / 2);
				return true;
			}
		}
	}

	return false;
}

bool AI::Get_NextPosDown(sf::Image& _collid, const float& _dt)
{
	sf::Vector2f nextpos = m_hitboxComponent->GetPosition();
	nextpos.y += m_hitboxComponent->GetGlobalBounds().height / 2;
	nextpos.x -= m_hitboxComponent->GetGlobalBounds().width / 2;

	for (int i = 1; i < (int)m_hitboxComponent->GetGlobalBounds().width; i++) {
		for (float y = 0; y < m_movementComponent->GetVelocity().y * _dt + 0.1; y += 0.1) {
			if (Check_Colid(_collid, sf::Vector2f(nextpos.x + i, nextpos.y + y))) {
				m_position = sf::Vector2f(m_position.x, (nextpos.y + y) - m_shape.getGlobalBounds().height / 2);
				return true;
			}
		}
	}

	return false;
}
