#include "EliteAI.h"

void EliteAI::InitAreaOfDetection()
{
	this->playerDetection.setSize(sf::Vector2f(500, this->m_shape.getGlobalBounds().height));
	this->playerDetection.setFillColor(sf::Color(sf::Color::Transparent));

	this->playerDetection.setPosition(sf::Vector2f(
		this->m_shape.getPosition().x - this->m_shape.getGlobalBounds().width / 2 - this->playerDetection.getGlobalBounds().width,
		this->m_shape.getPosition().y - this->m_shape.getGlobalBounds().height / 2));

	this->playerDetection.setOutlineColor(sf::Color(sf::Color::Yellow));
	this->playerDetection.setOutlineThickness(-1.f);
}

void EliteAI::InitAnim()
{
	this->anim.emplace("IDLE", Animator(sf::IntRect(0, 0, 107, 192), 8, .1f));
	this->anim.emplace("PRE_ATTACK", Animator(sf::IntRect(0, 192, 139, 192), 2, .1f));
	this->anim.emplace("ATTACK", Animator(sf::IntRect(0, 384, 173, 192), 4, .1f));
	this->anim.emplace("END_ATTACK", Animator(sf::IntRect(0, 576, 139, 192), 2, .1f));
	this->anim.emplace("DEATH", Animator(sf::IntRect(0, 830, 650, 650), 8, .07f));

	this->anim.emplace("FX", Animator(sf::IntRect(0, 0, 45, 50), 6, .1f));
	this->anim.emplace("FX_2", Animator(sf::IntRect(0, 0, 326, 70), 6, .07f));
}

void EliteAI::InitFx()
{
	this->fx.setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>("garde_fx"));

	this->fx.setSize(sf::Vector2f(
		this->fx.getTextureRect().height,
		this->fx.getTextureRect().width
	));

	this->fx.setOrigin(
		this->fx.getGlobalBounds().width / 2,
		this->fx.getGlobalBounds().height);

	this->fx.setPosition(this->m_shape.getPosition());
}

void EliteAI::InitFx_2()
{
	this->fx_2.setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>("garde_fx2"));

	this->fx_2.setSize(sf::Vector2f(
		this->fx_2.getTextureRect().height,
		this->fx_2.getTextureRect().width
	));

	this->fx_2.setOrigin(
		this->fx_2.getGlobalBounds().width / 2,
		this->fx_2.getGlobalBounds().height);

	this->fx_2.setPosition(this->m_shape.getPosition());
}

EliteAI::EliteAI(float _posX, float _posY, Player& _player)
	:AI(_posX, _posY, _player), detectPlayer(false), goToPlayer(0)
{
	this->movementSpeed = 1000.f;
	this->hp = 3;
	this->timeExplode = 0.f;

	this->InitShape("garde", _posX, _posY);
	m_shape.setTextureRect(sf::IntRect(0, 0, 107, 192));
	m_shape.setOrigin(53.5, 96);
	m_position = sf::Vector2f(_posX, _posY);
	m_shape.setPosition(_posX, _posY);

	this->CreateHitboxComponent(64, 192);
	this->CreateMovementComponent(10000.f, 2.f);

	this->InitAreaOfDetection();
	this->InitGroundDetection();
	this->InitFx();
	this->InitFx_2();
	this->InitAnim();
}

void EliteAI::FollowPlayer(sf::Image& _collid, const float& _dt)
{
	this->detectPlayer = false;

	if (DetectPlayer(this->playerDetection))
	{
		this->detectPlayer = true;
		this->goToPlayer += _dt;
	}
	else {
		this->m_movementComponent->StopVelocityX();
	}

	if (this->goToPlayer >= 1.2f && this->detectPlayer)
	{
		if (Check_Colid(_collid, this->groundDetection[2].getPosition()) ||
			Check_Colid(_collid, this->groundDetection[3].getPosition()))
		{
			this->m_movementComponent->StopVelocityX();
			return;
		}

		this->goToPlayer = 0;

		this->direction = this->player->GetPos() - this->m_shape.getPosition();
		this->normailizeDir.x = this->direction.x / sqrt(pow(this->direction.x, 2) + pow(this->direction.y, 2));

		if (Get_NextPosDown(_collid, _dt))
			this->normailizeDir.y = 0;
		else
			this->normailizeDir.y += 1.f * _dt;

		this->m_movementComponent->SetVelocity(this->normailizeDir * this->movementSpeed);

		if (func::distance(this->player->GetPos(), this->m_shape.getPosition()) <= 100.f)
			this->m_movementComponent->StopVelocityX();
	}
}

