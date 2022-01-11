#include "TilesMap.h"
#include <algorithm>
#include <iostream>
#include "Functions.h"

void TilesMap::AddTiles(Tiles _tiles, layers wanted)
{
	for (auto it = std::begin(m_map); it != std::end(m_map); ++it) {
		if (it->first == wanted)
			it->second.emplace_back(_tiles);
	}
}

void TilesMap::RemoveTiles(const sf::Vector2f& pos, layers wanted)
{
	for (auto it = std::begin(m_map); it != std::end(m_map); ++it) {
		if (it->first == wanted) {
			it->second.erase(std::remove_if(std::begin(it->second), std::end(it->second), [&pos](Tiles value) {
				return value.getShape().getGlobalBounds().contains(pos);
				}), std::end(it->second));
		}
	}
}

void TilesMap::RemoveVector(const sf::Vector2f& pos, std::vector<sf::Vector2f>& _v)
{
	_v.erase(std::remove_if(std::begin(_v), std::end(_v), [&pos](sf::Vector2f value) {
		return func::Circle_Collision(value, pos, 50, 50);
		}), std::end(_v));
}

void TilesMap::AddTrap(Tiles _tiles)
{
	Traps.push_back(_tiles);
}

void TilesMap::RemoveTrap(const sf::Vector2f& pos)
{
	Traps.erase(std::remove_if(std::begin(Traps), std::end(Traps), [&pos](Tiles value) {
		return sf::FloatRect(value.GetPos().x, value.GetPos().y, 64, 64).contains(pos);
		}), std::end(Traps));
}

void TilesMap::Update(sf::Vector2f _vitesse, const float& _dt, sf::View _view)
{
	if (Plan5.first != "") {
		Plan5.second.second.move(sf::Vector2f((_vitesse.x * 0.75) * _dt, 0));
		Plan5.second.first.move(sf::Vector2f((_vitesse.x * 0.75) * _dt, 0));

		Defilement(Plan5, _view);
	}
	if (Plan4.first != "") {
		Plan4.second.second.move(sf::Vector2f((_vitesse.x * 0.5) * _dt, 0));
		Plan4.second.first.move(sf::Vector2f((_vitesse.x * 0.5) * _dt, 0));

		Defilement(Plan4, _view);
	}
	if (Plan3.first != "") {
		Plan3.second.second.move(sf::Vector2f((_vitesse.x * 0.25) * _dt, 0));
		Plan3.second.first.move(sf::Vector2f((_vitesse.x * 0.25) * _dt, 0));

		Defilement(Plan3, _view);
	}
	if (Plan1.first != "") {
		Plan1.second.second.move(sf::Vector2f((-_vitesse.x * 1.05) * _dt, 0));
		Plan1.second.first.move(sf::Vector2f((-_vitesse.x * 1.05) * _dt, 0));

		Defilement(Plan1, _view);
	}
}

void TilesMap::AddLayer()
{
	m_map.insert(std::make_pair(layers::back, std::vector<Tiles>{}));
	m_map.insert(std::make_pair(layers::player, std::vector<Tiles>{}));
	m_map.insert(std::make_pair(layers::deco, std::vector<Tiles>{}));
	m_map.insert(std::make_pair(layers::front, std::vector<Tiles>{}));
}

std::vector<Tiles>& TilesMap::GetLayer(layers _layers)
{
	auto it = m_map.find(_layers);

	if (it != m_map.end())
		return it->second;
	throw std::runtime_error("didnt find the layer");
}

void TilesMap::RemoveLayer(layers wanted)
{
	for (auto it = std::begin(m_map); it != std::end(m_map); ++it)
		if (it->first == wanted)
			m_map.erase(it->first);
}

void TilesMap::Reset()
{
	m_map.clear();
	Traps.clear();
	Montre.clear();
	Bouclier.clear();
	Special.clear();
	Drone.clear();
	Araigne.clear();
	Garde.clear();
	AddLayer();
	Startpos = { 960,540 };
}

