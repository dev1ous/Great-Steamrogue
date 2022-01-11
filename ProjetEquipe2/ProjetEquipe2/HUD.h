#ifndef HUD_H
#define HUD_H

#include "SFML/Graphics.hpp"

class HUD
{
	sf::Sprite m_hud;
public:
	HUD();
	~HUD() = default;

	void update(const float&);
	void draw(sf::RenderWindow*);
};
#endif

