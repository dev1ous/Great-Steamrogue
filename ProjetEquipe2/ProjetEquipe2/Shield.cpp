#include "Shield.h"
#include "CacheSystem.h"

Shield::Shield(const std::string& texture,sf::Vector2f pos, sf::Vector2f origin)
{
	m_shield.setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>(texture));
	m_pos = pos;
	m_shield.setPosition(m_pos);
	m_origin = { 81 ,81 };
	m_shield.setOrigin(m_origin);

	animation.insert(std::make_pair("StartShield", Animator(sf::IntRect(0, 0, 162, 163), 5, 0.15f)));
	animation.insert(std::make_pair("ShieldPattern", Animator(sf::IntRect(0, 163, 162, 163), 23, 0.15f)));
	animation.insert(std::make_pair("EndShield", Animator(sf::IntRect(0, 326, 162, 163), 5, 0.15f)));
}

void Shield::update(const float& dt)
{
	if (inLife) {
		if (!leHoquetageDeLaurine) {
			animation.at("StartShield").Animation_NoRestart(m_shield, dt);
			if (animation.at("StartShield").Get_End()) {
				leHoquetageDeLaurine = true;
				animation.at("StartShield").Restart_Anim();
			}
		}
		else
			animation.at("ShieldPattern").Animation(m_shield, dt);
	}
}

void Shield::updateEndAnim(const float& dt)
{
	if (inLife) {
		if (leHoquetageDeLaurine) {
			if (!leHoquetementDeLaurine) {
				leHoquetementDeLaurine = true;
			}
			else {
				animation.at("EndShield").Animation_NoRestart(m_shield, dt);
				if (animation.at("EndShield").Get_End()) {
					leHoquetageDeLaurine = false;
					animation.at("EndShield").Restart_Anim();
				}
			}
		}
	}
}


void Shield::drawShield(sf::RenderWindow* window, Player* player)
{
	m_shield.setPosition(player->GetPos());

	if(inLife)
		window->draw(m_shield);
}
