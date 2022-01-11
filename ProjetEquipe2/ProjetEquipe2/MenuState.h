#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "FSMManager.h"
#include "Button.h"
#include "SFML/Audio.hpp"

class FSMManager;

class MenuState final : public FSM
{
	Button buttonMenu;
	sf::Sprite fond;
	sf::RectangleShape shapeMenu{ {200,50} };
	[[maybe_unused]] unsigned size{ 50 };
public:
	MenuState(FSMManager& _fsm, sf::RenderWindow& _window, sf::Font& _font, const bool change);
	~MenuState();

	void GoBackPreviousState() override;
	void processEvents(const sf::Event& e, const float& dt) override;
	void update(const float& dt)override;
	void draw(sf::RenderWindow& window)override;
};
#endif

