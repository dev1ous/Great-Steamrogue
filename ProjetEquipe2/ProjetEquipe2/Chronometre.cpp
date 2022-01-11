#include "Chronometre.h"
#include <sstream>
#include <iomanip>
#include "Functions.h"
#include "CacheSystem.h"

void Chronometre::startChronometre(const float& dt)
{
    if (startSec > 59)
        startSec = 0.f;
    else
        startSec += dt;
    StartMin += dt / 60;
    m_run = true;
}

void Chronometre::stopChronometre()
{
    startSec = 0.f;
    StartMin = 0.f;
    m_run = false;
}

bool Chronometre::ChronometreCheck()
{
   return m_run;
}

sf::Text Chronometre::printCurrentChrono()
{
    std::ostringstream sstr;
    sstr << std::setw(2) << std::setfill('0') << static_cast<int>(StartMin) << "   " << std::setw(2) << std::setfill('0') << static_cast<int>(startSec);

    text.setOutlineThickness(1.f);
    text.setString(sstr.str());
    text.setFont(gm::CacheSystem::AddAnyRessources<sf::Font>("Digital_System"));
    text.setCharacterSize(50);
    text.setFillColor(sf::Color(255, 177, 177, 155));
    text.setOutlineColor(sf::Color(255, 177, 177, 155));
    text.setPosition({ 1725,30 });

    return text;
}