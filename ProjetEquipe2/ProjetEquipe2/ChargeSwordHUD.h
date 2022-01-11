#ifndef CHARGESWORDHUD_H
#define CHARGESWORDHUD_H

#include "SFML/Graphics.hpp"

class ChargeSwordHUD
{
public:
	ChargeSwordHUD() = default;
	ChargeSwordHUD(const std::string&, sf::Vector2f, sf::Vector2f);
	~ChargeSwordHUD() = default;

	int getRectTop() { return m_rectCharge.top; }
	int getRectLeft() { return m_rectCharge.left; }
	void setRect() { m_spriteCharge.setTextureRect(m_rectCharge); }
	void setRectTop(int rectTop) { m_rectCharge.top = rectTop; }
	void setRectLeft(int rectLeft) { m_rectCharge.left = rectLeft; }

	void update(const float&);
	void draw(sf::RenderWindow*);
private:
	sf::IntRect m_rectCharge;
	sf::Sprite m_spriteCharge;
	float timerCharge{ 0.f };
};
#endif

