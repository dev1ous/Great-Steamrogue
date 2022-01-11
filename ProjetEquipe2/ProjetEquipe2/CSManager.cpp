#include "CSManager.h"
#include "Functions.h"

CSManager::CSManager()
{
	m_charge = ChargeSwordHUD("spritesheet_HUDtir", sf::Vector2f(20.f, 10.f), sf::Vector2f(10.f, 10.f));
	buffer_c.loadFromFile("../Ressources/Sound/CollectiblesTaken.wav");
	sound_c.setBuffer(buffer_c);
	sound_c.setVolume(20.f);
}


void CSManager::Clear()
{
	charge_sword.clear();
}

void CSManager::AddCharge(sf::Vector2f _pos)
{
	charge_sword.emplace_back("AnimChargeSword", _pos, sf::Vector2f(32.f, 32.f));
}

void CSManager::GrabChargeSword(Player* player)
{
	int i = 0;
	for (auto& c : charge_sword) {
		if (func::isCollide(player->GetShape(),c.getShape())) 
		{
			sound_c.play();
			NbrCharge++;
			m_charge.setRectLeft(0);
			charge_sword.erase(charge_sword.begin() + i);
			break;
		}
		i++;
	}
}

void CSManager::draw(sf::RenderWindow* window)
{
	std::for_each(std::begin(charge_sword), std::end(charge_sword), [window](cs::reference charge) { charge.draw(window); });
}

void CSManager::update(const float& dt)
{
	std::for_each(std::begin(charge_sword), std::end(charge_sword), [dt](cs::reference charge) { charge.AnimSword(dt); });
	updateChargeHUD(dt);
}

void CSManager::drawChargeHUD(sf::RenderWindow* window)
{
	m_charge.draw(window);
}

void CSManager::updateChargeHUD(const float& dt)
{
	if (NbrCharge == 1) 
		m_charge.update(dt);
	else if (NbrCharge == 2) {
		m_charge.setRectTop(80);
		m_charge.update(dt);
	}
	else if (NbrCharge == 3) {
		m_charge.setRectTop(160);
		m_charge.update(dt);
	}
	else if (NbrCharge == 0) 
	{
		m_charge.setRectTop(0);
		m_charge.setRectLeft(0);
		m_charge.setRect();
	}
}
