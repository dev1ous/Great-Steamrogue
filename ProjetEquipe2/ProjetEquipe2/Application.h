#ifndef APPLICATION_H
#define APPLICATION_H

#include "FSMManager.h"

class Application
{
	sf::RenderWindow m_window;
	FSMManager fsmManager;
	sf::Font m_font;

public:
	Application();
	~Application() = default;
};

#endif // !APPLICATION_H