void EliteAI::UpdateDetection()
{
	if (!this->isDead())
	{
		this->playerDetection.setSize(sf::Vector2f(500, this->m_shape.getTextureRect().height));

		this->playerDetection.setPosition(sf::Vector2f(
			this->m_shape.getPosition().x - this->m_shape.getGlobalBounds().width / 2 - this->playerDetection.getGlobalBounds().width /*- 250*/,
			this->m_shape.getPosition().y - this->m_shape.getGlobalBounds().height / 2));
	}
}

void EliteAI::Update(sf::Image& _collid, Shield* shield, std::vector<BulletPlayer>* bullet, const float& _dt)
{
	if (!shield->getLife())
		this->timerDamage += _dt;
	else
		this->timerDamage = 0;

	if (!this->isDead()) {
		Movable::Move(_dt);
		AI::Update(_collid, shield, bullet, _dt);

		this->m_hitboxComponent->SetSize(
			this->m_shape.getTextureRect().width - 30,
			this->m_shape.getTextureRect().height
		);
	}
	

	this->UpdateDetection();
	this->FollowPlayer(_collid, _dt);
	this->UpdateFx();
	this->UpdateFx_2();
	this->UpdateGroundDetection();
	this->UpdateAnim(_dt);

	int j = 0;
	for (auto& m : *bullet) {
		if (func::isCollide(m_shape, m.getBShape())) {
			hp -= 3;
			bullet->erase(std::begin(*bullet) + j);
			break;
		}
		j++;
	}

	if (!player->Get_Dash()) {
		if (this->attack && this->m_shape.getGlobalBounds().intersects(this->player->GetHitbox()) && shield->getLife())
			shield->setLife(false);
		else if (this->attack && this->m_hitboxComponent->GetGlobalBounds().intersects(this->player->GetHitbox()) && !shield->getLife() && this->timerDamage >= 1.f)
			this->player->Life = false;
	}

}

void EliteAI::UpdateAnim(const float& _dt)
{
	if (this->m_shape.getPosition().x < this->player->GetPos().x)
	{
		this->m_shape.setScale(-1, 1);

		this->playerDetection.setPosition(sf::Vector2f(
			this->m_shape.getPosition().x + this->m_shape.getGlobalBounds().width / 2,
			this->m_shape.getPosition().y - this->m_shape.getGlobalBounds().height / 2));
	}
	else
	{
		this->m_shape.setScale(1, 1);

		this->playerDetection.setPosition(sf::Vector2f(
			this->m_shape.getPosition().x - this->m_shape.getGlobalBounds().width / 2 - this->playerDetection.getGlobalBounds().width,
			this->m_shape.getPosition().y - this->m_shape.getGlobalBounds().height / 2));
	}

	this->anim.at("FX").Animation(this->fx, _dt);
	this->anim.at("FX_2").Animation(this->fx_2, _dt);

	if (this->m_movementComponent->GetVelocity().x == 0)
		this->anim.at("IDLE").Animation(this->m_shape, _dt);
	else if (this->m_movementComponent->GetVelocity().x <= 0)
		this->anim.at("ATTACK").Animation(this->m_shape, _dt);

	if (isDead())
	{
		this->attack = false;
		this->timeExplode += _dt;

		this->m_movementComponent->StopVelocity();
		this->anim.at("DEATH").Animation_NoRestart(this->m_shape, _dt);

		if (timeExplode >= .8f)
		{
			this->explode = true;
			this->timeExplode = 0.f;
		}
	}
}

void EliteAI::UpdateFx()
{
	this->fx.setScale(2, 2);

	this->fx.setPosition(
		this->m_shape.getPosition().x,
		this->m_shape.getPosition().y - this->m_shape.getGlobalBounds().height / 2 + 22
	);
}

void EliteAI::UpdateFx_2()
{
	this->fx_2.setPosition(
		this->m_shape.getPosition().x - 10,
		this->m_shape.getPosition().y - this->m_shape.getGlobalBounds().height / 2 + 30
	);
}

void EliteAI::Render(sf::RenderWindow* _target)
{
	Entity::Render(_target);

	if (!this->isDead()) {

		_target->draw(fx);

		if (detectPlayer)
			_target->draw(fx_2);
	}

}
