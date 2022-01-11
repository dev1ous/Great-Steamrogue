#ifndef EDITORSTATE_H
#define EDITORSTATE_H

#include "FSMManager.h"
#include "Button.h"
#include "TilesMap.h"
#include "Logger.h"
#include "Game.h"

class FSMManager;

class EditorState final : public FSM
{
private :
	std::unordered_map<HashID, sf::Sprite> m_loadingScreen;
	sf::Sprite m_fond;
	sf::Sprite m_fond_d;
	sf::Text textL{ "Loading", *font,50 };
	std::array < sf::Text, 3 > m_points;
	float timerCursor{ 0.f };

	std::unordered_map<std::string, std::pair<sf::Text, RoundedRectangleShape>> Emplace_Map;
	std::unordered_map<HashID, RoundedRectangleShape> rounded;
	std::unordered_map<HashID, sf::Sprite> m_buttons;
	std::unordered_map<HashID, sf::Text> text;
	Button button;
	Button Select_Tiles;
	Button Move;
	std::vector<Button> Parent;
	[[maybe_unused]] std::string* selectedTexture{ nullptr };
	TilesMap tm;
	MapManager Map;
	Tiles Tiles_Select{ "" };
	Logger log;
	sf::View view;
	sf::Vector2f lastpass;
	sf::Vector2i Nbr_Screen{ 1,1 };
	std::string input_text;

	bool Zoom{ false };
	bool Dezoom{ false };
	bool BlockPose{ true };
	int selectedLayer{ 1 };
	bool findMap{ false };
	bool cursorText{ false };
	bool Joystick{ false };
	char Caracter{ 'a' };
	void CheckLayersForTiles();
	void CheckTraps();
	void Load_Tiles();
	void Load_Deco();
	void Load_Asset();
	void Load_Traps();
	void Load_World();
	void Load_Biome();
	void ChooseCollect();
	void ChooseEnemy();
	void StartOrEnd();
	bool HideMenu{ false };

	void drawlayer();
	void drawCollectible();
	void drawEnnemies();

	Game ga;
	enum class World { Egouts, Ville, Serre };
	World Biome{ World::Ville };
	enum class State { Editor, Save, Load, New_Save, Select_Tiles, Select_World, Select_Asset, Set_Start, Traps, Select_Traps, Play, Asset, Collectible, Set_Enemy, Select, Set_End };
	State state{ State::Editor };
	enum class Collect { None, Montre, Bouclier, Special };
	Collect collectible{ Collect::None };
	enum class Enemy { None, Garde, Drone, Spider, Boss };
	Enemy Enemies{ Enemy::None };


public:
	EditorState(FSMManager& _fsm, sf::RenderWindow& _window, sf::Font& _font, const bool change);
	~EditorState();

	void GoBackPreviousState() override;
	void processEvents(const sf::Event& e, const float& dt) override;
	void update(const float& dt)override;
	void draw(sf::RenderWindow& window)override;
};

#endif