#include "CreditsState.h"
#include "Functions.h"
#include "CacheSystem.h"

CreditsState::CreditsState(FSMManager& _fsm, sf::RenderWindow& _window, sf::Font& _font, const bool change)
	:FSM{ _fsm,_window,_font,change }
{
	std::get<0>(names) = func::CreateText("PROGRAMMEURS :", *font, 30, { 500,400 }, sf::Color::Black);
	std::get<0>(names).setStyle(sf::Text::Bold | sf::Text::Underlined);
	std::get<1>(names) = func::CreateText("GRAPHISTES :", *font, 30, { 1200,400 }, sf::Color::Black);
	std::get<1>(names).setStyle(sf::Text::Bold | sf::Text::Underlined);
	std::get<2>(names) = func::CreateText("CANONGE Laurent", *font, 30, { 500,550 }, sf::Color::Black);
	std::get<2>(names).setStyle(sf::Text::Italic);
	std::get<3>(names) = func::CreateText("RENEVIER Jeremy", *font, 30, { 500,650 }, sf::Color::Black);
	std::get<3>(names).setStyle(sf::Text::Italic);
	std::get<4>(names) = func::CreateText("FALTRAUER Gregoire (le genie)", *font, 30, { 500,750 }, sf::Color::Black);
	std::get<4>(names).setStyle(sf::Text::Italic);
	std::get<5>(names) = func::CreateText("LUPOLLO Renzo", *font, 30, { 1200,550 }, sf::Color::Black);
	std::get<5>(names).setStyle(sf::Text::Italic);
	std::get<6>(names) = func::CreateText("TIXIDOR Gabriel", *font, 30, { 1200,650 }, sf::Color::Black);
	std::get<6>(names).setStyle(sf::Text::Italic);
	std::get<7>(names) = func::CreateText("RETSEN Lucas", *font, 30, { 1200,750 }, sf::Color::Black);
	std::get<7>(names).setStyle(sf::Text::Italic);
	std::get<8>(names) = func::CreateText("VALLET Mattheo", *font, 30, { 1200,850 }, sf::Color::Black);
	std::get<8>(names).setStyle(sf::Text::Italic);
}

CreditsState::~CreditsState()
{
	if (threading.joinable())
		threading.join();
}

void CreditsState::GoBackPreviousState()
{
}

void CreditsState::processEvents(const sf::Event& e, const float& dt)
{
	switch (e.type) {
	case sf::Event::Closed:
		window->close();
		break;
	case sf::Event::KeyPressed:
		switch (e.key.code) {
		case sf::Keyboard::Key::Escape:
			if (threading.joinable())
				threading.join();
			fsmManager->popState();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void CreditsState::update(const float& dt)
{
}

void CreditsState::draw(sf::RenderWindow& window)
{
	window.draw(*func::CreateSprite({ 0,0 }, gm::CacheSystem::AddAnyRessources<sf::Texture>("Afond")));
	func::drawSpan(std::span(names.data(), names.size()), window);
}
