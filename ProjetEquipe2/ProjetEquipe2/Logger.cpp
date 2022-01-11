#include "Logger.h"
#include <fstream>
#include "Tiles.h"
#include <string>
#include "CacheSystem.h"
#include <iostream>
#include <filesystem>

Logger::Logger()
{
}

void Logger::saveMap(const std::string& file,TilesMap& map, sf::Vector2i& size)
{
	const char* userpath = std::getenv("APPDATA");
	std::string Path;
	Path = Path.assign(userpath) + "/GreatSteamrogue/Map";

	std::replace(std::begin(Path), std::end(Path), '\\', '/');

	std::filesystem::create_directories(Path);

	std::ofstream writeMap(Path + "/" + file + ".txt");
	if (writeMap.is_open())
	{
		std::vector<Tiles> tmpMaps = map.GetLayer(TilesMap::layers::back);
		int i = 0;
		writeMap << std::to_string(static_cast<int>(size.x)) << std::endl;
		writeMap << std::to_string(static_cast<int>(size.y)) << std::endl;

		writeMap << (int)map.Startpos.x << std::endl;
		writeMap << (int)map.Startpos.y << std::endl;
		writeMap << (int)map.Endpos.x << std::endl;
		writeMap << (int)map.Endpos.y << std::endl;

		writeMap << map.Background.first << std::endl;
		writeMap << map.Plan5.first << std::endl;
		writeMap << map.Plan4.first << std::endl;
		writeMap << map.Plan3.first << std::endl;
		writeMap << map.Plan1.first << std::endl;
		
		writeMap << "Back_layer";
		std::string tmp = "Back_layer";
		bool done = true;
		while (done) {
			for (auto& t : tmpMaps) {
				i++;
				writeMap << std::endl;

				writeMap << t.GetFilepath() << " ";

				writeMap << std::to_string(static_cast<int>(t.GetPos().x)) << " ";

				writeMap << std::to_string(static_cast<int>(t.GetPos().y)) << " ";

				writeMap << std::to_string(static_cast<int>(t.GetRect().x)) << " ";

				writeMap << std::to_string(static_cast<int>(t.GetRect().y));
			}
			if (i == map.GetLayer(TilesMap::layers::back).size() && tmp == "Back_layer")
			{
				writeMap << std::endl;
				writeMap << "Player_layer";
				tmp = "Player_layer";
				tmpMaps = map.GetLayer(TilesMap::layers::player);
			}
			if (i == map.GetLayer(TilesMap::layers::back).size() + map.GetLayer(TilesMap::layers::player).size() 
				&& tmp == "Player_layer")
			{
				writeMap << std::endl;
				writeMap << "Deco_layer";
				tmp = "Deco_layer";
				tmpMaps = map.GetLayer(TilesMap::layers::deco);
			}
			if (i == map.GetLayer(TilesMap::layers::back).size() + map.GetLayer(TilesMap::layers::player).size()
				+ map.GetLayer(TilesMap::layers::deco).size() && tmp == "Deco_layer")
			{
				writeMap << std::endl;
				writeMap << "Front_layer";
				tmp = "Front_layer";
				tmpMaps = map.GetLayer(TilesMap::layers::front);
			}
			if (i == map.GetLayer(TilesMap::layers::back).size() + map.GetLayer(TilesMap::layers::player).size() 
				+ map.GetLayer(TilesMap::layers::deco).size() + map.GetLayer(TilesMap::layers::front).size() && tmp == "Front_layer")
			{
				done = false;
			}
		}

		writeMap << std::endl;
		writeMap << "Piques";
		for (auto& current : map.Traps) {
			writeMap << std::endl;

			writeMap << current.GetFilepath() << " ";

			writeMap << std::to_string(static_cast<int>(current.GetPos().x)) << " ";

			writeMap << std::to_string(static_cast<int>(current.GetPos().y)) << " ";

			writeMap << std::to_string(static_cast<int>(current.GetRect().x)) << " ";

			writeMap << std::to_string(static_cast<int>(current.GetRect().y));
		}

		writeMap << std::endl;
		writeMap << "Drone" << std::endl;
		for (auto& current : map.Drone) {
			writeMap << (int)current.x << ' ' << (int)current.y << std::endl;
		}

		writeMap << "Garde" << std::endl;
		for (auto& current : map.Garde) {
			writeMap << (int)current.x << ' ' << (int)current.y << std::endl;
		}

		writeMap << "Spider" << std::endl;
		for (auto& current : map.Araigne) {
			writeMap << (int)current.x << ' ' << (int)current.y << std::endl;
		}

		writeMap << "Bouclier" << std::endl;
		for (auto& current : map.Bouclier) {
			writeMap << (int)current.x << ' ' << (int)current.y << std::endl;
		}

		writeMap << "Special" << std::endl;
		for (auto& current : map.Special) {
			writeMap << (int)current.x << ' ' << (int)current.y << std::endl;
		}

		writeMap << "Montre" << std::endl;
		for (auto& current : map.Montre) {
			writeMap << (int)current.x << ' ' << (int)current.y << std::endl;
		}

		writeMap << "Boss" << std::endl;
		writeMap << map.Boss.x << ' ' << map.Boss.y;
		

		writeMap.close();
	}
}

