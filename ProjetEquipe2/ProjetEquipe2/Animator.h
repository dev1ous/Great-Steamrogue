#pragma once
#include "SFML/Graphics.hpp"

class Animator
{
	sf::IntRect Rect;
	int NbFrame;
	int Frame;
	int Space;
	float Frame_Timer;
	float Timer;

public:
	Animator() = default;
	Animator(sf::IntRect _rect, int _nbframe, float _frametimer, const int _space = 1);
	~Animator() = default;

	inline bool operator == (const Animator& a) const { return Rect == a.Rect; };
	inline bool operator != (const Animator& a) const { return !operator==(a); };

	void Animation(sf::RectangleShape& _sprite,const float& dt);
	void AnimationL(sf::RectangleShape& _sprite, const float& dt);
	void AnimationC(sf::CircleShape& _sprite,const float& dt);
	void Animation_NoRestart(sf::RectangleShape& _sprite, const float& dt);
	void Animation_NoRestartL(sf::RectangleShape& _sprite, const float& dt);
	void Animation_NoRestartC(sf::CircleShape& _sprite, const float& dt);
	inline void Restart_Anim() { Frame = 0; }
	inline bool Get_End() { return Frame == NbFrame - 1; }
	inline bool getShot() { return Frame == NbFrame - 3; }//Reserve pour l'attack speciale du joueur
};

