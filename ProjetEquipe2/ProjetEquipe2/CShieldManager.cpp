#include "CShieldManager.h"
#include "Functions.h"

CShieldManager::CShieldManager()
{
	buf_cs.loadFromFile("../Ressources/Sound/CollectiblesTaken.wav");
	sound_cs.setBuffer(buf_cs);
	sound_cs.setVolume(20.f);
}

void CShieldManager::Clear()
{
	m_shieldManager.clear();
}

void CShieldManager::AddShield(sf::Vector2f _pos)
{
	m_shieldManager.emplace_back("AnimChargeShield", _pos, sf::Vector2f(32.f, 32.f));
}

void CShieldManager::GrabChargeSword(Player* player, Shield* shield)
{
	int i = 0;
	for (auto& c : m_shieldManager) {
		if (func::isCollide(player->GetShape(), c.getShape()))
		{
			sound_cs.play();
			shield->setLife(true);
			m_shieldManager.erase(m_shieldManager.begin() + i);
			break;
		}
		i++;
	}
}

void CShieldManager::draw(sf::RenderWindow* window)
{
	std::for_each(std::begin(m_shieldManager), std::end(m_shieldManager), [window](cs_manager::reference val) { val.draw(window); });
}

void CShieldManager::update(const float& dt)
{
	std::for_each(std::begin(m_shieldManager), std::end(m_shieldManager), [dt](cs_manager::reference val) { val.AnimShield(dt); });
}
