#include "Montres.h"

Montres::Montres(const std::string& texture, sf::Vector2f pos, sf::Vector2f origin,float radius)
{
	m_pos = pos;
	montre.setPosition(m_pos);
	m_origin = origin;
	montre.setOrigin(m_origin);
	montre.setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>(texture));
	montre.setRadius(radius);
}

void Montres::drawMontre(sf::RenderWindow* window)
{
	window->draw(montre);
}

bool Montres::CircleRect(Player* player)
{
	float testX = m_pos.x;
	float testY = m_pos.y;

	if (m_pos.x < player->GetBounds().left)
		testX = player->GetBounds().left;
	else if (m_pos.x > player->GetBounds().left + player->GetBounds().width)
		testX = player->GetBounds().left + player->GetBounds().width;
	if (m_pos.y < player->GetBounds().top)
		testY = player->GetBounds().top;
	else if (m_pos.y > player->GetBounds().top + player->GetBounds().height)
		testY = player->GetBounds().top + player->GetBounds().height;

	float distX = m_pos.x - testX;
	float distY = m_pos.y - testY;
	float distance = sqrt((distX * distX) + (distY * distY));

	if (distance <= montre.getRadius())
		return true;

	return false;
}
