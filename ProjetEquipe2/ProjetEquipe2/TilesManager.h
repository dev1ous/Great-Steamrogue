#ifndef TILESMANAGER_H
#define TILESMANAGER_H

#include <string>
#include <vector>

class TilesManager
{
public:
	TilesManager() = default;
	TilesManager(std::string _name);
	~TilesManager() = default;

	inline std::string getName() const { return name; }

private:
	std::string name;
};

class MapManager
{
private :
	std::vector<TilesManager> listmap;

public :
	MapManager() = default;
	~MapManager() = default;

	void LoadMap();
	void SaveMap();

	void AddMap(std::string& _name) { listmap.push_back(TilesManager(_name)); }
	
	std::vector<TilesManager> Get_Map() { return listmap; }
	[[nodiscard]] std::string GetPathMap(std::string _name);
};
#endif