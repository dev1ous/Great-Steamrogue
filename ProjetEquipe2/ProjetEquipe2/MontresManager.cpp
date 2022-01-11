#include "MontresManager.h"
#include <algorithm>

MontresManager::MontresManager()
{
	c_montre.emplace("montreHUD_1", std::make_shared<montreHUD>("montreHUD3_", sf::Vector2f(107.f, 15.f), sf::Vector2f(10.f, 10.f)));
	c_montre.emplace("montreHUD2", std::make_shared<montreHUD>("montreHUD_", sf::Vector2f(107.f, 15.f), sf::Vector2f(10.f, 10.f)));
	c_montre.emplace("montreHUD3", std::make_shared<montreHUD>("montreHUD2_", sf::Vector2f(107.f, 15.f), sf::Vector2f(10.f, 10.f)));

	buf_m.loadFromFile("../Ressources/Sound/CollectiblesTaken.wav");
	sound_m.setBuffer(buf_m);
	sound_m.setVolume(20.f);
}

void MontresManager::Montre1(sf::Vector2f _pos)
{
	m_montre.insert(std::make_pair("montre_1", std::make_shared<Montres>("montre1", _pos, sf::Vector2f(10.f, 10.f), 50)));
}

void MontresManager::Montre2(sf::Vector2f _pos)
{
	m_montre.insert(std::make_pair("montre_2", std::make_shared<Montres>("montre2", _pos, sf::Vector2f(10.f, 10.f), 50)));
}

void MontresManager::Montre3(sf::Vector2f _pos)
{
	m_montre.insert(std::make_pair("montre_3", std::make_shared<Montres>("montre3", _pos, sf::Vector2f(10.f, 10.f), 50)));
}


void MontresManager::Clear()
{
	m_montre.clear();
	b1 = false;
	b2 = false;
	b3 = false;
}

void MontresManager::GrabMontre(Player* player,Chronometre* chrono)
{
	for (auto& [k, v] : m_montre) {
		if (k == "montre_1") {
			bool hit = v->CircleRect(player);
			if (hit) {
				sound_m.play();
				if (chrono->getStartSec() > 14)
					chrono->setStartSec(15);
				else
					chrono->setSSec(0);
				m_montre.erase(k);
				b1 = true;
				if (NbrDeMontres < 4)
					NbrDeMontres++;
				break;
			}
		}
		if (k == "montre_2") {
			bool hit2 = v->CircleRect(player);
			if (hit2) {
				sound_m.play();
				if (chrono->getStartSec() > 14)
					chrono->setStartSec(15);
				else
					chrono->setSSec(0);
				m_montre.erase(k);
				b2 = true;
				if (NbrDeMontres < 4)
					NbrDeMontres++;
				break;
			}
		}
		if (k == "montre_3") {
			bool hit3 = v->CircleRect(player);
			if (hit3) {
				sound_m.play();
				if (chrono->getStartSec() > 14)
					chrono->setStartSec(15);
				else
					chrono->setSSec(0);
				m_montre.erase(k);
				b3 = true;
				if (NbrDeMontres < 4)
					NbrDeMontres++;
				break;
			}
		}
	}
}

void MontresManager::drawAllMontre(sf::RenderWindow* window)
{
	std::for_each(std::begin(m_montre), std::end(m_montre), [window](M_Montre::value_type montre) { montre.second->drawMontre(window); });
}

void MontresManager::draw(sf::RenderWindow* window)
{
	if(b1)
		window->draw(c_montre.at("montreHUD_1").get()->getMontreHud());
	if(b2)
		window->draw(c_montre.at("montreHUD2").get()->getMontreHud());
	if(b3)
		window->draw(c_montre.at("montreHUD3").get()->getMontreHud());

}
