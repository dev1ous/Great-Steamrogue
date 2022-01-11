#ifndef LOGGER_H
#define LOGGER_H

#include <SFML/Graphics.hpp>
#include "TilesMap.h"

class Logger
{
public:
	Logger();
	~Logger() = default;

	void saveMap(const std::string& file,TilesMap& map, sf::Vector2i& size);
	void loadMap(const std::string& file,TilesMap& map, sf::Vector2i& size);
};
#endif
