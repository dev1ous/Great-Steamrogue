#pragma once
#include "Entity.h"

class Movable :
	public Entity
{
public:
	Movable() = default;
	~Movable() { }

	virtual void Move(const float _dt);
};

