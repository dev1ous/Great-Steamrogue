#include "RemoteAI.h"
#include "Data.h"

void RemoteAI::InitPatrol()
{
	this->patrol.push_back(
		func::rectangle(sf::Vector2f(
			this->m_shape.getPosition().x - this->m_shape.getTextureRect().width / 2 + 2000,
			this->m_shape.getPosition().y
		),
			sf::Vector2f(20, 20), sf::Color(sf::Color::Red), sf::Color(sf::Color::Transparent), 0.f));

	this->patrol.push_back(
		func::rectangle(sf::Vector2f(
			this->m_shape.getPosition().x + this->m_shape.getTextureRect().width / 2 - 2000,
			this->m_shape.getPosition().y
		),
			sf::Vector2f(20, 20), sf::Color(sf::Color::Red), sf::Color(sf::Color::Transparent), 0.f));
}

void RemoteAI::InitPlayerDetection()
{
	this->playerDetection.setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>("drone_vision"));

	this->playerDetection.setRadius(this->playerDetection.getTextureRect().height / 2.f);

	/*this->playerDetection.setOutlineColor(sf::Color(sf::Color::Yellow));
	this->playerDetection.setOutlineThickness(2.f);*/

	this->playerDetection.setOrigin(
		this->playerDetection.getGlobalBounds().width / 2,
		this->playerDetection.getGlobalBounds().height / 2);

	this->playerDetection.setPosition(sf::Vector2f(
		this->m_shape.getPosition().x,
		this->m_shape.getPosition().y
	));
}

void RemoteAI::InitAnim()
{
	this->anim.emplace("MOVE", Animator(sf::IntRect(0, 0, 106, 88), 4, .04f));
	this->anim.emplace("ATTACK", Animator(sf::IntRect(0, 176, 84, 88), 4, .04f));
	this->anim.emplace("DEATH", Animator(sf::IntRect(0, 352, 650, 650), 8, .07f));

	this->anim.emplace("PATROL", Animator(sf::IntRect(0, 0, 670, 585), 10, .07f));
	this->anim.emplace("DETECTION", Animator(sf::IntRect(0, 0, 670, 585), 10, .04f));
}

void RemoteAI::InitCanon(sf::Vector2f _pos)
{
	//salut 
	this->canon = std::make_unique<Canon>("drone_tourelle", _pos, 1);
}

RemoteAI::RemoteAI(float _posX, float _posY, Player& _player)
	:AI(_posX, _posY, _player), timeBeetwenShoot(0), goTo(1)
{
	this->hp = 1;
	this->movementSpeed = 300.f;
	this->timeExplode = 0.f;

	this->InitShape("drone", _posX, _posY);
	m_shape.setTextureRect(sf::IntRect(0, 0, 106, 88));
	m_shape.setOrigin(53, 44);
	m_position = sf::Vector2f(_posX, _posY);
	m_shape.setPosition(_posX, _posY);

	this->InitCanon(sf::Vector2f(this->m_shape.getPosition().x, this->m_shape.getPosition().y + this->m_shape.getGlobalBounds().height / 2));

	this->InitComponent();

	this->InitPatrol();
	this->InitAnim();
	this->InitPlayerDetection();
}

void RemoteAI::Move(sf::Image& _collid, const float& _dt)
{
	Movable::Move(_dt);
}

void RemoteAI::Shoot(const float& _dt)
{
	if (!this->isDead())
	{
		this->timeBeetwenShoot = 0;

		this->bullets.push_back(std::make_shared<Bullet>(sf::Vector2f(
			this->canon->GetPos().x,
			this->canon->GetPos().y
		),
			this->player->GetShape(), this->canon->GetAngle() - 180.f));
	}
}

