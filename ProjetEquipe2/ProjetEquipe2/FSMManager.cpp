#include "FSMManager.h"

FSMManager::FSMManager() : goLastState(false)
{
}

void FSMManager::Init(std::unique_ptr<FSM> _state)
{
	m_states.push(std::move(_state));
}

void FSMManager::pushState()
{
	if (goLastState) {
		if (!m_states.empty()) {
			m_states.pop();
			m_states.top()->GoBackPreviousState();
		}
		goLastState = false;
	}
	if (!m_states.empty()) {
		std::unique_ptr<FSM> tmp = m_states.top()->changeState();
		if (tmp != nullptr) {
			if (tmp->isChanged())
				m_states.pop();
			else
				m_states.top()->GoBackPreviousState();
			m_states.push(std::move(tmp));
		}
	}
}

void FSMManager::popState()
{
	goLastState = true;
}

void FSMManager::processEvents(const sf::Event& e,const float& dt)
{
	m_states.top()->processEvents(e, dt);
}

void FSMManager::update(const float& dt)
{
	m_states.top()->update(dt);
}

void FSMManager::draw(sf::RenderWindow& window)
{
	m_states.top()->draw(window);
}
