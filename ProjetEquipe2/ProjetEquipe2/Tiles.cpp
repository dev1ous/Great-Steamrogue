#include "Tiles.h"
#include "CacheSystem.h"
#include "Functions.h"

Tiles::Tiles(std::string _filepath, sf::Vector2f _pos, sf::Vector2i _rect, sf::Vector2i size) :m_position(_pos), filepath(_filepath), rect(_rect)
{
	if (_filepath != "") {
		m_shapeTile.setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>(_filepath));
		m_shapeTile.setTextureRect(sf::IntRect(_rect.x, _rect.y, size.x, size.y));
		m_shapeTile.setSize(sf::Vector2f(size.x, size.y));
	}
	
	setPos(_pos);
}

Tiles::Tiles(std::string _filepath, sf::Vector2f _pos) :m_position(_pos), filepath(_filepath)
{
	m_shapeTile.setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>(filepath));
	m_shapeTile.setSize(sf::Vector2f(m_shapeTile.getTexture()->getSize()));
	m_shapeTile.setOrigin(m_shapeTile.getGlobalBounds().width / 2, m_shapeTile.getGlobalBounds().height / 2);

	setPos(m_position);
}

void Tiles::setPos(const sf::Vector2f& pos)
{
	m_shapeTile.setPosition(pos);
}

void Tiles::draw(sf::RenderWindow& _window)const
{
	_window.draw(m_shapeTile);
}

void Tiles::draw(sf::RenderTexture& _texture, sf::Color _color) const
{
	_texture.draw(func::rectangle(m_position, m_shapeTile.getSize(), _color, sf::Color::Transparent, 0));
}
