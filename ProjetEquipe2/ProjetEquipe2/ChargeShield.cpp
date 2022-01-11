#include "ChargeShield.h"
#include "CacheSystem.h"

ChargeShield::ChargeShield(const std::string& texture, sf::Vector2f pos, sf::Vector2f origin) : rectShield(0, 0, 64, 64),
m_shape(gm::CacheSystem::AddAnyRessources<sf::Texture>(texture), rectShield)
{
    m_pos = pos;
    m_shape.setPosition(m_pos);
    m_origin = origin;
    m_shape.setOrigin(m_origin);
}

void ChargeShield::draw(sf::RenderWindow* window)
{
	window->draw(m_shape);
}

void ChargeShield::AnimShield(const float& dt)
{
    timer += dt;
    if (timer > 1.f) {
        if (rectShield.left == 448)
            rectShield.left = 0;
        else
            rectShield.left += 64;
        m_shape.setTextureRect(rectShield);
        timer = 0.f;
    }
}
