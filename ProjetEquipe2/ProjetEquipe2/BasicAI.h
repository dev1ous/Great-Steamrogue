#pragma once
#include "AI.h"
#include "Shield.h"

class Shield;

class BasicAI : public AI
{
	sf::CircleShape playerDetection;
	sf::RectangleShape fx;

	bool moveRight;
	bool isPatrolling;
	bool gonnaExplode;

	bool attack;
	float attackTimer;
	bool drawDetection;
	bool onePass;

	sf::Sound basicAI_sound;
	sf::SoundBuffer basicAI_buffer;

	void InitPlayerDetection();
	void InitFx();
	virtual void InitAnim() override;

public:
	BasicAI(float _posX, float _posY, Player& _player);
	~BasicAI() = default;

	virtual void FollowPlayer(sf::Image& _collid, const float& _dt) override;

	void Patrol(sf::Image& _collid, const float& _dt);

	virtual void UpdateDetection();
	virtual void UpdateFx();
	virtual void UpdateAnim(const float& _dt) override;

	using AI::Update;
	virtual void Update(sf::Image& _collid, Shield*, std::vector<BulletPlayer>*, const float& _dt) override;

	using Entity::Render;
	virtual void Render(sf::RenderWindow* _target) override;
};

