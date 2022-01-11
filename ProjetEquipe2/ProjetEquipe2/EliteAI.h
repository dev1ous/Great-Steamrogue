#pragma once
#include "AI.h"
#include "Shield.h"

class Shield;

class EliteAI : public AI
{
	sf::RectangleShape fx;
	sf::RectangleShape fx_2;

	bool detectPlayer;
	float goToPlayer;

	bool attack{ true };

	void InitAreaOfDetection();
	virtual void InitAnim()override;
	void InitFx();
	void InitFx_2();

public:
	EliteAI(float _posX, float _posY, Player& _player);
	~EliteAI() = default;

	virtual void FollowPlayer(sf::Image& _collid, const float& _dt) override;
	virtual void UpdateDetection();

	using Entity::Update;
	virtual void Update(sf::Image& _collid, Shield*,std::vector<BulletPlayer>*, const float& _dt)override;

	virtual void UpdateAnim(const float& _dt) override;
	void UpdateFx();
	void UpdateFx_2();

	using Entity::Render;
	virtual void Render(sf::RenderWindow* _target) override;
};