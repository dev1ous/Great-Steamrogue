#include "BasicAI.h"

void BasicAI::InitPlayerDetection()
{
	this->playerDetection.setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>("araignee_vision"));

	this->playerDetection.setRadius(this->playerDetection.getTextureRect().height);

	this->playerDetection.setOrigin(
		this->playerDetection.getGlobalBounds().width / 2,
		this->playerDetection.getGlobalBounds().height);

	this->playerDetection.setPosition(this->m_shape.getPosition());
}

void BasicAI::InitFx()
{
	this->fx.setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>("araignee_fx"));

	this->fx.setSize(sf::Vector2f(
		this->fx.getTextureRect().height,
		this->fx.getTextureRect().width
	));

	this->fx.setOrigin(
		this->fx.getGlobalBounds().width / 2,
		this->fx.getGlobalBounds().height);

	this->fx.setPosition(this->m_shape.getPosition());
}

void BasicAI::InitAnim()
{
	this->anim.emplace("MOVE", Animator(sf::IntRect(0, 0, 63, 64), 6, .1f));
	this->anim.emplace("PRE_ATTACK", Animator(sf::IntRect(0, 64, 65, 70), 3, .1f));
	this->anim.emplace("ATTACK", Animator(sf::IntRect(0, 134, 200, 140), 3, .1f));
	this->anim.emplace("DEATH", Animator(sf::IntRect(0, 352, 440, 440), 8, .07f));

	this->anim.emplace("VISION", Animator(sf::IntRect(0, 0, 431, 402), 10, .1f));

	this->anim.emplace("FX", Animator(sf::IntRect(0, 0, 45, 31), 3, .15f));
}

BasicAI::BasicAI(float _posX, float _posY, Player& _player)
	:AI(_posX, _posY, _player), moveRight(false), isPatrolling(true), gonnaExplode(false), attack(false), attackTimer(0.f), drawDetection(true), onePass(false)
{
	this->hp = 1;
	this->movementSpeed = 300.f;
	this->timeExplode = 0.f;
	this->timerDamage = 0.f;

	this->InitAnim();
	this->InitShape("AnimAraignee", _posX, _posY);
	m_shape.setTextureRect(sf::IntRect(0, 0, 63, 70));
	m_shape.setOrigin(31.5, 35);
	m_position = sf::Vector2f(_posX, _posY);
	m_shape.setPosition(_posX, _posY);
	this->InitComponent();
	this->InitFx();

	this->InitGroundDetection();
	this->InitPlayerDetection();
}


void BasicAI::FollowPlayer(sf::Image& _collid, const float& _dt)
{
	if (DetectPlayer(this->playerDetection))
	{
		if (Check_Colid(_collid, this->groundDetection[2].getPosition()) ||
			Check_Colid(_collid, this->groundDetection[3].getPosition()))
		{
			this->m_movementComponent->StopVelocityX();
			return;
		}

		this->isPatrolling = false;

		if (Get_NextPosDown(_collid, _dt) &&
			abs(func::distance(this->player->GetPos(), this->m_shape.getPosition())) <= 100.f)
		{
			attackTimer += _dt;
			this->gonnaExplode = true;
			this->drawDetection = false;

			//Attack
			if (this->attackTimer >= .5f)
			{
				this->attack = true;
				this->gonnaExplode = false;
				this->drawDetection = false;
			}
		}
		else if (Get_NextPosDown(_collid, _dt) &&
			abs(func::distance(this->player->GetPos(), this->m_shape.getPosition())) > 100.f)
		{
			this->drawDetection = true;
			this->direction = this->player->GetPos() - this->m_shape.getPosition();
			this->normailizeDir.x = this->direction.x / sqrt(pow(this->direction.x, 2) + pow(this->direction.y, 2));

			if (Get_NextPosDown(_collid, _dt))
				this->normailizeDir.y = 0;
			else
				this->normailizeDir.y += 1.f * _dt;

			this->m_movementComponent->SetVelocity(this->normailizeDir * this->movementSpeed);
		}
	}
	else
	{
		this->Patrol(_collid, _dt);
	}
}

