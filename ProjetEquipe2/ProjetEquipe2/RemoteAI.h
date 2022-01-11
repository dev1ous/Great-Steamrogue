#ifndef REMOTEAI_H
#define REMOTEAI_H

#include "AI.h"
#include "Bullet.h"
#include "Data.h"
#include "Canon.h"
#include <complex>

class RemoteAI: public AI
{
	float timeBeetwenShoot;
	sf::CircleShape playerDetection;
	std::vector<sf::RectangleShape> patrol;
	int goTo;
	bool isPatroling;

	std::vector<std::shared_ptr<Bullet>> bullets;
	std::unique_ptr<Canon> canon;
	
	void InitPatrol();
	void InitPlayerDetection();
	virtual void InitAnim()override;
	void InitCanon(sf::Vector2f _pos);

public:
	RemoteAI(float _posX, float _posY, Player& _player);
	~RemoteAI() = default;
	
	using Movable::Move;
	virtual void Move(sf::Image& _collid, const float& _dt);

	void Shoot(const float& _dt);
	void deleteBullets(sf::Image& _collid, const float& _dt);

	void Patrol(sf::Image& _collid, const float& _dt);
	virtual void FollowPlayer(sf::Image& _collid, const float& _dt) override;
	virtual void UpdateDetection();
	virtual void UpdatePatrol();
	void UpdateCanon(const float& _dt);
	virtual void UpdateAnim(const float& _dt) override;

	virtual void Update(sf::Image& _collid, Shield*, std::vector<BulletPlayer>*, const float& _dt)override;

	virtual void Render(sf::RenderWindow* _target) override;
};
#endif