#ifndef SCORESTATE_H
#define SCORESTATE_H

#include "SFML/Graphics.hpp"
#include "FSMManager.h"
#include "Button.h"
#include "FileLogger.h"

class FSMManager;

class ScoreState final : public FSM
{
	sf::Sprite m_fondHigh;
	sf::Vector2f m_fondPos{ 0,0 };
	[[maybe_unused]] bool shake{ false };
	[[maybe_unused]] float timer_shake{ 0.f };
	sf::Text printHighscore;
	Button highscores_b;
	FileLogger highscoreTable;

public:
	enum class Return{Game,Editor};
	Return State;

	ScoreState(FSMManager& _fsm, sf::RenderWindow& _window, sf::Font& _font, Return _state, const bool change);
	~ScoreState();

	void GoBackPreviousState() override;
	void processEvents(const sf::Event& e, const float& dt) override;
	void update(const float& dt)override;
	void draw(sf::RenderWindow& window)override;
};
#endif
