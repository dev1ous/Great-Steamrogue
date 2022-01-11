#include "Application.h"
#include "SFML/Window/Event.hpp"
#include "MenuState.h"
#include <chrono>
#include <iostream>
#include "CacheSystem.h"

Application::Application()
{
	std::chrono::steady_clock::time_point Clock = std::chrono::high_resolution_clock::now();

	m_window.create(sf::VideoMode::getDesktopMode(), "Great Steamrogue", sf::Style::Fullscreen, sf::ContextSettings{ 0,0,8,1,1,0,false });

	m_font = gm::CacheSystem::AddAnyRessources<sf::Font>("veteran_typewriter");

	fsmManager.Init(FSMManager::create<MenuState>(fsmManager, m_window, m_font, true));

	m_window.setVerticalSyncEnabled(true);
	m_window.setFramerateLimit(60);
	m_window.setMouseCursorVisible(true);

	while (m_window.isOpen()) {

		const std::chrono::steady_clock::time_point tmp = std::chrono::high_resolution_clock::now();
		const auto m_fDeltaTime = tmp - Clock;
		Clock = tmp;

		fsmManager.pushState();

		if (m_window.hasFocus())
		{
			for (auto e = sf::Event{}; m_window.pollEvent(e);) {
				fsmManager.processEvents(e, std::chrono::duration<float>{m_fDeltaTime}.count());
			}

			fsmManager.update(std::chrono::duration<float>{m_fDeltaTime}.count());
		}

		m_window.clear(sf::Color(150, 150, 150, 255));
		m_window.setView(m_window.getDefaultView());
		fsmManager.draw(m_window);
		m_window.display();

		//TEST TIMER A REALISER
		//std::cout << std::to_string(10000 / std::chrono::duration<float>{m_fDeltaTime}.count() *10000) << std::endl;
	}
}

