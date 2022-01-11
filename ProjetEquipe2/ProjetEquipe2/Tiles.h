#ifndef TILES_H
#define TILES_H

#include "SFML/Graphics.hpp"

class Tiles
{
	sf::RectangleShape m_shapeTile;
	sf::Vector2f m_position;
	std::string filepath;
	sf::Vector2i rect;

public:
	Tiles(std::string _filepath, sf::Vector2f _pos, sf::Vector2i _rect, sf::Vector2i size);
	Tiles(std::string _filepath, sf::Vector2f _pos);
	Tiles(std::string _filepath) :Tiles(_filepath, { 0,0 }, { 0,0 }, { 0,0 }) {};
	~Tiles() = default;
	std::string& GetFilepath() { return filepath; }
	sf::RectangleShape& getShape() { return m_shapeTile; }
	void setFilepath(std::string _texture) { filepath = _texture; }
	sf::Vector2f& GetPos() { return m_position; }
	sf::Vector2i& GetRect() { return rect; }
	void setPos(const sf::Vector2f& pos);
	void draw(sf::RenderWindow& _window)const;
	void draw(sf::RenderTexture& _texture, sf::Color _color)const;
};
#endif 

