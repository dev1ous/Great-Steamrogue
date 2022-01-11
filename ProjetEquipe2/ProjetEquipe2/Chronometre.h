#ifndef CHRONOMETRE_H
#define CHRONOMETRE_H

#include <chrono>
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

class Chronometre
{
	float startSec{ 0.f }, StartMin{ 0.f };
	bool m_run{ false };
	sf::Text text;
public:
	Chronometre() = default;
	~Chronometre() = default;

	void startChronometre(const float&);
	void stopChronometre();
	void pauseChrono() { m_run = false; }
	void relanceChrono() { m_run = true; }

	bool ChronometreCheck();

	float getStartSec() { return startSec; }
	float getStartMin() { return StartMin; }

	void setSSec(int _new) { startSec = _new; }
	void setMMin(int _new) { StartMin = _new; }

	void setStartSec(int decrement) { startSec -= decrement; }
	void setStartMin(int decrement) { StartMin -= decrement; }

	sf::Text printCurrentChrono();
};
#endif
