#include "HUD.h"
#include "CacheSystem.h"

HUD::HUD()
{
	m_hud.setPosition({ 0,0 });
	m_hud.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("HUD_Renzo_2"));
}

void HUD::update(const float& dt)
{
}

void HUD::draw(sf::RenderWindow* window)
{
	window->draw(m_hud);
}
