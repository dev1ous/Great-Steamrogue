#include "Player.h"
#include "Functions.h"
#include "CacheSystem.h"
#include <thread>
#include "AI.h"
#include "OptionState.h"

Player& Player::init(std::string _filepath, std::string filepathAttack, sf::Vector2f _pos)
{
	anim.emplace("idle", Animator(sf::IntRect(0, 0, 197, 140), 4, 0.15f));
	anim.emplace("Walk_right", Animator(sf::IntRect(0, 140, 155, 140), 5, 0.15f));

	anim.emplace("Jump_top", Animator(sf::IntRect(0, 280, 164, 144), 5, 0.15f, 0));
	anim.emplace("Jump_down", Animator(sf::IntRect(815, 280, 164, 144), 3, 0.15f, 0));

	anim.emplace("Dash", Animator(sf::IntRect(0, 424, 125, 138), 4, 0.2f));
	anim.emplace("Attack_spe", Animator(sf::IntRect(0, 561, 242, 167), 7, 0.2f));
	anim.emplace("Grab", Animator(sf::IntRect(0, 754, 60, 137), 1, 0.3f));
	anim.emplace("Mort", Animator(sf::IntRect(0, 891, 278, 125), 4, 0.15f));

	anim.emplace("Beat_right", Animator(sf::IntRect(0, 0, 218, 131), 3, 0.15f));
	anim.emplace("Beat_up", Animator(sf::IntRect(0, 132, 226, 161), 3, 0.15f));
	anim.emplace("Beat_down", Animator(sf::IntRect(0, 292, 200, 150), 3, 0.15f));

	dashBuffer.loadFromFile("../Ressources/Sound/son/dash.wav");
	dash.setBuffer(dashBuffer);
	runBuffer.loadFromFile("../Ressources/Sound/running.wav");
	run.setBuffer(runBuffer);
	grabBuffer.loadFromFile("../Ressources/Sound/grabWall.wav");
	grab.setBuffer(grabBuffer);
	sword_b.loadFromFile("../Ressources/Sound/son/sword.wav");
	sword_s.setBuffer(sword_b);
	attackSpe_b.loadFromFile("../Ressources/Sound/son/tir_special.wav");
	attackSpe_s.setBuffer(attackSpe_b);
	pique_b.loadFromFile("../Ressources/Sound/piques.wav");
	pique_s.setBuffer(pique_b);

	this->InitShape(_filepath, filepathAttack, _pos.x, _pos.y);
	this->CreateHitboxComponent(64, 128);
	this->CreateMovementComponent(600, 2);
	this->m_position = _pos;
	return *this;
}

void Player::InitShape(std::string _filepath, std::string filepathAttack, float _posX, float _posY)
{
	this->m_shape.setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>(_filepath));
	this->m_shapeAttack.setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>(filepathAttack));
	this->m_shape.setTextureRect(sf::IntRect(0, 0, 197, 140));
	this->m_shape.setSize(sf::Vector2f(197, 140));

	this->m_shape.setOrigin(sf::Vector2f(
		this->m_shape.getGlobalBounds().width / 2, m_shape.getGlobalBounds().height - 64));

	this->m_position = sf::Vector2f(_posX, _posY);
	this->m_shape.setPosition(this->m_position);

}

