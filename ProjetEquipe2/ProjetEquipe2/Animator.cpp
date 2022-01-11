#include "Animator.h"

Animator::Animator(sf::IntRect _rect, int _nbframe, float _frametimer, const int _space)
{
	Rect = _rect;
	NbFrame = _nbframe;
	Frame_Timer = _frametimer;
	Space = _space;
	Frame = 0;
	Timer = 0;
}

void Animator::Animation(sf::RectangleShape& _sprite, const float& dt)
{
	Timer += dt;

	if (Timer > Frame_Timer)
	{
		Frame++;

		if (Frame >= NbFrame)
			Frame = 0;

		Timer = 0;
	}

	_sprite.setSize(sf::Vector2f(Rect.width, Rect.height));
	_sprite.setTextureRect(sf::IntRect(Rect.left + (Frame * Rect.width) + (Frame * Space), Rect.top, Rect.width, Rect.height));
	_sprite.setOrigin(Rect.width / 2.f, Rect.height / 2.f);
}

void Animator::AnimationL(sf::RectangleShape& _sprite, const float& dt)
{
	Timer += dt;

	if (Timer > Frame_Timer)
	{
		Frame++;

		if (Frame >= NbFrame)
			Frame = 0;

		Timer = 0;
	}

	_sprite.setSize(sf::Vector2f(Rect.width, Rect.height));
	_sprite.setTextureRect(sf::IntRect(Rect.left + (Frame * Rect.width) + (Frame * Space), Rect.top, Rect.width, Rect.height));
	_sprite.setOrigin(Rect.width / 2.f, Rect.height - 64);
}

void Animator::AnimationC(sf::CircleShape& _sprite, const float& dt)
{
	Timer += dt;

	if (Timer > Frame_Timer)
	{
		Frame++;

		if (Frame >= NbFrame)
			Frame = 0;

		Timer = 0;
	}

	_sprite.setRadius(Rect.width / 2.f);
	_sprite.setTextureRect(sf::IntRect(Rect.left + (Frame * Rect.width) + (Frame * Space), Rect.top, Rect.width, Rect.height));
	_sprite.setOrigin(Rect.width / 2.f, Rect.height / 2.f);
}

void Animator::Animation_NoRestart(sf::RectangleShape& _sprite, const float& dt)
{
	Timer += dt;

	if (Timer > Frame_Timer) {
		Frame++;

		if (Frame >= NbFrame)
			Frame = NbFrame - 1;

		Timer = 0;
	}

	_sprite.setSize(sf::Vector2f(Rect.width, Rect.height));
	_sprite.setTextureRect(sf::IntRect(Rect.left + (Frame * Rect.width) + (Frame * Space), Rect.top, Rect.width, Rect.height));
	_sprite.setOrigin(Rect.width / 2.f, Rect.height / 2.f);
}

void Animator::Animation_NoRestartL(sf::RectangleShape& _sprite, const float& dt)
{
	Timer += dt;

	if (Timer > Frame_Timer) {
		Frame++;

		if (Frame >= NbFrame)
			Frame = NbFrame - 1;

		Timer = 0;
	}

	_sprite.setSize(sf::Vector2f(Rect.width, Rect.height));
	_sprite.setTextureRect(sf::IntRect(Rect.left + (Frame * Rect.width) + (Frame * Space), Rect.top, Rect.width, Rect.height));
	_sprite.setOrigin(Rect.width / 2.f, Rect.height - 64);
}

void Animator::Animation_NoRestartC(sf::CircleShape& _sprite, const float& dt)
{
	Timer += dt;

	if (Timer > Frame_Timer) {
		Frame++;

		if (Frame >= NbFrame)
			Frame = NbFrame - 1;

		Timer = 0;
	}

	_sprite.setRadius(Rect.width / 2.f);
	_sprite.setTextureRect(sf::IntRect(Rect.left + (Frame * Rect.width) + (Frame * Space), Rect.top, Rect.width, Rect.height));
	_sprite.setOrigin(Rect.width / 2.f, Rect.height / 2.f);
}