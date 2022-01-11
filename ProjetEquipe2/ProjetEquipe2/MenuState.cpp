#include "MenuState.h"
#include <iostream>
#include "SFML/Window/Event.hpp"
#include "GameState.h"
#include "EditorState.h"
#include "OptionState.h"
#include "Functions.h"
#include "CreditsState.h"

MenuState::MenuState(FSMManager& _fsm, sf::RenderWindow& _window, sf::Font& _font, const  bool change)
	:FSM{ _fsm,_window,_font,change }, buttonMenu{ nullptr, sf::RectangleShape({531,147}) }
{
	buffer.loadFromFile("../Ressources/Sound/son/son_eqout.wav");
	m_music.setBuffer(buffer);
	m_music.setVolume(5);
	m_music.setLoop(true);
	m_music.play();


	fond.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("Afond"));
	fond.setPosition({ 0,0 });
	buttonMenu.setPosition({ 900,430 });
	buttonMenu.setOrigin();
	buttonMenu.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("JouerSelec")).setTextureState("JouerSelec", "Jouer", "Jouer");
	buttonMenu.CreateFunctionCall([this]() {


		if (threading.joinable())
			threading.join();
		threading = std::thread([this]
			{
				Check_Mutex.lock();
				ptr = FSMManager::create<GameState>(*fsmManager, *window, *font, false);
				Check_Mutex.unlock();
			});
		});
	buttonMenu.CenterText();

	//childs
	[[maybe_unused]] Button& Editor = buttonMenu.Add("editor"_hash, func::rectangle({ 900,530 }, { 531, 147 }, sf::Color::White, sf::Color::White, 0))
		.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("EditerSelec")).setPosition({ 898,560 }).setOrigin().setTextureState("EditerSelec", "Editer", "Editer");

	Editor.CreateFunctionCall([this]() {

		if (threading.joinable())
			threading.join();

		threading = std::thread([this]
			{
				Check_Mutex.lock();
				ptr = FSMManager::create<EditorState>(*fsmManager, *window, *font, false);
				Check_Mutex.unlock();
			});
		});

	[[maybe_unused]] Button& Options = buttonMenu.Add("options"_hash, func::rectangle({ 900,692 }, { 531,147 }, sf::Color::White, sf::Color::White, 0));
	Options.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("OptionsSelec")).setOrigin().setTextureState("OptionsSelec", "Options", "Options");
	Options.CreateFunctionCall([this]() {

		if (threading.joinable())
			threading.join();

		threading = std::thread([this]
			{
				Check_Mutex.lock();
				ptr = FSMManager::create<OptionState>(*fsmManager, *window, *font, false);
				Check_Mutex.unlock();
				m_music.play();
			});
		});

	[[maybe_unused]] Button& Credits = buttonMenu.Add("credits"_hash, func::rectangle({ 900,830 }, { 531,147 }, sf::Color::White, sf::Color::White, 0));
	Credits.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("CreditsSelec")).setOrigin().setTextureState("CreditsSelec", "Credits", "Credits");
	Credits.CreateFunctionCall([this]() {

		if (threading.joinable())
			threading.join();

		threading = std::thread([this]
			{
				Check_Mutex.lock();
				ptr = FSMManager::create<CreditsState>(*fsmManager, *window, *font, false);
				Check_Mutex.unlock();
			});
		});

	[[maybe_unused]] Button& Quit = buttonMenu.Add("quit"_hash, func::rectangle({ 892,970 }, { 531, 147 }, sf::Color::White, sf::Color::White, 0));
	Quit.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("QuitterSelec")).setOrigin().setTextureState("QuitterSelec", "Quitter", "Quitter");

	Quit.CreateFunctionCall([&_window, this]() {
		m_music.stop();

		if (threading.joinable())
			threading.join();

		_window.close();
		});
}

MenuState::~MenuState()
{
	if (threading.joinable())
		threading.join();
}

void MenuState::GoBackPreviousState()
{
	std::cout << "vous avez rien a mettre dans celui la" << std::endl;
}

void MenuState::processEvents(const sf::Event& e, const float& dt)
{

	if (!Check_Mutex)
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
				window->close();
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		buttonMenu.processEvents(e);
	}
}

void MenuState::update(const float& dt)
{
	if (Check_Mutex)
	{
		m_loadingScreen.at("g1"_hash).rotate(200 * dt);
		m_loadingScreen.at("g2"_hash).rotate(-150 * dt);
		m_loadingScreen.at("g3"_hash).rotate(110 * dt);
		m_loadingScreen.at("g4"_hash).rotate(60 * dt);
		timerCursor += dt;
	}
	else
	{
	}
	if (sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheHori)) < -30 ||
		sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheHori)) > 30 ||
		sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheVert)) < -30 ||
		sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheVert)) > 30)
		sf::Mouse::setPosition(sf::Vector2i(sf::Mouse::getPosition().x + sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheHori)) * 5 * dt,
			sf::Mouse::getPosition().y + sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheVert)) * 5 * dt));

}

void MenuState::draw(sf::RenderWindow& window)
{
	if (Check_Mutex) {
		window.draw(m_fond_d);
		func::drawUnorderMap(m_loadingScreen, &window);
		window.draw(m_fond);
		window.draw(text);

		if (timerCursor >= 0.5f)
			window.draw(std::get<0>(m_points));
		if (timerCursor >= 1.f)
			window.draw(std::get<1>(m_points));
		if (timerCursor >= 1.5f)
			window.draw(std::get<2>(m_points));
		if (timerCursor >= 2.f)
			timerCursor = 0.f;
	}
	else
	{
		window.draw(fond);
		buttonMenu.draw(window);
	}
}