void TilesMap::draw(sf::RenderWindow& window, layers wanted, sf::View* view)
{
	for (auto it = std::begin(m_map); it != std::end(m_map); ++it) {
		if (it->first == wanted) {
			for (auto itl = std::begin(it->second); itl != std::end(it->second); ++itl) {
				if (sf::FloatRect(view->getCenter().x - view->getSize().x / 2 - 64, view->getCenter().y - view->getSize().y / 2 - 64,
					view->getSize().x + 64, view->getSize().y + 64).contains(itl->GetPos()))
					itl->draw(window);
			}
		}
	}
}

void TilesMap::drawTraps(sf::RenderWindow& window, sf::View* view)
{
	for (auto it = std::begin(Traps); it != std::end(Traps); ++it) {
		if (sf::FloatRect(view->getCenter().x - view->getSize().x / 2 - 64, view->getCenter().y - view->getSize().y / 2 - 64,
			view->getSize().x + 64, view->getSize().y + 64).contains(it->GetPos()))
			it->draw(window);
	}
}

void TilesMap::draw(sf::RenderWindow& window,layers wanted) const
{
	for (auto it = std::begin(m_map); it != std::end(m_map);++it) {
		if (it->first == wanted) {
			for (auto itl = std::begin(it->second); itl != std::end(it->second); ++itl) {
				itl->draw(window);
			}
		}
	}
}

void TilesMap::draw(sf::RenderTexture& texture, layers wanted) const
{
	for (auto it = std::begin(m_map); it != std::end(m_map); ++it) {
		if (it->first == wanted) {
			for (auto itl = std::begin(it->second); itl != std::end(it->second); ++itl) {
				itl->draw(texture, sf::Color::Black);
			}
		}
	}
}

void TilesMap::drawTrapsT(sf::RenderTexture& texture) const
{
	std::for_each(std::begin(Traps), std::end(Traps), [&texture](std::vector<Tiles>::value_type t) {
		t.draw(texture, sf::Color::Red);
		});
}

void TilesMap::Draw_BackGround(sf::RenderWindow& window)
{
	window.draw(Background.second);
}

void TilesMap::Draw_BackPlan(sf::RenderWindow& window)
{
	if (Plan5.first != "") {
		window.draw(Plan5.second.second);
		window.draw(Plan5.second.first);
	}
	if (Plan4.first != "") {
		window.draw(Plan4.second.second);
		window.draw(Plan4.second.first);
	}
	if (Plan3.first != "") {
		window.draw(Plan3.second.second);
		window.draw(Plan3.second.first);
	}
}

void TilesMap::Draw_FrontPlan(sf::RenderWindow& window)
{
	if (Plan1.first != "") {
		window.draw(Plan1.second.second);
		window.draw(Plan1.second.first);
	}
}

void TilesMap::Defilement(std::pair<std::string, std::pair<sf::Sprite, sf::Sprite>>& _plan, sf::View _view)
{
	if (_plan.second.second.getPosition().x < _view.getCenter().x - _view.getSize().x / 2 - _plan.second.second.getGlobalBounds().width) {
		_plan.second.second.setPosition(_plan.second.first.getPosition().x + _plan.second.second.getGlobalBounds().width, 0);
	}

	if (_plan.second.second.getPosition().x > _view.getCenter().x - _view.getSize().x / 2 + _plan.second.second.getGlobalBounds().width) {
		_plan.second.second.setPosition(_plan.second.first.getPosition().x - _plan.second.second.getGlobalBounds().width, 0);
	}

	if (_plan.second.first.getPosition().x < _view.getCenter().x - _view.getSize().x / 2 - _plan.second.first.getGlobalBounds().width) {
		_plan.second.first.setPosition(_plan.second.second.getPosition().x + _plan.second.first.getGlobalBounds().width, 0);
	}

	if (_plan.second.first.getPosition().x > _view.getCenter().x - _view.getSize().x / 2 + _plan.second.first.getGlobalBounds().width) {
		_plan.second.first.setPosition(_plan.second.second.getPosition().x - _plan.second.first.getGlobalBounds().width, 0);
	}
}
