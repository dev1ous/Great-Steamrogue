#ifndef MONTREHUD_H
#define MONTREHUD_H

#include "SFML/Graphics.hpp"
#include "Player.h"

class montreHUD
{
public:
	montreHUD(const std::string&, sf::Vector2f, sf::Vector2f);
	~montreHUD() = default;
	sf::Sprite& getMontreHud() { return montre_hud; }
private:
	sf::Sprite montre_hud;
	sf::Vector2f m_pos_hud;
	sf::Vector2f m_origin_hud;
};
#endif