void Player::Move_Dash(sf::Image& _collid, const float& _dt)
{
	pique_s.setVolume(OptionState::volume);
	attackSpe_s.setVolume(OptionState::volume);
	sword_s.setVolume(OptionState::volume);
	grab.setVolume(OptionState::volume);
	run.setVolume(OptionState::volume);
	dash.setVolume(OptionState::volume);

	this->timer_dash += _dt;

	if (func::IsDirectionPressed(sf::Keyboard::Key::LShift, func::PadAxis::JoystickGachettes, 30) && !Dash &&
		this->timer_dash > this->Cooldown && Dash_Number > 0) {
		if (func::IsDirectionPressed(sf::Keyboard::Key::D, func::PadAxis::JoystickGaucheHori, 30) && !Get_NextPosRight(_collid, _dt)) {
			Direction = Dir::Right;
			Dash = true;
			this->timer_dash = 0;
			this->m_movementComponent->StopVelocityX();
			if (m_movementComponent->GetVelocity().y > 0)
				this->m_movementComponent->StopVelocityY();
			Dash_Number--;
			WallJump = false;
			dash.play();
		}
		if (func::IsDirectionPressed(sf::Keyboard::Key::Q, func::PadAxis::JoystickGaucheHori, -30) && !Get_NextPosLeft(_collid, _dt)) {
			Direction = Dir::Left;
			Dash = true;
			this->timer_dash = 0;
			this->m_movementComponent->StopVelocityX();
			if (m_movementComponent->GetVelocity().y > 0)
				this->m_movementComponent->StopVelocityY();
			Dash_Number--;
			WallJump = false;
			dash.play();
		}
	}

	if (this->Dash == true) {
		if (this->Direction == Dir::Right) {
			this->m_movementComponent->Move(Speed_Dash, 0, _dt);

			if (Get_NextPosRight(_collid, _dt)) {
				this->m_movementComponent->StopVelocityX();
			}
		}
		if (this->Direction == Dir::Left) {
			this->m_movementComponent->Move(-Speed_Dash, 0, _dt);

			if (Get_NextPosLeft(_collid, _dt)) {
				this->m_movementComponent->StopVelocityX();
			}
		}

		if (this->timer_dash > 0.2f) {
			this->Dash = false;
			this->timer_dash = 0;
		}
	}
}

void Player::Move_Jump(sf::Image& _collid, const float& _dt)
{
	if (Get_NextPosRight(_collid, _dt)) { m_movementComponent->StopVelocityX(); }
	if (Get_NextPosLeft(_collid, _dt)) { m_movementComponent->StopVelocityX(); }

	if (func::IsButtonPressed(sf::Keyboard::Key::Space, 0) && ReJump) {
		run.stop();

		if ((Grab_Left || Grab_Right) && !Get_NextPosDown(_collid, _dt)) {
			if (Grab_Left) {
				grab.play();
				this->m_movementComponent->Move(KnockBack_WallJump, 0, _dt);
				Direction = Dir::Right;
				this->m_movementComponent->Move(0, -Power_Jump, _dt);
				WallJump = true;
				if (Get_NextPosRight(_collid, _dt))
					m_movementComponent->StopVelocity();

				ReJump = false;
			}
			if (Grab_Right) {
				this->m_movementComponent->Move(-KnockBack_WallJump, 0, _dt);
				Direction = Dir::Left;
				this->m_movementComponent->Move(0, -Power_Jump, _dt);
				WallJump = true;
				if (Get_NextPosLeft(_collid, _dt))
					m_movementComponent->StopVelocity();

				ReJump = false;
			}
		}
		else if (this->Jump_Number > 0) {
			this->m_movementComponent->StopVelocityY();

			if (this->Jump == false) {
				this->Jump = true;
				this->Jump_Number -= 1;
				this->m_movementComponent->Move(0, -Power_Jump * 1.2, _dt);
				ReJump = false;
			}
			else {
				this->Jump_Number -= 2;
				Dash_Number = 1;
				this->m_movementComponent->Move(0, -Power_Jump, _dt);
				ReJump = false;
			}

		}

		if (Get_NextPosUp(_collid, _dt))
			this->m_movementComponent->StopVelocityY();

		if (Jump) {
			anim["Jump_top"].Restart_Anim();
			anim["Jump_down"].Restart_Anim();
		}
	}

	if (!func::IsButtonPressed(sf::Keyboard::Key::Space, 0)) {
		ReJump = true;
	}
}

