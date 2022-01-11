#include "montreHUD.h"

montreHUD::montreHUD(const std::string& texture, sf::Vector2f pos, sf::Vector2f origin)
{
	m_pos_hud = pos;
	montre_hud.setPosition(m_pos_hud);
	m_origin_hud = origin;
	montre_hud.setOrigin(m_origin_hud);
	montre_hud.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>(texture));
}