void BasicAI::Patrol(sf::Image& _collid, const float& _dt)
{
	this->attackTimer = 0.f;

	this->isPatrolling = true;
	this->gonnaExplode = false;
	this->attack = false;
	this->drawDetection = true;

	if (Check_Colid(_collid, this->groundDetection[2].getPosition()))
		this->moveRight = false;
	else if (Check_Colid(_collid, this->groundDetection[3].getPosition()))
		this->moveRight = true;

	if (Check_Colid(_collid, this->groundDetection[0].getPosition()) &&
		!Check_Colid(_collid, this->groundDetection[1].getPosition()))
		this->moveRight = true;
	else if (Check_Colid(_collid, this->groundDetection[1].getPosition()) &&
		!Check_Colid(_collid, this->groundDetection[0].getPosition()))
		this->moveRight = false;

	if (this->moveRight)
		this->direction = this->groundDetection[0].getPosition() - this->m_shape.getPosition();
	else
		this->direction = this->groundDetection[1].getPosition() - this->m_shape.getPosition();

	this->normailizeDir.x = this->direction.x / sqrt(pow(this->direction.x, 2) + pow(this->direction.y, 2));

	if (Get_NextPosDown(_collid, _dt))
		this->normailizeDir.y = 0;
	else
		this->normailizeDir.y += 1.f * _dt;

	this->m_movementComponent->SetVelocity(this->normailizeDir * this->movementSpeed);
}

void BasicAI::UpdateDetection()
{
	this->playerDetection.setOrigin(
		this->playerDetection.getGlobalBounds().width / 2 + 3,
		this->playerDetection.getGlobalBounds().height / 2 + this->m_shape.getGlobalBounds().height / 2 - 28
	);

	this->playerDetection.setPosition
	(
		this->m_shape.getPosition().x + 4,
		this->m_shape.getPosition().y - 3
	);
}

void BasicAI::UpdateFx()
{
	this->fx.setPosition(
		this->m_shape.getPosition().x + 5,
		this->m_shape.getPosition().y - this->m_shape.getGlobalBounds().height / 2 + 8
	);
}

void BasicAI::UpdateAnim(const float& _dt)
{
	if (!this->gonnaExplode && !this->attack) {
		this->anim.at("MOVE").Animation(this->m_shape, _dt);
		this->anim.at("FX").Animation(this->fx, _dt);

		this->anim.at("VISION").AnimationC(this->playerDetection, _dt);

		onePass = false;
	}
	else if (this->gonnaExplode && !this->attack)
		this->anim.at("PRE_ATTACK").Animation(this->m_shape, _dt);
	else if (!this->gonnaExplode && this->attack)
	{
		this->anim.at("ATTACK").Animation(this->m_shape, _dt);

		this->drawDetection = false;

		if (!onePass)
		{
			onePass = true;

			this->m_shape.setPosition(sf::Vector2f(
				this->m_shape.getPosition().x,
				this->player->GetPos().y
			));
		}
	}

	if (isDead())
	{
		this->timeExplode += _dt;

		this->m_movementComponent->StopVelocity();
		this->anim.at("DEATH").Animation_NoRestart(this->m_shape, _dt);

		m_shape.setPosition(
			sf::Vector2f(
				this->m_shape.getPosition().x,
				this->player->GetPos().y + this->player->GetBounds().height / 2
			)
		);

		if (timeExplode >= 0.8f)
		{
			this->explode = true;
			this->timeExplode = 0.f;
		}
	}
}

void BasicAI::Update(sf::Image& _collid, Shield* shield, std::vector<BulletPlayer>* bulletP, const float& _dt)
{
	if (!shield->getLife())
		this->timerDamage += _dt;
	else
		this->timerDamage = 0;

	AI::Update(_collid, shield, bulletP, _dt);

	this->FollowPlayer(_collid, _dt);

	if (!this->gonnaExplode && !this->attack)
		Movable::Move(_dt);

	this->UpdateGroundDetection();
	this->UpdateFx();

	this->UpdateAnim(_dt);

	this->UpdateDetection();

	if (this->attack && shield->getLife()) {
		shield->setLife(false);
	}
	else if (this->attack && !shield->getLife() && this->timerDamage >= 1.f) {
		this->player->Life = false;
	}

	int j = 0;
	for (auto& m : *bulletP) {
		if (func::isCollide(m_shape, m.getBShape())) {
			hp -= 1;
			bulletP->erase(std::begin(*bulletP) + j);
			break;
		}
		j++;
	}
}

void BasicAI::Render(sf::RenderWindow* _target)
{
	if (!this->isDead()) {
		_target->draw(fx);

		if (this->drawDetection)
			_target->draw(playerDetection);
	}

	Entity::Render(_target);

}
