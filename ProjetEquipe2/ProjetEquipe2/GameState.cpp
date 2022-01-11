#include "GameState.h"
#include "SFML/Window/Event.hpp" 
#include <thread>
#include "ScoreState.h"
#include <fstream>
#include <utility>
#include "OptionState.h"

GameState::GameState(FSMManager& _fsm, sf::RenderWindow& _window, sf::Font& _font, const bool change) :FSM{ _fsm,_window,_font,change }, ga{ window, *font },
Select_Level{ nullptr, func::rectangle({ 1440,610 }, { 449,330 }, sf::Color::White, sf::Color::White, 0.f), func::CreateText("", *font, 40, {}, sf::Color::Black) }
{
	fond.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("Afond"));
	fond.setPosition({ 0,0 });
	/*std::get<0>(m_buffer).loadFromFile("../Ressources/Sound/cityRoof.wav");
	std::get<0>(m_sound).setBuffer(std::get<0>(m_buffer));
	std::get<0>(m_sound).setLoop(true);
	std::get<1>(m_buffer).loadFromFile("../Ressources/Sound/son/son_autre_niv.wav");
	std::get<1>(m_sound).setBuffer(std::get<1>(m_buffer));
	std::get<1>(m_sound).setLoop(true);
	std::get<2>(m_buffer).loadFromFile("../Ressources/Sound/train.wav");
	std::get<2>(m_sound).setBuffer(std::get<2>(m_buffer));
	std::get<2>(m_sound).setLoop(true);*/

	Select_Level.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("sous_terrain")).setTextureState("sous_terrain", "sous_terrain_S", "sous_terrain_S").setOrigin();
	Select_Level.CreateFunctionCall([&] {
		/*std::get<1>(m_sound).stop();
		std::get<2>(m_sound).stop();
		m_music.stop();*/
		m_music.setLoop(false);
		if (threading.joinable())
			threading.join();

		threading = std::thread([this]
			{
				Check_Mutex.lock();
				CurrentMap = "Niveau3";
				ga.Init(CurrentMap);
				InGame = true;
				Check_Mutex.unlock();
				/*std::get<2>(m_sound).play();*/
			});
		});

	Button& tab57 = Select_Level.Add("Ville"_hash, func::rectangle({ 480,610 }, { 449,330 }, sf::Color::White, sf::Color::White, 0.f)).setOrigin();
	tab57.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("ville")).setTextureState("ville", "ville_S", "ville_S");
	tab57.CreateFunctionCall([&] {
		/*std::get<0>(m_sound).stop();
		std::get<1>(m_sound).stop();
		m_music.stop();
		m_music.setLoop(false);*/

		if (threading.joinable())
			threading.join();

		threading = std::thread([this]
			{
				Check_Mutex.lock();
				CurrentMap = "Niveau1";
				ga.Init(CurrentMap);
				InGame = true;
				Check_Mutex.unlock();
				/*std::get<0>(m_sound).play();*/
			});
		});

	Button& tab58 = Select_Level.Add("Serre"_hash, func::rectangle({ 960,610 }, { 449,330 }, sf::Color::White, sf::Color::White, 0.f)).setOrigin();
	tab58.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("serre")).setTextureState("serre", "serre_S", "serre_S");
	tab58.CreateFunctionCall([&] {
	/*	std::get<2>(m_sound).stop();
		std::get<0>(m_sound).stop();
		m_music.stop();
		m_music.setLoop(false);*/

		if (threading.joinable())
			threading.join();

		threading = std::thread([this]
			{
				Check_Mutex.lock();
				CurrentMap = "Niveau2";
				ga.Init(CurrentMap);
				InGame = true;
				Check_Mutex.unlock();
			/*	std::get<1>(m_sound).play();*/
			});
		});

	[[maybe_unused]] Button& Quit = Select_Level.Add("quit"_hash, func::rectangle({ 960,970 }, { 531, 147 }, sf::Color::White, sf::Color::White, 0)).setOrigin();
	Quit.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("QuitterSelec")).setOrigin().setTextureState("QuitterSelec", "Quitter", "Quitter");

	Quit.CreateFunctionCall([this]() {

		if (threading.joinable())
			threading.join();

		fsmManager->popState();
		});
}

GameState::~GameState()
{
	if (threading.joinable())
		threading.join();
}

void GameState::GoBackPreviousState()
{
}