void RemoteAI::deleteBullets(sf::Image& _collid, const float& dt)
{
	int i = 0;
	for (auto& b : bullets)
	{
		bool hit = b->BulletCol(this->player);
		if (hit) {
			player->Life = false;
		}
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

void RemoteAI::Patrol(sf::Image& _collid, const float& _dt)
{
	if (this->patrol[0].getGlobalBounds().intersects(this->m_shape.getGlobalBounds()))
		this->goTo = 1;
	else if (this->patrol[1].getGlobalBounds().intersects(this->m_shape.getGlobalBounds()))
		this->goTo = 0;

	this->direction = this->patrol[this->goTo].getPosition() - this->m_shape.getPosition();

	this->normailizeDir.x = this->direction.x / sqrt(pow(this->direction.x, 2) + pow(this->direction.y, 2));
	this->normailizeDir.y = this->direction.y / sqrt(pow(this->direction.x, 2) + pow(this->direction.y, 2));

	this->m_movementComponent->SetVelocity(this->normailizeDir * this->movementSpeed);

}

void RemoteAI::FollowPlayer(sf::Image& _collid, const float& _dt)
{
	if (DetectPlayerD(this->playerDetection))
	{
		this->isPatroling = false;

		if (timeBeetwenShoot >= 1.5f && 
			(this->canon.get()->GetAngle() >= this->theta + 178.f && this->canon.get()->GetAngle() <= this->theta + 182.f))
			this->Shoot(_dt);

		this->UpdatePatrol();

		this->direction = this->player->GetPos() - this->m_shape.getPosition();
		this->normailizeDir.x = this->direction.x / sqrt(pow(this->direction.x, 2) + pow(this->direction.y, 2));
		this->normailizeDir.y = 0;

		if (func::distance(m_shape.getPosition(), this->player->GetPos()) <= 500.f)
			m_movementComponent->StopVelocityX();
		else
			this->m_movementComponent->SetVelocity(this->normailizeDir * this->movementSpeed);
	}
	else
	{
		this->Patrol(_collid, _dt);
		this->isPatroling = true;
	}
}

void RemoteAI::UpdateDetection()
{
	if (this->isPatroling)
	{
		if (this->m_movementComponent->GetVelocity().x >= 0)
		{
			this->playerDetection.setRotation(25);
			this->playerDetection.setScale(-1, 1);

			this->playerDetection.setPosition(sf::Vector2f(
				this->m_shape.getPosition().x + this->playerDetection.getGlobalBounds().width / 2 - 96,
				this->m_shape.getPosition().y + 125
			));
		}
		else
		{
			this->playerDetection.setRotation(-25);
			this->playerDetection.setScale(1, 1);

			this->playerDetection.setPosition(sf::Vector2f(
				this->m_shape.getPosition().x - this->playerDetection.getGlobalBounds().width / 2 + 96,
				this->m_shape.getPosition().y + 125
			));
		}
	}
	else
	{
		this->playerDetection.setRotation(0);

		if (this->player->GetPos().x >= this->m_shape.getPosition().x)
		{
			this->playerDetection.setScale(-1, 1);

			this->playerDetection.setPosition(sf::Vector2f(
				this->m_shape.getPosition().x + this->playerDetection.getGlobalBounds().width / 2 + 32,
				this->m_shape.getPosition().y - 30
			));
		}
		else
		{
			this->playerDetection.setScale(1, 1);

			this->playerDetection.setPosition(sf::Vector2f(
				this->m_shape.getPosition().x - this->playerDetection.getGlobalBounds().width / 2 - 32,
				this->m_shape.getPosition().y - 30
			));
		}
	}
}

void RemoteAI::UpdatePatrol()
{
	this->patrol[0].setPosition(sf::Vector2f(
		this->m_shape.getPosition().x - this->m_shape.getGlobalBounds().width / 2 - 300,
		this->m_shape.getPosition().y));

	this->patrol[1].setPosition(sf::Vector2f(
		this->m_shape.getPosition().x + this->m_shape.getGlobalBounds().width / 2 + 300,
		this->m_shape.getPosition().y));
}

void RemoteAI::UpdateCanon(const float& _dt)
{
	this->diff = this->canon->GetPos() - this->player->GetPos();
	theta = arg(std::complex<float>(diff.x, diff.y));

	theta = theta * 180.f / data::pi;
	if (this->isPatroling)
	{
		//Repositionne le canon par rapport au sprite de l'ennemie
		if (this->m_movementComponent->GetVelocity().x >= 0)
		{
			this->canon->SetPos(this->m_shape.getPosition().x, this->m_shape.getPosition().y + this->m_shape.getGlobalBounds().height / 2);
			this->canon->GetShape().setScale(-1, 1);

			this->canon->SetAngle(25.f);
			this->canon->SetRotation(this->canon->GetAngle());
		}
		else
		{
			this->canon->SetPos(this->m_shape.getPosition().x, this->m_shape.getPosition().y + this->m_shape.getGlobalBounds().height / 2);
			this->canon->GetShape().setScale(1, 1);

			this->canon->SetAngle(-25.f);
			this->canon->SetRotation(this->canon->GetAngle());
		}
	}
	else
	{
		//Repositionne le canon par rapport au sprite de l'ennemie
		if (this->player->GetPos().x >= this->m_shape.getPosition().x)
		{
			this->canon->SetPos(this->m_shape.getPosition().x + 11.f, this->m_shape.getPosition().y + this->m_shape.getGlobalBounds().height / 2);
			this->canon->GetShape().setScale(-1, 1);

			//Rotation du canon si le joueur est à droite
			this->canon->SetSpeed(180.f, 0.f, &this->player->GetShape(), theta + 180.f, _dt);
			this->canon->SetRotation(this->canon->GetAngle());
		}
		else
		{
			this->canon->SetPos(this->m_shape.getPosition().x - 11.f, this->m_shape.getPosition().y + this->m_shape.getGlobalBounds().height / 2);
			this->canon->GetShape().setScale(1, 1);

			//Rotation du canon si le joueur est à gauche
			this->canon->SetSpeed(180.f, 0.f, &this->player->GetShape(), theta + 180.f, _dt);
			this->canon->SetRotation(this->canon->GetAngle() - 180.f);
		}
	}
}

void RemoteAI::UpdateAnim(const float& _dt)
{
	if (!isPatroling) 
	{
		if (this->player->GetPos().x >= this->m_shape.getPosition().x)
			this->m_shape.setScale(-1, 1);
		else
			this->m_shape.setScale(1, 1);

		this->anim.at("ATTACK").Animation(this->m_shape, _dt);
		this->anim.at("DETECTION").AnimationC(this->playerDetection, _dt);
	}
	else
	{
		if (this->m_movementComponent->GetVelocity().x >= 0)
			this->m_shape.setScale(-1, 1);
		else
			this->m_shape.setScale(1, 1);

		this->anim.at("MOVE").Animation(this->m_shape, _dt);
		this->anim.at("PATROL").AnimationC(this->playerDetection, _dt);
	}

	if (isDead())
	{
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

void RemoteAI::Update(sf::Image& _collid, Shield* shield,std::vector<BulletPlayer>* bulletP, const float& _dt)
{
	this->timeBeetwenShoot += _dt;

	this->Move(_collid, _dt);

	this->m_movementComponent->Update(_dt);
	this->m_hitboxComponent->Update();

	this->m_hitboxComponent->SetSize(
		this->m_shape.getTextureRect().width,
		this->m_shape.getTextureRect().height
	);

	this->m_hitboxComponent->SetOrigin(
		this->m_shape.getTextureRect().width / 2.f,
		this->m_shape.getTextureRect().height / 2.f);

	this->FollowPlayer(_collid, _dt);
	this->UpdateDetection();
	this->UpdateCanon(_dt);
	this->ReceiveDamage(_dt);

	for (auto& it : this->bullets)
	{
		it->move(_dt);
	}

	this->UpdateAnim(_dt);

	if(!shield->inLife)
		this->deleteBullets(_collid, _dt);
	else {
		int i = 0;
		for (auto& it : bullets) {
			if (func::isCollide(it.get()->GetShape(), shield->getShapeShield())) {
				shield->setLife(false);
			}
			if (func::isCollide(it.get()->GetShape(), shield->getShapeShield()) ||
				Check_Colid(_collid, it->GetPos())) {
				bullets.erase(std::begin(bullets) + i);
				break;
			}
			i++;
		}
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

void RemoteAI::Render(sf::RenderWindow* _target)
{
	std::for_each(std::begin(bullets), std::end(bullets), [&_target](std::vector<std::shared_ptr<Bullet>>::reference b) {
		b->draw(_target);
		});

	Entity::Render(_target);

	if (!this->isDead())
	{
		_target->draw(this->playerDetection);
		this->canon->Render(_target);
	}
}