void Logger::loadMap(const std::string& file,TilesMap& map, sf::Vector2i& size)
{
	map.Reset();

	const char* userpath = std::getenv("APPDATA");
	std::string Path;
	Path = Path.assign(userpath) + "/GreatSteamrogue/Map";

	std::replace(std::begin(Path), std::end(Path), '\\', '/');

	std::filesystem::create_directories(Path);

	if (file == "Niveau1" || file == "Niveau2" || file == "Niveau3")
		Path = "../Ressources/Map";

	std::ifstream readMap(Path + "/" + file + ".txt");
	if (readMap.is_open())
	{
		std::string line;
		std::string tmp;

		readMap >> size.x;
		readMap >> size.y;

		readMap >> map.Startpos.x;
		readMap >> map.Startpos.y;
		readMap >> map.Endpos.x;
		readMap >> map.Endpos.y;

		readMap >> map.Background.first;
		readMap >> map.Plan5.first;
		readMap >> map.Plan4.first;
		readMap >> map.Plan3.first;
		readMap >> map.Plan1.first;

		if (map.Background.first != "") {
			map.Background.second = sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>(map.Background.first));
		}
		if (map.Plan5.first != "") {
			map.Plan5.second.first = sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>(map.Plan5.first));
			map.Plan5.second.second = sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>(map.Plan5.first));

			map.Plan5.second.first.setPosition(0, 0);
			map.Plan5.second.second.setPosition(map.Plan5.second.second.getGlobalBounds().width, 0);
		}
		if (map.Plan4.first != "") {
			map.Plan4.second.first = sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>(map.Plan4.first));
			map.Plan4.second.second = sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>(map.Plan4.first));

			map.Plan4.second.first.setPosition(0, 0);
			map.Plan4.second.second.setPosition(map.Plan4.second.second.getGlobalBounds().width, 0);
		}
		if (map.Plan3.first != "") {
			map.Plan3.second.first = sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>(map.Plan3.first));
			map.Plan3.second.second = sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>(map.Plan3.first));

			map.Plan3.second.first.setPosition(0, 0);
			map.Plan3.second.second.setPosition(map.Plan3.second.second.getGlobalBounds().width, 0);
		}		
		if (map.Plan1.first != "") {
			map.Plan1.second.first = sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>(map.Plan1.first));
			map.Plan1.second.second = sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>(map.Plan1.first));

			map.Plan1.second.first.setPosition(0, 0);
			map.Plan1.second.second.setPosition(map.Plan1.second.second.getGlobalBounds().width, 0);
		}

		while (std::getline(readMap, line))
		{
			std::string name = "----";
			int tmpPosX{ 0 };
			int tmpPosY{ 0 };

			if (line == "Back_layer")
				tmp = "Back_layer";
			else if (line == "Player_layer")
				tmp = "Player_layer";
			else if (line == "Deco_layer")
				tmp = "Deco_layer";
			else if (line == "Front_layer")
				tmp = "Front_layer";
			else if (line == "Piques")
				tmp = "Piques";
			else if (line == "Drone")
				tmp = "Drone";
			else if (line == "Garde")
				tmp = "Garde";
			else if (line == "Spider")
				tmp = "Spider";
			else if (line == "Boss")
				tmp = "Boss";
			else if (line == "Bouclier")
				tmp = "Bouclier";
			else if (line == "Special")
				tmp = "Special";
			else if (line == "Montre")
				tmp = "Montre";
			else if (tmp == "Back_layer" || tmp == "Front_layer" || tmp == "Deco_layer" || tmp == "Player_layer" || tmp == "Piques")
			{
				int tmpX{ 0 };
				int tmpY{ 0 };

				name = line.substr(0, line.find(" "));
				line.erase(0, line.find(" ") + 1);

				tmpPosX = stoi(line.substr(0, line.find(" ")));
				line.erase(0, line.find(" ") + 1);

				tmpPosY = stoi(line.substr(0, line.find(" ")));
				line.erase(0, line.find(" ") + 1);

				tmpX = stoi(line.substr(0, line.find(" ")));
				line.erase(0, line.find(" ") + 1);

				tmpY = stoi(line.substr(0, line.find(" ")));
				line.erase(0, line.size());

				if (tmp == "Back_layer")
					map.AddTiles(Tiles(name, sf::Vector2f(tmpPosX, tmpPosY)), TilesMap::layers::back);
				if (tmp == "Front_layer")
					map.AddTiles(Tiles(name, sf::Vector2f(tmpPosX, tmpPosY)), TilesMap::layers::front);
				if (tmp == "Deco_layer")
					map.AddTiles(Tiles(name, sf::Vector2f(tmpPosX, tmpPosY), sf::Vector2i(tmpX, tmpY), sf::Vector2i(64, 64)), TilesMap::layers::deco);
				if (tmp == "Player_layer")
					map.AddTiles(Tiles(name, sf::Vector2f(tmpPosX, tmpPosY), sf::Vector2i(tmpX, tmpY), sf::Vector2i(64, 64)), TilesMap::layers::player);
				if (tmp == "Piques")
					map.AddTrap(Tiles(name, sf::Vector2f(tmpPosX, tmpPosY), sf::Vector2i(tmpX, tmpY), sf::Vector2i(64, 64)));
			}
			else if (tmp == "Drone") {
				tmpPosX = stoi(line.substr(0, line.find(" ")));
				line.erase(0, line.find(" ") + 1);

				tmpPosY = stoi(line.substr(0, line.find(" ")));
				line.erase(0, line.size());

				map.Drone.push_back(sf::Vector2f(tmpPosX, tmpPosY));
			}
			else if (tmp == "Garde") {
				tmpPosX = stoi(line.substr(0, line.find(" ")));
				line.erase(0, line.find(" ") + 1);

				tmpPosY = stoi(line.substr(0, line.find(" ")));
				line.erase(0, line.size());

				map.Garde.push_back(sf::Vector2f(tmpPosX, tmpPosY));
			}
			else if (tmp == "Spider") {
				tmpPosX = stoi(line.substr(0, line.find(" ")));
				line.erase(0, line.find(" ") + 1);

				tmpPosY = stoi(line.substr(0, line.find(" ")));
				line.erase(0, line.size());

				map.Araigne.push_back(sf::Vector2f(tmpPosX, tmpPosY));
			}
			else if (tmp == "Bouclier")	{
				tmpPosX = stoi(line.substr(0, line.find(" ")));
				line.erase(0, line.find(" ") + 1);

				tmpPosY = stoi(line.substr(0, line.find(" ")));
				line.erase(0, line.size());

				map.Bouclier.push_back(sf::Vector2f(tmpPosX, tmpPosY));
			}
			else if (tmp == "Special") {
				tmpPosX = stoi(line.substr(0, line.find(" ")));
				line.erase(0, line.find(" ") + 1);

				tmpPosY = stoi(line.substr(0, line.find(" ")));
				line.erase(0, line.size());

				map.Special.push_back(sf::Vector2f(tmpPosX, tmpPosY));
			}
			else if (tmp == "Montre") {
				tmpPosX = stoi(line.substr(0, line.find(" ")));
				line.erase(0, line.find(" ") + 1);

				tmpPosY = stoi(line.substr(0, line.find(" ")));
				line.erase(0, line.size());

				map.Montre.push_back(sf::Vector2f(tmpPosX, tmpPosY));
			}
			else if (tmp == "Boss") {
				tmpPosX = stoi(line.substr(0, line.find(" ")));
				line.erase(0, line.find(" ") + 1);

				tmpPosY = stoi(line.substr(0, line.find(" ")));
				line.erase(0, line.size());

				map.Boss = sf::Vector2f(tmpPosX, tmpPosY);
			}
		}
		readMap.close();
	}
}