void Player::Attack(sf::Image& _collid, std::vector<BulletPlayer>* bullet, CSManager* mana, const float& _dt)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Joystick::getAxisPosition(0, (sf::Joystick::Axis)func::PadAxis::JoystickGachettes) < -30) {
		sword_s.play();
		if (func::IsDirectionPressed(sf::Keyboard::Key::Z, func::JoystickGaucheVert, -30)) {
			FightTop = true;
			Sword.setSize({ 50,100 });
			Sword.setOrigin({ 25,50 });
			if (Direction == Dir::Left) {
				Sword.setPosition(this->GetPos().x - 60, this->GetPos().y - 50);
			}
			if (Direction == Dir::Right) {
				Sword.setPosition(this->GetPos().x + 60, this->GetPos().y - 50);
			}
		}
		else if (func::IsDirectionPressed(sf::Keyboard::Key::S, func::JoystickGaucheVert, 30)) {
			FightDown = true;
			Sword.setSize({ 50,100 });
			Sword.setOrigin({ 25,50 });
			if (Direction == Dir::Left) {
				Sword.setPosition(this->GetPos().x - 30, this->GetPos().y + 75);
			}
			if (Direction == Dir::Right) {
				Sword.setPosition(this->GetPos().x + 30, this->GetPos().y + 75);
			}
		}
		else {
			FightHori = true;
			Sword.setSize({ 100,50 });
			Sword.setOrigin({ 50,25 });
			if (Direction == Dir::Left) {
				Sword.setPosition(this->GetPos().x - 75, this->GetPos().y);
			}
			if (Direction == Dir::Right) {
				Sword.setPosition(this->GetPos().x + 75, this->GetPos().y);
			}
		}
	}
	if (anim["Beat_right"].Get_End() || anim["Beat_up"].Get_End() || anim["Beat_down"].Get_End()) {
		FightTop = false;
		FightDown = false;
		FightHori = false;

		Sword.setSize({ 0, 0 });

		anim["Beat_right"].Restart_Anim();
		anim["Beat_up"].Restart_Anim();
		anim["Beat_down"].Restart_Anim();
	}

	if ((sf::Mouse::isButtonPressed(sf::Mouse::Right) || sf::Joystick::isButtonPressed(0, 5)) && !SpeHori && mana->NbrCharge > 2) {
		attackSpe_s.play();
		anim["Attack_spe"].Restart_Anim();
		SpeHori = true;
	}
	if (anim["Attack_spe"].getShot() && SpeHori && !IsShoot) {
		Shot = true;
		IsShoot = true;
	}
	if (Shot) {
		if (Direction == Dir::Right) {
			bullet->push_back(BulletPlayer("joueur_tir", sf::Vector2f(GetPos().x + 150, GetPos().y), sf::Vector2f(43.5, 24)));
			bullet->back().setSpeed(600, 1);
		}
		if (Direction == Dir::Left) {
			bullet->push_back(BulletPlayer("joueur_tir", sf::Vector2f(GetPos().x - 150, GetPos().y), sf::Vector2f(43.5, 24)));
			bullet->back().setSpeed(-600, -1);
		}
		mana->NbrCharge = 0;
		Shot = false;
	}

	for (auto& b : *bullet) {
		b.update(_dt);
		b.MoveSpeed(_dt);
	}

	if (anim["Attack_spe"].Get_End()) {
		SpeHori = false;
		IsShoot = false;
		anim["Attack_spe"].Restart_Anim();
	}
}

void Player::AffectGravity(sf::Image& _collid, const float& _dt)
{
	if (Life && !SpeHori) {
		if (Get_NextPosDown(_collid, _dt)) {
			if (this->m_movementComponent->GetVelocity().y > 0) {
				this->m_movementComponent->StopVelocityY();
			}

			Jump_Number = 2;
			Dash_Number = 1;
			this->Jump = false;
			WallJump = false;
		}
		else if ((Grab_Left || Grab_Right) && (Left || Right) && !Dash) {
			this->m_movementComponent->GravityUpdate(_dt);
			if (this->m_movementComponent->GetVelocity().y > 0) {
				this->m_movementComponent->StopVelocityY();
			}
			Dash_Number = 1;

			if (Grab_Left)
				Left = false;
			if (Grab_Right)
				Right = false;

			if (!Jump) {
				Jump = true;
			}
		}
		else if (!Dash) {
			this->m_movementComponent->GravityUpdate(_dt);
			this->Jump = true;
		}
	}
	else if (!Life) {
		if (Get_NextPosDown(_collid, _dt)) {
			this->m_movementComponent->StopVelocity();
		}
		else {
			this->m_movementComponent->GravityUpdate(_dt);
		}
	}
	else if (SpeHori) {
		this->m_movementComponent->StopVelocity();
	}
}

