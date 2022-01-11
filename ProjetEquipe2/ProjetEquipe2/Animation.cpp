#include "Animation.h"

Animation::Animation(sf::IntRect _rect, int _nbframe, float _frametimer, const int _space)
{
	Rect = _rect;
	NbFrame = _nbframe;
	Frame_Timer = _frametimer;
	Space = _space;
	Frame = 0;
	Timer = 0;
}

void Animation::Animator(sf::RectangleShape& _sprite, const float& _dt)
{
	Timer += _dt;

	if (Timer > Frame_Timer)
	{
		Frame++;

		if (Frame >= NbFrame)
			Frame = 0;

		Timer = 0;
	}

	_sprite.setTextureRect(sf::IntRect(Rect.left + (Frame * Rect.width) + (Frame * Space), Rect.top, Rect.width, Rect.height));
	_sprite.setOrigin(Rect.width / 2, Rect.height / 2);
}
