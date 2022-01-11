#include "TilesManager.h"
#include <fstream>

TilesManager::TilesManager(std::string _name):name(_name)
{
}

void MapManager::LoadMap()
{
	std::ifstream save("../Ressources/Infos/Map.load");
	if (save.is_open()) {
		std::string line;
		while (std::getline(save, line)) {
			std::string tmpname;

			tmpname = line.substr(0, line.find(" "));
			line.erase(0, line.size());

			listmap.push_back(TilesManager(tmpname));
		}
	}
}

void MapManager::SaveMap()
{
	std::ofstream save("../Ressources/Infos/Map.load");
	if (save.is_open()) {
		for (auto& Current : listmap)
			save << Current.getName() << std::endl;
	}
}

std::string MapManager::GetPathMap(std::string _name)
{
	for (auto& Current : listmap)
		if (Current.getName() == _name)
			return Current.getName();
	return "";
}