void Player::Update(sf::Image& _collid, std::vector<BulletPlayer>* bullet, CSManager* mana, const float& _dt, sf::Font& font)
{
	if (Life) {
		if (func::IsDirectionPressed(sf::Keyboard::Key::D, func::PadAxis::JoystickGaucheHori, 30) && !this->Dash) {
			if ((WallJump && m_movementComponent->GetVelocity().y > 0) || !WallJump) {
				if ((run.getStatus() == sf::Sound::Status::Stopped || run.getStatus() == sf::Sound::Status::Paused) && !Jump)
					run.play();
				Right = true;
			}
		}
		else if (func::IsDirectionPressed(sf::Keyboard::Key::Q, func::PadAxis::JoystickGaucheHori, -30) && !this->Dash && !WallJump) {
			if ((WallJump && m_movementComponent->GetVelocity().y > 0) || !WallJump) {
				if ((run.getStatus() == sf::Sound::Status::Stopped || run.getStatus() == sf::Sound::Paused) && !Jump)
					run.play();
				Left = true;
			}
		}
		else if (!this->Dash && !WallJump) {
			this->m_movementComponent->StopVelocityX();
			if (run.getStatus() == sf::Sound::Status::Playing)
				run.stop();
		}

		Grab_Right = false;
		Grab_Left = false;

		Attack(_collid, bullet, mana, _dt);
		this->Move_Dash(_collid, _dt);
		this->Move_Jump(_collid, _dt);
	}

	AffectGravity(_collid, _dt);
	updateAnim(_collid, _dt);

	if (Left) {
		if (this->Direction == Dir::Right)
			this->m_movementComponent->StopVelocityX();

		this->m_movementComponent->Move(-Speed_run, 0, _dt);

		if (Get_NextPosLeft(_collid, _dt))
			this->m_movementComponent->StopVelocityX();
		else
			this->Direction = Dir::Left;
		Left = false;
	}
	if (Right) {
		if (this->Direction == Dir::Left)
			this->m_movementComponent->StopVelocityX();

		this->m_movementComponent->Move(Speed_run, 0, _dt);

		if (Get_NextPosRight(_collid, _dt))
			this->m_movementComponent->StopVelocityX();
		else
			this->Direction = Dir::Right;
		Right = false;
	}

	if (Get_NextPosUp(_collid, _dt)) {
		if (m_movementComponent->GetVelocity().y < 0) {
			m_movementComponent->StopVelocityY();
		}
	}
	this->Move(_dt);
	this->m_hitboxComponent->UpdateL();

	if (Life) {
		static float timerGold{ 0.f };
		timerGold += _dt;
		if (timerGold > 1 && gold > 0) {
			gold -= 1;
			timerGold = 0.f;
		}
		if (gold < 1)
			gold = 0;
		gold_t = sf::Text(std::to_string(gold), font, 40);
		gold_t.setPosition({ 1350,8 });
	}
	else {
		gold = 0;
		gold_t = sf::Text(std::to_string(gold), font, 40);
		gold_t.setPosition({ 1350,8 });
	}
}

