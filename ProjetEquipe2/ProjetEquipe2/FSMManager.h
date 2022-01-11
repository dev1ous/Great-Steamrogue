#ifndef FSMMANAGER_H
#define FSMMANAGER_H

#include "FSM.h"
#include <stack>
#include "SFML/Audio.hpp"

class FSMManager
{
	//choose stack cuz nice data structures like vector and no need to have more than 1 state in same time
	std::stack<std::unique_ptr<FSM>> m_states;
	bool goLastState;

public:
	FSMManager();

	void Init(std::unique_ptr<FSM>_state);
	void pushState();
	void popState();

	void processEvents(const sf::Event& e,const float& dt);
	void update(const float& dt);
	void draw(sf::RenderWindow& window);

	template<typename T, class ... Args>
	static std::unique_ptr<T> create(Args&& ...args);
};

template<typename T, class ... Args>
inline std::unique_ptr<T> FSMManager::create(Args&& ...args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}
#endif
