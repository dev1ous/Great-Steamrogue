#ifndef BOSS_H
#define BOSS_H

#include "AI.h"
#include "Bullet.h"
#include "Canon.h"
#include "Data.h"

#include <complex>

class Boss : public AI
{
private:

	sf::Sound boss_sound;
	sf::SoundBuffer boss_buffer;

	sf::RectangleShape playerDetection;
	sf::RectangleShape arm;
	sf::RectangleShape weapon;

	std::vector<std::shared_ptr<Bullet>> bullets;
	std::unique_ptr<Canon> canon;

	float timeBeetwenShoot;
	float attackTimer;
	bool gonnaExplode;
	bool attack;
	[[maybe_unused]] bool onePass;
	bool isShooting;

	bool moveRight;
	bool isPatrolling;

	void InitPlayerDetection();
	virtual void InitAnim() override;
	void InitArm();
	void InitWeapon();

public:
	Boss(float _posX, float _posY, Player& _player);
	~Boss() = default;

	virtual void FollowPlayer(sf::Image& _collid, const float& _dt) override;
	void Patrol(sf::Image& _collid, const float& _dt);
	void Shoot(const float& _dt);
	void deleteBullets(sf::Image& _collid);

	using AI::Update;
	virtual void Update(sf::Image& _collid, Shield*,std::vector<BulletPlayer>*, const float& _dt) override;
	virtual void UpdateAnim(const float& _dt) override;
	virtual void UpdateDetection();
	void UpdateArm();
	void UpdateWeapon(const float& _dt);
	void UpdateBullets(const float& _dt);

	using Entity::Render;
	virtual void Render(sf::RenderWindow* _target) override;
};
#endif