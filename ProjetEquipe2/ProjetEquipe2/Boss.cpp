#include "Boss.h"

void Boss::InitPlayerDetection()
{
	this->playerDetection.setSize(sf::Vector2f(
		data::WIDTH * 2,
		data::HEIGHT
	));

	/*this->playerDetection.setFillColor(sf::Color(sf::Color::Transparent));
	this->playerDetection.setOutlineColor(sf::Color(sf::Color::Yellow));
	this->playerDetection.setOutlineThickness(-1.f);*/

	this->playerDetection.setOrigin(
		this->playerDetection.getGlobalBounds().width / 2,
		this->playerDetection.getGlobalBounds().height / 2);

	this->playerDetection.setPosition(sf::Vector2f(
		this->m_shape.getPosition()
	));
}

void Boss::InitAnim()
{
	this->anim.emplace("IDLE", Animator(sf::IntRect(0, 0, 186, 256), 4, .1f));
	this->anim.emplace("MOVE", Animator(sf::IntRect(0, 256, 186, 256), 3, .1f));
	this->anim.emplace("PRE_ATTACK", Animator(sf::IntRect(0, 513, 240, 272), 3, .1f));
	this->anim.emplace("ATTACK", Animator(sf::IntRect(0, 785, 702, 472), 3, .1f));
	this->anim.emplace("DEATH", Animator(sf::IntRect(0, 1443, 1300, 1300), 8, .1f));

	this->anim.emplace("ARM_IDLE", Animator(sf::IntRect(0, 0, 186, 256), 4, .1f));
	this->anim.emplace("ARM_MOVE", Animator(sf::IntRect(0, 256, 186, 256), 1, .1f));
}

void Boss::InitArm()
{
	this->arm.setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>("boss_bras"));

	this->arm.setSize(sf::Vector2f(
		this->arm.getTextureRect().width,
		this->arm.getTextureRect().height
	));

	this->arm.setOrigin(sf::Vector2f(
		this->arm.getTextureRect().width / 2,
		this->arm.getTextureRect().height / 2
	));

	this->arm.setPosition(this->m_position);
}

void Boss::InitWeapon()
{
	this->canon = std::make_unique<Canon>("boss_arme", m_position, 2);
}

Boss::Boss(float _posX, float _posY, Player& _player)
	:AI(_posX, _posY, _player), timeBeetwenShoot(0.f), attackTimer(0.f), gonnaExplode(false), attack(false), onePass(false), isShooting(false), moveRight(false), isPatrolling(true)
{
	this->hp = 1.f;
	this->movementSpeed = 300.f;
	this->timeExplode = 0.f;

	this->InitShape("Boss", _posX, _posY);

	m_shape.setTextureRect(sf::IntRect(0, 0, 186, 256));
	m_shape.setOrigin(93, 128);
	m_shape.setPosition(_posX, _posY);

	this->InitComponent();
	this->InitAnim();

	this->InitGroundDetection();
	this->InitPlayerDetection();
	this->InitArm();
	this->InitWeapon();
}

void Boss::FollowPlayer(sf::Image& _collid, const float& _dt)
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
			abs(func::distance(this->player->GetPos(), this->m_shape.getPosition())) <= 200.f)
		{
			attackTimer += _dt;
			this->gonnaExplode = true;

			//Attack
			if (this->attackTimer >= 1.f)
			{
				this->attack = true;
				this->gonnaExplode = false;
			}
		}
		else if (Get_NextPosDown(_collid, _dt) &&
			abs(func::distance(this->player->GetPos(), this->m_shape.getPosition())) > 200.f)
		{
			if (sqrt(pow((this->player->GetPos().x - this->m_position.x), 2) + pow((this->player->GetPos().y - this->m_position.y), 2)) >= 500.f) {
			this->Shoot(_dt);
			return;
		}

			this->attackTimer = 0;
			this->attack = false;
			this->gonnaExplode = false;

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
		this->Patrol(_collid, _dt);
}

