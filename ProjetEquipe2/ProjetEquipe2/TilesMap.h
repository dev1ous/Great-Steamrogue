#ifndef TILESMAP_H
#define TILESMAP_H

#include "Tiles.h"
#include <unordered_map>
#include "TilesManager.h"

class TilesMap
{
public:
	enum class layers {
		back, player, deco, front,
	};

	TilesMap() = default;
	~TilesMap() = default;

	void AddTiles(Tiles _tiles, layers wanted);
	void RemoveTiles(const sf::Vector2f& pos, layers wanted);

	void RemoveVector(const sf::Vector2f& pos, std::vector<sf::Vector2f>& _v);

	void AddTrap(Tiles _tiles);
	void RemoveTrap(const sf::Vector2f& pos);

	void Update(sf::Vector2f _vitesse, const float& _dt, sf::View _view);

	void AddLayer();
	std::vector<Tiles>& GetLayer(layers _layers);
	void RemoveLayer(layers wanted);
	void Reset();

	//void Load_Map(std::string);

	void draw(sf::RenderWindow& window, layers wanted, sf::View* view);
	void drawTraps(sf::RenderWindow& window, sf::View* view);
	void draw(sf::RenderWindow& window, layers wanted)const;
	void draw(sf::RenderTexture& texture, layers wanted)const;
	void drawTrapsT(sf::RenderTexture& texture)const;

	void Draw_BackGround(sf::RenderWindow& window);
	void Draw_BackPlan(sf::RenderWindow& window);
	void Draw_FrontPlan(sf::RenderWindow& window);

	std::unordered_map <layers, std::vector<Tiles>>& GetMap() { return m_map; }

	std::pair<std::string, sf::Sprite> Background;
	std::pair<std::string, std::pair<sf::Sprite, sf::Sprite>> Plan5;
	std::pair<std::string, std::pair<sf::Sprite, sf::Sprite>> Plan4;
	std::pair<std::string, std::pair<sf::Sprite, sf::Sprite>> Plan3;
	std::pair<std::string, std::pair<sf::Sprite, sf::Sprite>> Plan1;

	sf::Vector2f Startpos{ 960,540 };
	sf::Vector2f Endpos{ 0,0 };
	std::vector<sf::Vector2f> Montre;
	std::vector<sf::Vector2f> Bouclier;
	std::vector<sf::Vector2f> Special;

	std::vector<sf::Vector2f> Drone;
	std::vector<sf::Vector2f> Araigne;
	std::vector<sf::Vector2f> Garde;
	sf::Vector2f Boss{ -5000,-5000 };
	std::vector<Tiles> Traps;

private:
	std::unordered_map < layers, std::vector<Tiles>> m_map;

	void Defilement(std::pair<std::string, std::pair<sf::Sprite, sf::Sprite>>& _plan, sf::View _view);
};
#endif

