#pragma once
#include "SFML/Graphics.hpp"
#include "CacheSystem.h"

class Animation
{
private:
	sf::IntRect Rect;
	int NbFrame;
	int Frame;
	int Space;
	float Frame_Timer;
	float Timer;

	sf::Clock mainTime;

public:
	Animation() = default;
	Animation(sf::IntRect _rect, int _nbframe, float _frametimer, const int _space = 1);
	~Animation() = default;

	inline bool operator == (const Animation& a) const { return Rect == a.Rect; };
	inline bool operator != (const Animation& a) const { return !operator==(a); };

	void Animator(sf::RectangleShape & _sprite, const float& _dt);
//public:
//	std::string& filepath;
//	sf::RectangleShape& shape;
//	sf::IntRect startRect;
//	sf::IntRect currentRect;
//	sf::IntRect endRect;
//
//	int width;
//	int height;
//
//	float animationTimer;
//	float timer;
//
//public:
//	Animation(sf::RectangleShape& _shape, std::string& _filepath, float _animation_timer,
//		int _start_frames_x, int _start_frames_y, int _frames_x, int _frames_y, int _width, int _height)
//		:shape(_shape), animationTimer(_animation_timer), width(_width), height(_height), filepath(_filepath)
//	{
//		this->timer = 0;
//		this->startRect = sf::IntRect(_start_frames_x * width, _start_frames_y * height, width, height);
//		this->currentRect = this->startRect;
//		this->endRect = sf::IntRect(_frames_x * width, _frames_y * height, width, height);
//
//		this->shape.setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>(filepath));
//		this->shape.setTextureRect(this->startRect);
//	}
//	~Animation() = default;
//
//	void Update(const float& _dt)
//	{
//		//Update timer
//		this->timer += _dt;
//
//		if (this->timer >= this->animationTimer)
//		{
//			//reset timer
//			this->timer = 0.f;
//
//			//Animate
//			if (this->currentRect != this->endRect)
//				this->currentRect.left += this->width;
//			//Reset
//			else
//				this->currentRect.left = this->startRect.left;
//		}
//		this->shape.setTextureRect(this->currentRect);
//	}
//
//	void Reset()
//	{
//		this->timer = 0.f;
//		this->currentRect = this->startRect;
//	}
};