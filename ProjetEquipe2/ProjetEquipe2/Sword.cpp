#include "Sword.h"
#include "CacheSystem.h"

Sword::Sword(const std::string& texture, sf::Vector2f pos, sf::Vector2f origin):
	swordRect(0,0,64,64), m_sword(gm::CacheSystem::AddAnyRessources<sf::Texture>(texture),swordRect)
{
	m_pos = pos;
	m_sword.setPosition(m_pos);
	m_origin = origin;
	m_sword.setOrigin(m_origin);
}

//int Sword::DamageInflicted(AI* ennemy,Player* player)
//{
//	int damage = 0;
//
//	//créer une variable qui augmente avec la velocity/dash( possiblement moins que la velocity elle meme)
//	if (double(std::rand()) / RAND_MAX > ennemy.getCurrentVelocity()) {
//		int attack = player->getDegats() + ( weapon == nullptr /*si le joueur n'a pas d'épée*/ ? 0 : damageSword);
//		int defense = ennemy.getPuissance()// implementez une variable dans chaque type d'ennemis pour evaluer leur puissance suivant leur niveau
//
//		if (player->attackSpecial()) {
//			damage = attack / 2 + std::rand() % (attack / 2 + 1);
//		}
//		else {
//			int baseDamage = attack - defense / 2;
//			if (damage < 1)
//				damage = std::rand() % 2;
//		}
//		ennemy.getPointsDeVie() -= damage;
//	}
//	return damage;
//}

void Sword::handleEvents(const sf::Event& e, const float& dt)
{
	switch (e.type) {
	case sf::Event::KeyPressed:
		switch (e.key.code) {
		case sf::Keyboard::Key::E:
			updateAnim(dt);
		default:
			break;
		}
	default:
		break;
	}
}

void Sword::updateAnim(const float& dt)
{
	timer += dt;
	if (timer > 1.f) {
		if (swordRect.left == 448)
			swordRect.left = 0;
		else
			swordRect.left += 64;
		m_sword.setTextureRect(swordRect);
		timer = 0.f;
	}
}

void Sword::updateCol(std::vector<AI>& ennemy,std::vector<Tiles>& tiles)
{
	for (auto& ai : ennemy) {
		if (func::isCollide(m_sword, ai.GetShape())) {
			
			break;
		}
	}
}

void Sword::draw(sf::RenderWindow* window)
{
	window->draw(m_sword);
}
