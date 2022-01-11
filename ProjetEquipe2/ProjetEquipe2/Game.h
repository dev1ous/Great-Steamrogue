#pragma once

#include "TilesMap.h"
#include "Logger.h"
#include "Player.h"
#include "AI.h"
#include "EliteAI.h"
#include "BasicAI.h"
#include "RemoteAI.h"
#include "MontresManager.h"
#include <map>
#include "CShieldManager.h"
#include "HUD.h"
#include "Button.h"
#include "FileLogger.h"
#include "BulletPlayer.h"

class Game
{
protected :
	std::unique_ptr<Player> player;
	Chronometre chronom;
	sf::Text printChronom;
	std::vector<std::shared_ptr<AI>> m_vectorAI;
	Logger log;
	TilesMap tm;
	MapManager Map;
	sf::Vector2i Size;
	sf::View view;
	sf::Image collid;
	MontresManager m_montres;
	CSManager chargeSword;
	CShieldManager chargeShield;
	HUD hud;
	bool pause{ false };
	bool BlockPauseWhenDie{ false };
	std::array<sf::Sprite, 5>menu_ig;
	Button Menu_IG;
	std::array<sf::Sprite, 3>m_sprite;
	sf::RectangleShape fondNoir{ {1920,1080} };
	sf::RenderWindow* window;
	sf::Font font;
	std::string input_text;
	bool cursorText{ false };
	bool Joystick{ false };
	char Caracter{ 'a' };
	FileLogger highscoreTable;
	Button FinDeNiv;
	Shield m_shield;
	sf::Text m_score, m_min, m_sec, m_score2, m_min2, m_sec2;
	sf::Sprite End;
	std::vector<BulletPlayer> bullet_p;
	float timerGameOver{ false };
	bool EnterOptionsIG{ false };
	sf::Sprite m_spriteIG;
	sf::Sprite m_volumeIG;
	sf::Text VsyncIG{ "V-SYNC ENABLED",font,40 }, VsyncDisabledIG{ "V-SYNC DISABLED",font,40 }, FullscreenIG{ "FULLSCREEN",font,40 }, FULLSCREENwIG{ "FULLSCREEN WINDOWED",font,40 };
	Button retourIG;
	bool selectedModeIG{ false };
	bool selectedVSYNCIG{ false };
	sf::Vector2f p1IG{ 415,270 };

	void InitEnemy();
	void InitCollectible();

public :
	Game(sf::RenderWindow* _window, sf::Font& _font);
	~Game() = default;

	void Init(std::string);
	void Restart();
	
	void DeleteAI(const float& _dt);
	void processEvents(const sf::Event& e, const float& dt);
	void AIUpdate(const float& dt);
	void renderAI(sf::RenderWindow* window);
	void renderPlayer(sf::RenderWindow* window);
	void Create_MaskCollid();
	void update(const float& dt);
	void draw(sf::RenderWindow& window);

	bool GameOver{ false };
	bool Return{ false };
	bool Reset{ false };
	bool NewScoreToScoreState{ false };
	bool ReturnAfterLose{ false };
	bool ReturnMenuAfterLose{ false };
};

