#ifndef CREDITSSTATE_H
#define CREDITSSTATE_H

#include "FSMManager.h"
#include "SFML/Graphics.hpp"

class FSMManager;

class CreditsState final :public FSM
{
	std::array<sf::Text, 9> names;
public:
	CreditsState(FSMManager& _fsm, sf::RenderWindow& _window, sf::Font& _font, const bool change);
	~CreditsState();

	void GoBackPreviousState() override;
	void processEvents(const sf::Event& e, const float& dt) override;
	void update(const float& dt)override;
	void draw(sf::RenderWindow& window)override;
};
#endif