void Boss::Patrol(sf::Image& _collid, const float& _dt)
{
	this->attackTimer = 0.f;

	this->isPatrolling = true;
	this->gonnaExplode = false;
	this->attack = false;

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

void Boss::Shoot(const float& _dt)
{
	this->gonnaExplode = false;
	this->attack = false;
	this->isShooting = true;

	if (!this->isDead() &&
		this->timeBeetwenShoot >= 1.5f &&
		this->bullets.size() <= 5)
	{
		if (this->m_shape.getPosition().x >= this->player->GetPos().x)
		{
			this->bullets.push_back(std::make_shared<Bullet>(sf::Vector2f(
				this->canon->GetPos().x,
				this->canon->GetPos().y
			),
				this->player->GetShape(), this->canon->GetAngle()));
		}
		else
		{
			this->bullets.push_back(std::make_shared<Bullet>(sf::Vector2f(
				this->canon->GetPos().x,
				this->canon->GetPos().y
			),
				this->player->GetShape(), this->canon->GetAngle() - 180.f));
		}

 		this->timeBeetwenShoot = 0;
	}
}

void Boss::deleteBullets(sf::Image& _collid)
{
	int i = 0;
	for (auto& b : bullets)
	{
		bool hit = b->BulletCol(this->player);

		if (hit)
			player->Life = false;
		if (hit ||
			Check_Colid(_collid, b->GetPos()))
		{
			if (i != 0)
				i--;

			bullets.erase(bullets.begin() + i);
			break;
		}
		i++;
	}
}

void Boss::Update(sf::Image& _collid, Shield* shield, std::vector<BulletPlayer>* bullet, const float& _dt)
{
	if (!shield->getLife())
		this->timerDamage += _dt;
	else
		this->timerDamage = 0;

	this->timeBeetwenShoot += _dt;

	AI::Update(_collid, shield, bullet, _dt);

	this->FollowPlayer(_collid, _dt);

	if (!this->gonnaExplode && !this->attack)
		Movable::Move(_dt);

	this->UpdateGroundDetection();

	this->UpdateAnim(_dt);

	this->UpdateArm();
	this->UpdateWeapon(_dt);
	this->UpdateDetection();

	this->UpdateBullets(_dt);
	this->deleteBullets(_collid);

	if (this->attack && shield->getLife()) {
		shield->setLife(false);
	}
	else if (this->attack && !shield->getLife() && this->timerDamage >= 1.f) {
		this->player->Life = false;
	}

	if (!shield->inLife)
		this->deleteBullets(_collid);
	else {
		int i = 0;
		for (auto& it : bullets) {
			if (func::isCollide(it.get()->GetShape(), shield->getShapeShield())) {
				shield->setLife(false);
				bullets.erase(std::begin(bullets) + i);
				break;
			}
			i++;
		}
	}

	int j = 0;
	for (auto& m : *bullet) {
		if (func::isCollide(m_shape, m.getBShape())) {
			hp -= 5;
			bullet->erase(std::begin(*bullet) + j);
			break;
		}
		j++;
	}
}

void Boss::UpdateAnim(const float& _dt)
{
	if (this->m_movementComponent->GetVelocity().x == 0)
		this->anim.at("ARM_IDLE").Animation(this->arm, _dt);
	else
		this->anim.at("ARM_MOVE").Animation(this->arm, _dt);

	if (this->m_shape.getPosition().x < this->player->GetPos().x)
	{
		this->m_shape.setScale(-1, 1);
		this->arm.setScale(-1, 1);
		this->canon->GetShape().setScale(-1, 1);
	}
	else
	{
		this->m_shape.setScale(1, 1);
		this->arm.setScale(1, 1);
		this->canon->GetShape().setScale(1, 1);
	}

	if (!this->isShooting &&
		!this->gonnaExplode &&
		!this->attack)
	{
		this->anim.at("MOVE").Animation(this->m_shape, _dt);
	}
	else
		this->anim.at("IDLE").Animation(this->m_shape, _dt);


	if (this->gonnaExplode && !this->attack)
		this->anim.at("PRE_ATTACK").Animation(this->m_shape, _dt);
	else if (!this->gonnaExplode && this->attack)
	{
		this->anim.at("ATTACK").Animation(this->m_shape, _dt);

		this->m_shape.setOrigin(sf::Vector2f(
			this->m_shape.getTextureRect().width / 2,
			this->m_shape.getTextureRect().height / 2 + 110
		));
	}


	if (isDead())
	{
		this->m_movementComponent->StopVelocity();
		this->gonnaExplode = false;
		this->attack = false;
		this->timeExplode += _dt;

		this->m_movementComponent->StopVelocity();
		this->anim.at("DEATH").Animation_NoRestart(this->m_shape, _dt);

		if (timeExplode >= 0.8f)
		{
			this->explode = true;
			this->timeExplode = 0.f;
		}
	}
}

void Boss::UpdateDetection()
{
	this->playerDetection.setPosition
	(
		this->m_shape.getPosition().x,
		this->m_shape.getPosition().y
	);
}

void Boss::UpdateArm()
{
	this->arm.setPosition(this->m_position);
}

void Boss::UpdateWeapon(const float& _dt)
{
	this->diff = this->canon->GetPos() - this->player->GetPos();
	theta = arg(std::complex<float>(diff.x, diff.y));

	if (this->m_shape.getPosition().x < this->player->GetPos().x)
	{
		this->canon->SetPos(
			this->m_shape.getPosition().x + this->m_shape.getGlobalBounds().width / 2 - 44,
			this->m_shape.getPosition().y + 30
		);

		this->canon->SetSpeed(-30.f, 30.f, &this->player->GetShape(), theta, _dt);
		this->canon->SetRotation(this->canon->GetAngle());
	}
	else
	{
		this->canon->SetPos(
			this->m_shape.getPosition().x - 50,
			this->m_shape.getPosition().y + 30
		);

		this->canon->SetSpeed(-30.f, 30.f, &this->player->GetShape(), theta, _dt);
		this->canon->SetRotation(this->canon->GetAngle());
	}

}

void Boss::UpdateBullets(const float& _dt)
{
	for (auto& it : this->bullets)
		it->move(_dt);
}

void Boss::Render(sf::RenderWindow* _target)
{
	Entity::Render(_target);

	std::for_each(std::begin(bullets), std::end(bullets), [&_target](std::vector<std::shared_ptr<Bullet>>::reference b) {
		b->draw(_target);
		});

	if (!this->isDead() && !this->attack) {
		this->canon->Render(_target);
		_target->draw(this->arm);
	}

	/*std::for_each(std::begin(groundDetection), std::end(groundDetection), [&_target](std::vector<sf::RectangleShape>::reference r) {
		_target->draw(r);
		});*/
}