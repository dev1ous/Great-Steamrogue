#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "FSMManager.h"
#include "Game.h"

class FSMManager;

class GameState final : public FSM
{
protected:
	Game ga;
	Button Select_Level;
	bool InGame{ false };
	std::string CurrentMap;
	sf::Sprite fond;

public:
	GameState(FSMManager& _fsm, sf::RenderWindow& _window, sf::Font& _font, const bool change);
	~GameState();

	void GoBackPreviousState() override;
	void processEvents(const sf::Event& e, const float& dt) override;
	void update(const float& dt)override;
	void draw(sf::RenderWindow& window)override;

	std::array<sf::Sound, 3> m_sound;
	std::array<sf::SoundBuffer, 3>m_buffer;
};
#endif 