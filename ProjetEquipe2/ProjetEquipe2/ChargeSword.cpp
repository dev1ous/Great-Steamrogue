#include "ChargeSword.h"
#include "CacheSystem.h"

ChargeSword::ChargeSword(const std::string& texture, sf::Vector2f pos, sf::Vector2f origin) : rectSword(0, 0, 64, 64),
m_shape(gm::CacheSystem::AddAnyRessources<sf::Texture>(texture), rectSword)
{
	m_pos = pos;
	m_shape.setPosition(m_pos);
    m_origin = origin;
    m_shape.setOrigin(m_origin);
}

void ChargeSword::draw(sf::RenderWindow* window)
{
	window->draw(m_shape);
}

void ChargeSword::AnimSword(const float& dt)
{
    timer += dt;
    if (timer > 1.f) {
        if (rectSword.left == 384)
            rectSword.left = 0;
        else
            rectSword.left += 64;
        m_shape.setTextureRect(rectSword);
        timer = 0.f;
    }
}
