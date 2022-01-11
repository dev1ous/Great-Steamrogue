#include "ChargeSwordHUD.h"
#include "CacheSystem.h"

ChargeSwordHUD::ChargeSwordHUD(const std::string& texture, sf::Vector2f pos, sf::Vector2f origin) : m_rectCharge(0, 0 , 80, 80),
m_spriteCharge(gm::CacheSystem::AddAnyRessources<sf::Texture>(texture), m_rectCharge)
{
    m_spriteCharge.setPosition(pos);
    m_spriteCharge.setOrigin(origin);
}

void ChargeSwordHUD::update(const float& dt)
{
    if (m_rectCharge.left == 240) {
        timerCharge = 0.f;
    }
    else 
    {
        timerCharge += dt;
        if (timerCharge > 1.f) {
            m_rectCharge.left += 80;
            m_spriteCharge.setTextureRect(m_rectCharge);
            timerCharge = 0.f;
        }
    }
}

void ChargeSwordHUD::draw(sf::RenderWindow* window)
{
    window->draw(m_spriteCharge);
}