void GameState::processEvents(const sf::Event& e, const float& dt)
{
	if (!Check_Mutex)
	{
		switch (e.type)
		{
		case sf::Event::Closed:
			window->close();
			break;
		default:
			break;
		}

		if (InGame)
			ga.processEvents(e, dt);
		else {
			Select_Level.processEvents(e);
		}
	}
}

void GameState::update(const float& dt)
{
	if (Check_Mutex) {
		m_loadingScreen.at("g1"_hash).rotate(200 * dt);
		m_loadingScreen.at("g2"_hash).rotate(-150 * dt);
		m_loadingScreen.at("g3"_hash).rotate(110 * dt);
		m_loadingScreen.at("g4"_hash).rotate(60 * dt);
		timerCursor += dt;
	}
	else {
		if (InGame) {
			ga.update(dt);

			/*std::get<0>(m_sound).setVolume(OptionState::volume);
			std::get<1>(m_sound).setVolume(OptionState::volume);
			std::get<2>(m_sound).setVolume(OptionState::volume);*/

			if (ga.Return) {
				/*if (CurrentMap == "Niveau1")
					std::get<0>(m_sound).stop();
				if (CurrentMap == "Niveau2")
					std::get<1>(m_sound).stop();
				if (CurrentMap == "Niveau3")
					std::get<2>(m_sound).stop();*/
				InGame = false;
				/*m_music.play();*/
				return;
			}

			if (ga.Reset) {
				/*if (CurrentMap == "Niveau1")
					std::get<0>(m_sound).stop();
				if (CurrentMap == "Niveau2")
					std::get<1>(m_sound).stop();
				if (CurrentMap == "Niveau3")
					std::get<2>(m_sound).stop();*/
				ga.Restart();
				/*if (CurrentMap == "Niveau1")
					std::get<0>(m_sound).play();
				if (CurrentMap == "Niveau2")
					std::get<1>(m_sound).play();
				if (CurrentMap == "Niveau3")
					std::get<2>(m_sound).play();*/
				return;
			}

			if (ga.ReturnAfterLose) {
				/*if (CurrentMap == "Niveau1")
					std::get<0>(m_sound).stop();
				if (CurrentMap == "Niveau2")
					std::get<1>(m_sound).stop();
				if (CurrentMap == "Niveau3")
					std::get<2>(m_sound).stop();*/
				ga.Restart();
				/*if (CurrentMap == "Niveau1")
					std::get<0>(m_sound).play();
				if (CurrentMap == "Niveau2")
					std::get<1>(m_sound).play();
				if (CurrentMap == "Niveau3")
					std::get<2>(m_sound).play();*/
				return;
			}
		
			if (ga.ReturnMenuAfterLose) {
				/*if (CurrentMap == "Niveau1")
					std::get<0>(m_sound).stop();
				if (CurrentMap == "Niveau2")
					std::get<1>(m_sound).stop();
				if (CurrentMap == "Niveau3")
					std::get<2>(m_sound).stop();*/

				if (threading.joinable())
					threading.join();

				fsmManager->popState();
				/*m_music.play();*/
				InGame = false;
				return;
			}
			
			if (ga.NewScoreToScoreState) {
				/*if (CurrentMap == "Niveau1")
					std::get<0>(m_sound).stop();
				if (CurrentMap == "Niveau2")
					std::get<1>(m_sound).stop();
				if (CurrentMap == "Niveau3")
					std::get<2>(m_sound).stop();*/

				if (threading.joinable())
					threading.join();

				threading = std::thread([this] {
						Check_Mutex.lock();
						ptr = FSMManager::create<ScoreState>(*fsmManager, *window, *font, ScoreState::Return::Game, false);
						Check_Mutex.unlock();
						InGame = true;
						/*m_music.play();*/
					});
				InGame = false;
				return;
			}
		}
		else {
			if (sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheHori)) < -30 ||
				sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheHori)) > 30 ||
				sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheVert)) < -30 ||
				sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheVert)) > 30)
				sf::Mouse::setPosition(sf::Vector2i(sf::Mouse::getPosition().x + sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheHori)) * 7 * dt,
					sf::Mouse::getPosition().y + sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheVert)) * 7 * dt));
		}
	}
}

void GameState::draw(sf::RenderWindow& window)
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
	else {
		if (InGame)
			ga.draw(window);
		else {
			window.draw(fond);
			Select_Level.draw(window);
		}
	}
	
}