void Player::updateAnim(sf::Image& _collid, const float& dt)
{
	if (!Life){
		timer_mort += dt;
		anim["Mort"].Animation_NoRestartL(m_shape, dt);
	}
	else if (FightTop) {
			m_shapeAttack.setPosition(GetPos().x, GetPos().y);
			if (Direction == Dir::Right) {
				anim["Beat_up"].Animation_NoRestartL(m_shapeAttack, dt);
				m_shapeAttack.setScale(sf::Vector2f(1, 1));
			}
			else if (Direction == Dir::Left) {
				anim["Beat_up"].Animation_NoRestartL(m_shapeAttack, dt);
				m_shapeAttack.setScale(sf::Vector2f(-1, 1));
			}
		}
	else if (FightDown) {
			m_shapeAttack.setPosition(GetPos().x, GetPos().y + 30);
			if (Direction == Dir::Right) {
				anim["Beat_down"].Animation_NoRestartL(m_shapeAttack, dt);
				m_shapeAttack.setScale(sf::Vector2f(1, 1));
			}
			else if (Direction == Dir::Left) {
				anim["Beat_down"].Animation_NoRestartL(m_shapeAttack, dt);
				m_shapeAttack.setScale(sf::Vector2f(-1, 1));
			}
		}
	else if (FightHori) {
			m_shapeAttack.setPosition(GetPos().x, GetPos().y + 5);
			if (Direction == Dir::Right) {
				anim["Beat_right"].Animation_NoRestartL(m_shapeAttack, dt);
				m_shapeAttack.setScale(sf::Vector2f(1, 1));
			}
			else if (Direction == Dir::Left) {
				anim["Beat_right"].Animation_NoRestartL(m_shapeAttack, dt);
				m_shapeAttack.setScale(sf::Vector2f(-1, 1));
			}
		}
	else if (SpeHori) {
			if (Direction == Dir::Right) {
				anim["Attack_spe"].Animation_NoRestartL(m_shape, dt);
				m_shape.setScale(sf::Vector2f(1, 1));
			}
			else if (Direction == Dir::Left) {
				anim["Attack_spe"].Animation_NoRestartL(m_shape, dt);
				m_shape.setScale(sf::Vector2f(-1, 1));
			}
		}
	else if (this->Dash == true) {
			if (Direction == Dir::Right) {
				anim["Dash"].Animation_NoRestartL(m_shape, dt);
				m_shape.setScale(sf::Vector2f(1, 1));
			}
			else if (Direction == Dir::Left) {
				anim["Dash"].Animation_NoRestartL(m_shape, dt);
				m_shape.setScale(sf::Vector2f(-1, 1));
			}
		}
	else {
			if (anim["Dash"].Get_End())
				anim["Dash"].Restart_Anim();

			if (Grab_Left && !Get_NextPosDown(_collid, dt)) {
				anim["Grab"].AnimationL(m_shape, dt);
				m_shape.setScale(sf::Vector2f(1, 1));
			}
			else if (Grab_Right && !Get_NextPosDown(_collid, dt)) {
				anim["Grab"].AnimationL(m_shape, dt);
				m_shape.setScale(sf::Vector2f(-1, 1));
			}
			else {
				grab.stop();
				if (m_movementComponent->isIdle()) {
					if (Direction == Dir::Right) {
						anim["idle"].AnimationL(m_shape, dt);
						m_shape.setScale(sf::Vector2f(1, 1));
					}
					else if (Direction == Dir::Left) {
						anim["idle"].AnimationL(m_shape, dt);
						m_shape.setScale(sf::Vector2f(-1, 1));
					}
				}
				else {
					if (Jump == true) {
						if (Direction == Dir::Right) {
							if (m_movementComponent->GetVelocity().y > 0) {
								anim["Jump_top"].Restart_Anim();
								anim["Jump_down"].Animation_NoRestartL(m_shape, dt);
							}
							else {
								anim["Jump_down"].Restart_Anim();
								anim["Jump_top"].Animation_NoRestartL(m_shape, dt);
							}

							m_shape.setScale(sf::Vector2f(1, 1));
						}
						else if (Direction == Dir::Left) {
							if (m_movementComponent->GetVelocity().y > 0) {
								anim["Jump_top"].Restart_Anim();
								anim["Jump_down"].Animation_NoRestartL(m_shape, dt);
							}
							else {
								anim["Jump_down"].Restart_Anim();
								anim["Jump_top"].Animation_NoRestartL(m_shape, dt);
							}
							m_shape.setScale(sf::Vector2f(-1, 1));
						}
					}
					else {
						if (Direction == Dir::Right) {
							anim["Walk_right"].AnimationL(m_shape, dt);
							m_shape.setScale(sf::Vector2f(1, 1));
						}
						else if (Direction == Dir::Left) {
							anim["Walk_right"].AnimationL(m_shape, dt);
							m_shape.setScale(sf::Vector2f(-1, 1));
						}
					}
				}
			}
		}
}

bool Player::Check_Colid(sf::Image& _collid, sf::Vector2f _pos)
{
	if (sf::FloatRect(0, 0, _collid.getSize().x, _collid.getSize().y).contains(_pos)) {
		if (_collid.getPixel(_pos.x, _pos.y) == sf::Color::Red) {
			pique_s.play();
			Life = false;
		}
		return _collid.getPixel(_pos.x, _pos.y) == sf::Color::Black;
	}
	return true;
}

