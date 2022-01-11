#ifndef OPTIONSTATE_H
#define OPTIONSTATE_H

#include "FSMManager.h"
#include "Button.h"
#include "Player.h"

class OptionState final : public FSM
{
	sf::Sprite m_sprite;
	sf::Sprite m_volume;
	sf::Text Vsync{ "V-SYNC ENABLED",*font,40 }, VsyncDisabled{ "V-SYNC DISABLED",*font,40 }, Fullscreen{ "FULLSCREEN",*font,40 }, FULLSCREENw{ "FULLSCREEN WINDOWED",*font,40 };
	Button retour;
	bool selectedMode{ false };
	bool selectedVSYNC{ false };
	sf::Vector2f p1{ 415,270 };
public:
	OptionState(FSMManager& _fsm, sf::RenderWindow& _window, sf::Font& _font, const bool change);
	~OptionState();

	void GoBackPreviousState() override;
	void processEvents(const sf::Event & e, const float& dt) override;
	void update(const float& dt)override;
	void draw(sf::RenderWindow & window)override;

	inline static float volume{ 20.f };
};
#endif
