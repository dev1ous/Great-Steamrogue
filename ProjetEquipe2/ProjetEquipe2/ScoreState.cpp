#include "ScoreState.h"
#include "CacheSystem.h"
#include "Functions.h"
#include "MenuState.h"
#include "GameState.h"
#include "EditorState.h"

ScoreState::ScoreState(FSMManager& _fsm, sf::RenderWindow& _window, sf::Font& _font, Return _state, const bool change) :FSM{ _fsm,_window,_font,change },
highscores_b{ nullptr, func::rectangle({950,950},{346,101},sf::Color::White,sf::Color::White,0.f),func::CreateText("MENU",_font, 40,{},sf::Color::Black) }
{
	State = _state;
	highscoreTable.load();

	highscores_b.setOrigin().CenterText();
	highscores_b.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("menu_1")).setTextureState("menu_1", "menu_2", "menu_3");
	highscores_b.CreateFunctionCall([this]() {
			if (threading.joinable())
				threading.join();

			threading = std::thread([this]
				{
					Check_Mutex.lock();
					ptr = FSMManager::create<MenuState>(*fsmManager, *window, *font, false);
					Check_Mutex.unlock();
				});
		});
	Button& recommencer_ = highscores_b.Add("recommencer"_hash, func::rectangle({ 400,950 }, { 481, 101 }, sf::Color::White, sf::Color::White, 0.f),
		func::CreateText("RECOMMENCER", _font, 40, {},sf::Color::Black));
	recommencer_.setOrigin().CenterText();
	recommencer_.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("recommencer_1"));
	recommencer_.setTextureState("recommencer_1", "recommencer_2", "recommencer_3");
	recommencer_.CreateFunctionCall([this]() {
			if (threading.joinable())
				threading.join();

			threading = std::thread([this]
				{
					Check_Mutex.lock();
					if (State == Return::Game) {
						ptr = FSMManager::create<GameState>(*fsmManager, *window, *font, false);
					}
					else if (State == Return::Editor) {
						ptr = FSMManager::create<EditorState>(*fsmManager, *window, *font, false);
					}
					Check_Mutex.unlock();
					/*if (State == Return::Editor)*/
						/*m_music.play();*/
				});
		});
	Button& NextLevel = highscores_b.Add("nextlevel"_hash, func::rectangle({ 1500,950 }, { 481,101 }, sf::Color::White, sf::Color::White, 0.f),
		func::CreateText("NEXT LEVEL", _font, 40, {},sf::Color::Black));
	NextLevel.setOrigin().CenterText();
	NextLevel.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("niveau_suivant_1"));
	NextLevel.setTextureState("niveau_suivant_1", "niveau_suivant_2", "niveau_suivant_3");
	NextLevel.CreateFunctionCall([this]() {
			if (threading.joinable())
				threading.join();

			threading = std::thread([this]
				{
					Check_Mutex.lock();
					if (State == Return::Game) {
						ptr = FSMManager::create<GameState>(*fsmManager, *window, *font, false);
					}
					else if (State == Return::Editor) {
						ptr = FSMManager::create<EditorState>(*fsmManager, *window, *font, false);
					}
					Check_Mutex.unlock();
					/*if (State == Return::Editor)*/
						/*m_music.play();*/
				});
		});

	m_fondHigh.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("highscore"));
	m_fondHigh.setPosition(m_fondPos);
}

ScoreState::~ScoreState()
{
	if (threading.joinable())
		threading.join();
}

void ScoreState::GoBackPreviousState()
{

}

void ScoreState::processEvents(const sf::Event& e, const float& dt)
{
	switch (e.type)
	{
	case sf::Event::Closed:
		if (threading.joinable())
			threading.join();

		window->close();
		break;
	case sf::Event::KeyPressed:
		switch (e.key.code)
		{
		default:
			break;
		}
		break;
	default:
		break;
	}
	highscores_b.processEvents(e);
}

void ScoreState::update(const float& dt)
{
	
}

void ScoreState::draw(sf::RenderWindow& window)
{
	window.draw(m_fondHigh);
	highscoreTable.draw(&window, *font);
	highscores_b.draw(window);
}