bool Player::Get_NextPosLeft(sf::Image& _collid, const float& _dt)
{
	sf::Vector2f nextpos = m_hitboxComponent->GetPosition();
	nextpos.x -= m_hitboxComponent->GetGlobalBounds().width / 2;
	nextpos.y -= m_hitboxComponent->GetGlobalBounds().height / 2;

	bool find = false;
	for (int i = 5; i < (int)m_hitboxComponent->GetGlobalBounds().height - 5; i++) {
		for (float y = 0; y < -m_movementComponent->GetVelocity().x * _dt + 0.1; y += 0.1) {
			if (Check_Colid(_collid, sf::Vector2f(nextpos.x - y, nextpos.y + i))) {
				if (i > ((int)m_hitboxComponent->GetGlobalBounds().height / 5) &&
					i < ((int)m_hitboxComponent->GetGlobalBounds().height / 5) * 2) {
					run.stop();
					Grab_Left = true;
					WallJump = false;
				}
				if (!find) {
					m_position = sf::Vector2f((nextpos.x - y) + m_hitboxComponent->GetGlobalBounds().width / 2, m_position.y);
				}
				find = true;
			}
		}
	}

	return find;
}

bool Player::Get_NextPosRight(sf::Image& _collid, const float& _dt)
{
	sf::Vector2f nextpos = m_hitboxComponent->GetPosition();
	nextpos.x += m_hitboxComponent->GetGlobalBounds().width / 2;
	nextpos.y -= m_hitboxComponent->GetGlobalBounds().height / 2;

	bool find = false;
	for (int i = 5; i < (int)m_hitboxComponent->GetGlobalBounds().height - 5; i++) {
		for (float y = 0; y < m_movementComponent->GetVelocity().x * _dt + 0.1; y += 0.1) {
			if (Check_Colid(_collid, sf::Vector2f(nextpos.x + y, nextpos.y + i))) {
				if (i > ((int)m_hitboxComponent->GetGlobalBounds().height / 5) &&
					i < ((int)m_hitboxComponent->GetGlobalBounds().height / 5) * 2) {
					run.stop();
					Grab_Right = true;
					WallJump = false;
				}
				if (!find) {
					m_position = sf::Vector2f((nextpos.x + y) - m_hitboxComponent->GetGlobalBounds().width / 2, m_position.y);
				}
				find = true;
			}
		}
	}

	return find;
}

bool Player::Get_NextPosUp(sf::Image& _collid, const float& _dt)
{
	sf::Vector2f nextpos = m_hitboxComponent->GetPosition();
	nextpos.y -= m_hitboxComponent->GetGlobalBounds().height / 2;
	nextpos.x -= m_hitboxComponent->GetGlobalBounds().width / 2;

	for (int i = 1; i < (int)m_hitboxComponent->GetGlobalBounds().width; i++) {
		for (float y = 0; y < -m_movementComponent->GetVelocity().y * _dt + 0.1; y += 0.1) {
			if (Check_Colid(_collid, sf::Vector2f(nextpos.x + i, nextpos.y - y))) {
				m_position = sf::Vector2f(m_position.x, (nextpos.y - y) + 64);
				return true;
			}
		}
	}

	return false;

}

bool Player::Get_NextPosDown(sf::Image& _collid, const float& _dt)
{
	sf::Vector2f nextpos = m_hitboxComponent->GetPosition();
	nextpos.y += m_hitboxComponent->GetGlobalBounds().height / 2;
	nextpos.x -= m_hitboxComponent->GetGlobalBounds().width / 2;

	for (int i = 1; i < (int)m_hitboxComponent->GetGlobalBounds().width; i++) {
		for (float y = 0; y < m_movementComponent->GetVelocity().y * _dt + 0.1; y += 0.1) {
			if (Check_Colid(_collid, sf::Vector2f(nextpos.x + i, nextpos.y + y))) {
				if (Check_Colid(_collid, sf::Vector2f(nextpos.x + i, nextpos.y - 0.1f))) {
					m_position = sf::Vector2f(m_position.x, (nextpos.y - 0.3f) - 64);
				}
				else {
					if (Life)
						m_position = sf::Vector2f(m_position.x, (nextpos.y + y) - 64);
				}
				return true;
			}
		}
	}

	return false;
}

void Player::drawTextGold(sf::RenderWindow& window)
{
	window.draw(gold_t);
}

void Player::Render(sf::RenderWindow* _target, std::vector<BulletPlayer>* bullet)
{
	if (!FightHori && !FightDown && !FightTop) {
		_target->draw(m_shape);
	}
	else {
		_target->draw(m_shapeAttack);
	}

	if (m_hitboxComponent)
		m_hitboxComponent->draw(_target);

	for (auto& b : *bullet) {
		b.draw(_target);
	}
}