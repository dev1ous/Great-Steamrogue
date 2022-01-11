#include "EditorState.h"
#include "SFML/Window/Event.hpp"
#include "Functions.h"
#include "CacheSystem.h"
#include "ScoreState.h"
#include "OptionState.h"

EditorState::EditorState(FSMManager& _fsm, sf::RenderWindow& _window, sf::Font& _font, const bool change) 
	:FSM{ _fsm,_window,_font,change },
	button{ nullptr,func::rectangle({ 1406,80 },{64,64},sf::Color::White,sf::Color::White,0.f), sf::Text("CHARGER\nNIVEAU", *font, 20) },
	Select_Tiles{ nullptr, func::rectangle({ 253,80 } ,{64,64},sf::Color::White,sf::Color::White,0.f), sf::Text("CHARGER\nTILES", *font, 20) },
	Move{ nullptr, func::rectangle({ 960,220 } ,{64,64},sf::Color::White,sf::Color::White,0.f), sf::Text("REMONTER HUD", *font, 20) }, ga{ &_window, *font }
{
	/*m_music.play();*/
	m_loadingScreen.reserve(4);

	m_loadingScreen.insert(std::make_pair("g1"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("gear1"))));
	m_loadingScreen.insert(std::make_pair("g2"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("gear2"))));
	m_loadingScreen.insert(std::make_pair("g3"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("gear3"))));
	m_loadingScreen.insert(std::make_pair("g4"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("gear4"))));
	m_fond.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("Square"));

	m_loadingScreen.at("g1"_hash).setOrigin({ 30.f,30.5f });
	m_loadingScreen.at("g1"_hash).setPosition({ 1590.f,855.f });
	m_loadingScreen.at("g2"_hash).setOrigin({ 39.5f,40.f });
	m_loadingScreen.at("g2"_hash).setPosition({ 1268.f,958.f });
	m_loadingScreen.at("g3"_hash).setOrigin({ 59.f,60.f });
	m_loadingScreen.at("g3"_hash).setPosition({ 1715.f,965.f });
	m_loadingScreen.at("g4"_hash).setOrigin({ 54.5f,53.5f });
	m_loadingScreen.at("g4"_hash).setPosition({ 1275.f,885.f });
	m_fond.setPosition({ 1240.f,850.f });
	m_fond_d.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("Illu"));
	m_fond_d.setPosition({ 0,0 });
	textL.setOrigin(m_fond.getLocalBounds().left + m_fond.getLocalBounds().width / 2.f, m_fond.getLocalBounds().top + textL.getLocalBounds().height / 2.f);
	textL.setPosition({ m_fond.getPosition().x + 380, m_fond.getPosition().y + 60 });
	textL.setFillColor(sf::Color::Black);
	std::get<0>(m_points) = { ".", *font,50 };
	std::get<0>(m_points).setPosition({ textL.getPosition().x - 70,textL.getPosition().y - 20 });
	std::get<0>(m_points).setFillColor(sf::Color::Black);
	std::get<1>(m_points) = { ".", *font,50 };
	std::get<1>(m_points).setPosition({ textL.getPosition().x - 60,textL.getPosition().y - 20 });
	std::get<1>(m_points).setFillColor(sf::Color::Black);
	std::get<2>(m_points) = { ".", *font,50 };
	std::get<2>(m_points).setPosition({ textL.getPosition().x - 50,textL.getPosition().y - 20 });
	std::get<2>(m_points).setFillColor(sf::Color::Black);

	Select_Tiles.setOrigin().CenterText();
	tm.AddLayer();
	Map.LoadMap();

	view.setSize(1920, 1080);
	view.setCenter(960, 540);
	input_text = "";

	Load_Biome();

	rounded.insert(std::make_pair("backFond"_hash, RoundedRectangleShape({ 1920,256 }, 25.f, 15, sf::Color::White, sf::Color::White, 0.f, sf::Vector2f(0, 0))));
	rounded.at("backFond"_hash).setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>("menu_lvleditor"));

	Move.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("Arrow_Down1")).setTextureState("Arrow_Down1", "Arrow_Down1", "Arrow_Down1").Resize().setOrigin().setRotation(180).CenterText();
	Move.CreateFunctionCall([this] {
		HideMenu = !HideMenu;
		if (HideMenu) {
			rounded.at("backFond"_hash).setPosition(0, -200);
			Move.setRotation(0);
		}
		else {
			rounded.at("backFond"_hash).setPosition(0, 0);
			Move.setRotation(180);
		}

		Move.setPosition(sf::Vector2f(960, rounded.at("backFond"_hash).getPosition().y + 220));
		});

	Tiles_Select = Tiles("TilesE", sf::Vector2f(sf::Mouse::getPosition()), { 128,128 }, { 64,64 });
	Tiles_Select.getShape().setOrigin(32, 32);

	Select_Tiles.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("logo_tiles1")).setTextureState("logo_tiles1", "logo_tiles1", "logo_tiles2").setOrigin().Resize().CenterText();
	Select_Tiles.CreateFunctionCall([this]() {
		Select_Tiles.Children_Clear();
		m_buttons.insert(std::make_pair("TilesMenu"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("menu_selectionsprite"))));
		m_buttons["TilesMenu"_hash].setOrigin({ m_buttons["TilesMenu"_hash].getGlobalBounds().width / 2,m_buttons["TilesMenu"_hash].getGlobalBounds().height / 2 });
		m_buttons["TilesMenu"_hash].setPosition({ 960,540 });
		if (selectedLayer == 0) {
			Load_Asset();
			state = State::Select_Tiles;
		}
		if (selectedLayer == 1) {
			Load_Tiles();
			state = State::Select_Tiles;
		}
		if (selectedLayer == 2) {
			Load_Deco();
			state = State::Select_Tiles;
		}
		if (selectedLayer == 3) {
			Load_Asset();
			state = State::Select_Tiles;
		}
		});

	[[maybe_unused]] Button& fondJ = button.Add("boutton_arriere5"_hash, sf::RectangleShape({ 64,64 })).setOrigin().setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("spritesheet_boutton_arriere1"))
		.setTextureState("spritesheet_boutton_arriere1", "spritesheet_boutton_arriere2", "spritesheet_boutton_arriere2").setPosition({ 125,80 }).Resize();
	[[maybe_unused]] Button& tab24 = button.Add("World"_hash, func::rectangle(fondJ.Get_Position(), { 64,64 },
		sf::Color::White, sf::Color::Transparent, 0.f), sf::Text("ARRIERE-PLAN", *font, 20)).setOrigin()
		.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("logo_niv1")).setTextureState("logo_niv1", "logo_niv1", "logo_niv2").Resize();
	tab24.CenterText();
	tab24.CreateFunctionCall([this] {
		Select_Tiles.Children_Clear();
		m_buttons.insert(std::make_pair("TilesMenu"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("menu_selectionsprite"))));
		m_buttons["TilesMenu"_hash].setOrigin({ m_buttons["TilesMenu"_hash].getGlobalBounds().width / 2,m_buttons["TilesMenu"_hash].getGlobalBounds().height / 2 });
		m_buttons["TilesMenu"_hash].setPosition({ 960,540 });
		Load_World();
		state = State::Select_World;
		});

	[[maybe_unused]] Button& fondA = button.Add("boutton_arriere"_hash, sf::RectangleShape({ 64,64 })).setOrigin().setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("spritesheet_boutton_arriere1"))
		.setTextureState("spritesheet_boutton_arriere1", "spritesheet_boutton_arriere2", "spritesheet_boutton_arriere2").setPosition(button.Get_Position()).Resize();

	[[maybe_unused]] Button& fondX = button.Add("boutton_arriere"_hash, sf::RectangleShape({ 64,64 })).setOrigin().setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("spritesheet_boutton_arriere1"))
		.setTextureState("spritesheet_boutton_arriere1", "spritesheet_boutton_arriere2", "spritesheet_boutton_arriere2").setPosition(Select_Tiles.Get_Position()).Resize();

	[[maybe_unused]] Button& fondB = button.Add("boutton_arriere"_hash, sf::RectangleShape({ 64,64 })).setOrigin().setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("spritesheet_boutton_arriere1"))
		.setTextureState("spritesheet_boutton_arriere1", "spritesheet_boutton_arriere2", "spritesheet_boutton_arriere2").setPosition({ 1535,80 }).Resize();
	[[maybe_unused]] Button& tab1 = button.Add("SaveMap"_hash, func::rectangle(fondB.Get_Position(), { 64,64 },
		sf::Color::White, sf::Color::White, 0.f), sf::Text("SAUVEGARDER\nNIVEAU", *font, 20)).setOrigin()
		.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("logo_save1")).setTextureState("logo_save1", "logo_save1", "logo_save2").Resize();
	tab1.CenterText();

	[[maybe_unused]] Button& fondN = button.Add("boutton_arriere2"_hash, sf::RectangleShape({ 64,64 })).setOrigin().setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("spritesheet_boutton_arriere1"))
		.setTextureState("spritesheet_boutton_arriere1", "spritesheet_boutton_arriere2", "spritesheet_boutton_arriere2").setPosition({ 768,80 }).Resize();
	[[maybe_unused]] Button& tab2 = button.Add("Start"_hash, func::rectangle(fondN.Get_Position(), { 64,64 },
		sf::Color::White, sf::Color::White, 0.f), sf::Text("POSITION\nDEPART", *font, 20)).setOrigin()
		.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("logo_start_end1")).setTextureState("logo_start_end1", "logo_start_end1", "logo_start_end2").Resize();
	tab2.CenterText();
	tab2.CreateFunctionCall([this] {
		m_buttons.insert(std::make_pair("TilesMenu"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("menu_selectionsprite"))));
		m_buttons["TilesMenu"_hash].setOrigin({ m_buttons["TilesMenu"_hash].getGlobalBounds().width / 2,m_buttons["TilesMenu"_hash].getGlobalBounds().height / 2 });
		m_buttons["TilesMenu"_hash].setPosition({ 960,540 });
		StartOrEnd();
		state = State::Select;
		});

	[[maybe_unused]] Button& fondZ = button.Add("boutton_arriere4"_hash, sf::RectangleShape({ 64,64 })).setOrigin().setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("spritesheet_boutton_arriere1"))
		.setTextureState("spritesheet_boutton_arriere1", "spritesheet_boutton_arriere2", "spritesheet_boutton_arriere2").setPosition({ 510, 80 }).Resize();
	[[maybe_unused]] Button& tab4 = button.Add("Ennemis"_hash, func::rectangle(fondZ.Get_Position(), { 64,64 },
		sf::Color::White, sf::Color::Transparent, 0.f), sf::Text("ENNEMIES", *font, 20)).setOrigin()
		.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("logo_ennemis1")).setTextureState("logo_ennemis1", "logo_ennemis1", "logo_ennemis2").Resize();
	tab4.CenterText();
	tab4.CreateFunctionCall([this] {
		Select_Tiles.Children_Clear();
		m_buttons.insert(std::make_pair("TilesMenu"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("menu_selectionsprite"))));
		m_buttons["TilesMenu"_hash].setOrigin({ m_buttons["TilesMenu"_hash].getGlobalBounds().width / 2,m_buttons["TilesMenu"_hash].getGlobalBounds().height / 2 });
		m_buttons["TilesMenu"_hash].setPosition({ 960,540 });
		ChooseEnemy();
		Enemies = Enemy::None;
		state = State::Set_Enemy;
		});

	[[maybe_unused]] Button& fondE = button.Add("boutton_arriere5"_hash, sf::RectangleShape({ 64,64 })).setOrigin().setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("spritesheet_boutton_arriere1"))
		.setTextureState("spritesheet_boutton_arriere1", "spritesheet_boutton_arriere2", "spritesheet_boutton_arriere2").setPosition({ 636,80 }).Resize();
	[[maybe_unused]] Button& tab5 = button.Add("Pieges"_hash, func::rectangle(fondE.Get_Position(), { 64,64 },
		sf::Color::White, sf::Color::Transparent, 0.f), sf::Text("PIEGES", *font, 20)).setOrigin()
		.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("logo_pieges1")).setTextureState("logo_pieges1", "logo_pieges1", "logo_pieges2").Resize();
	tab5.CenterText();
	tab5.CreateFunctionCall([this] {
		Select_Tiles.Children_Clear();
		m_buttons.insert(std::make_pair("TilesMenu"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("menu_selectionsprite"))));
		m_buttons["TilesMenu"_hash].setOrigin({ m_buttons["TilesMenu"_hash].getGlobalBounds().width / 2,m_buttons["TilesMenu"_hash].getGlobalBounds().height / 2 });
		m_buttons["TilesMenu"_hash].setPosition({ 960,540 });
		Load_Traps();
		state = State::Select_Traps;
		});

	[[maybe_unused]] Button& fondR = button.Add("boutton_arriere6"_hash, sf::RectangleShape({ 64,64 })).setOrigin().setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("spritesheet_boutton_arriere1"))
		.setTextureState("spritesheet_boutton_arriere1", "spritesheet_boutton_arriere2", "spritesheet_boutton_arriere2").setPosition({ 383,80 }).Resize();
	[[maybe_unused]] Button& tab6 = button.Add("bonus"_hash, func::rectangle(fondR.Get_Position(), { 64,64 },
		sf::Color::White, sf::Color::White, 0.f), sf::Text("COLLECTIBLES", *font, 20)).setOrigin()
		.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("logo_bonus1")).setTextureState("logo_bonus1", "logo_bonus1", "logo_bonus2").Resize();
	tab6.CenterText();
	tab6.CreateFunctionCall([this] {
		Select_Tiles.Children_Clear();
		m_buttons.insert(std::make_pair("TilesMenu"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("menu_selectionsprite"))));
		m_buttons["TilesMenu"_hash].setOrigin({ m_buttons["TilesMenu"_hash].getGlobalBounds().width / 2,m_buttons["TilesMenu"_hash].getGlobalBounds().height / 2 });
		m_buttons["TilesMenu"_hash].setPosition({ 960,540 });
		ChooseCollect();
		collectible = Collect::None;
		state = State::Collectible;
		});

	m_buttons.insert(std::make_pair("Font_Layer"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("arriere_layer"))));
	m_buttons["Font_Layer"_hash].setOrigin({ m_buttons["Font_Layer"_hash].getGlobalBounds().width / 2,m_buttons["Font_Layer"_hash].getGlobalBounds().height / 2 });
	m_buttons["Font_Layer"_hash].setPosition({ 960,80 });

	m_buttons.insert(std::make_pair("Back_Layer"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("Layer1_1"))));
	m_buttons["Back_Layer"_hash].setOrigin({ m_buttons["Back_Layer"_hash].getGlobalBounds().width / 2,m_buttons["Back_Layer"_hash].getGlobalBounds().height / 2 });
	m_buttons["Back_Layer"_hash].setPosition({ 960,80 });

	m_buttons.insert(std::make_pair("Player_Layer"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("Layer3_1"))));
	m_buttons["Player_Layer"_hash].setOrigin({ m_buttons["Player_Layer"_hash].getGlobalBounds().width / 2,m_buttons["Player_Layer"_hash].getGlobalBounds().height / 2 });
	m_buttons["Player_Layer"_hash].setPosition({ 960,80 });

	m_buttons.insert(std::make_pair("Deco_Layer"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("Layer2_1"))));
	m_buttons["Deco_Layer"_hash].setOrigin({ m_buttons["Deco_Layer"_hash].getGlobalBounds().width / 2,m_buttons["Deco_Layer"_hash].getGlobalBounds().height / 2 });
	m_buttons["Deco_Layer"_hash].setPosition({ 960,80 });

	m_buttons.insert(std::make_pair("Front_Layer"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("Layer4_1"))));
	m_buttons["Front_Layer"_hash].setOrigin({ m_buttons["Front_Layer"_hash].getGlobalBounds().width / 2,m_buttons["Front_Layer"_hash].getGlobalBounds().height / 2 });
	m_buttons["Front_Layer"_hash].setPosition({ 960,80 });

	[[maybe_unused]] Button& tab7 = button.Add("DimLayer"_hash, func::rectangle({ m_buttons.at("Back_Layer"_hash).getPosition().x - 100,m_buttons.at("Back_Layer"_hash).getPosition().y },
		{ 37,65 }, sf::Color::White,
		sf::Color::White, 0.f)).setOrigin();
	[[maybe_unused]] Button& tab8 = button.Add("AugLayer"_hash, func::rectangle({ m_buttons.at("Back_Layer"_hash).getPosition().x + 100,m_buttons.at("Back_Layer"_hash).getPosition().y },
		{ 37,65 }, sf::Color::White,
		sf::Color::White, 0.f)).setOrigin();
	tab7.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("fleche1_L1")).setTextureState("fleche1_L1", "fleche1_L1", "fleche1_L2").Resize();
	tab8.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("fleche1_R1")).setTextureState("fleche1_R1", "fleche1_R1", "fleche1_R2").Resize();

	tab7.CreateFunctionCall([this] {
		selectedLayer--;
		if (selectedLayer < 0)
			selectedLayer = 0;
		if (selectedLayer == 1 || selectedLayer == 2) {
			Tiles_Select = Tiles("TilesE", sf::Vector2f(sf::Mouse::getPosition()), { 128,128 }, { 64,64 });
			Tiles_Select.getShape().setOrigin(32, 32);
		}
		else {
			Tiles_Select = Tiles("Cables", sf::Vector2f(sf::Mouse::getPosition()));
		}
		});
	tab8.CreateFunctionCall([this] {
		selectedLayer++;
		if (selectedLayer > 3)
			selectedLayer = 3;
		if (selectedLayer == 1 || selectedLayer == 2) {
			Tiles_Select = Tiles("TilesE", sf::Vector2f(sf::Mouse::getPosition()), { 128,128 }, { 64,64 });
			Tiles_Select.getShape().setOrigin(32, 32);
		}
		else {
			Tiles_Select = Tiles("Cables", sf::Vector2f(sf::Mouse::getPosition()));
		}
		});

	rounded.insert(std::make_pair("InfoScreenX"_hash, RoundedRectangleShape({ 64,64 }, 2.f, 2, sf::Color::White, sf::Color::White, 0.f, { 1280,80 })));
	rounded.insert(std::make_pair("InfoScreenY"_hash, RoundedRectangleShape({ 64,64 }, 2.f, 2, sf::Color::White, sf::Color::White, 0.f, { 1150,80 })));

	rounded.at("InfoScreenX"_hash).setOrigin(rounded.at("InfoScreenX"_hash).getGlobalBounds().width / 2, rounded.at("InfoScreenX"_hash).getGlobalBounds().height / 2);
	rounded.at("InfoScreenY"_hash).setOrigin(rounded.at("InfoScreenY"_hash).getGlobalBounds().width / 2, rounded.at("InfoScreenY"_hash).getGlobalBounds().height / 2);
	rounded.at("InfoScreenX"_hash).setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>("taille_decrans_horizontal"));
	rounded.at("InfoScreenY"_hash).setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>("taille_decrans_vertical"));

	text.insert(std::make_pair("ScreenX"_hash, func::CreateText(std::to_string(Nbr_Screen.x), *font, 20, rounded.at("InfoScreenX"_hash).getPosition(), sf::Color::White)));
	text.insert(std::make_pair("ScreenY"_hash, func::CreateText(std::to_string(Nbr_Screen.y), *font, 20, rounded.at("InfoScreenY"_hash).getPosition(), sf::Color::White)));

	[[maybe_unused]] Button& tab10 = button.Add("DimScreenX"_hash, func::rectangle({ rounded.at("InfoScreenX"_hash).getPosition().x - 55,rounded.at("InfoScreenX"_hash).getPosition().y },
		{ 37,65 }, sf::Color::White,
		sf::Color::White, 0.f)).setOrigin();
	[[maybe_unused]] Button& tab9 = button.Add("AugScreenX"_hash, func::rectangle({ rounded.at("InfoScreenX"_hash).getPosition().x + 52,rounded.at("InfoScreenX"_hash).getPosition().y },
		{ 37,65 }, sf::Color::White,
		sf::Color::White, 0.f)).setOrigin();

	tab10.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("fleche1_L1")).setTextureState("fleche1_L1", "fleche1_L1", "fleche1_L2").Resize();
	tab9.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("fleche1_R1")).setTextureState("fleche1_R1", "fleche1_R1", "fleche1_R2").Resize();

	tab9.CreateFunctionCall([this] {
		Nbr_Screen.x++;
		if (Nbr_Screen.x > 10)
			Nbr_Screen.x = 10;
		});
	tab10.CreateFunctionCall([this] {
		Nbr_Screen.x--;
		if (Nbr_Screen.x < 1)
			Nbr_Screen.x = 1;
		});

	[[maybe_unused]] Button& fondT = button.Add("boutton_arriere8"_hash, sf::RectangleShape({ 64,64 })).setOrigin().setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("spritesheet_boutton_arriere1"))
		.setTextureState("spritesheet_boutton_arriere1", "spritesheet_boutton_arriere2", "spritesheet_boutton_arriere2").setPosition({ 1790,80 }).Resize();

	[[maybe_unused]] Button& fondL = button.Add("boutton_arriere8"_hash, sf::RectangleShape({ 64,64 })).setOrigin().setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("spritesheet_boutton_arriere1"))
		.setTextureState("spritesheet_boutton_arriere1", "spritesheet_boutton_arriere2", "spritesheet_boutton_arriere2").setPosition({ 1663,80 }).Resize();

	[[maybe_unused]] Button& tab11 = button.Add("Play"_hash, func::rectangle(fondL.Get_Position(), { 64,64 },
		sf::Color::White, sf::Color::Transparent, 0.f), sf::Text("JOUER", *font, 20)).setOrigin()
		.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("logo_play1")).setTextureState("logo_play1", "logo_play1", "logo_play2").Resize();
	tab11.CenterText();
	tab11.CreateFunctionCall([this] {
		if (threading.joinable())
			threading.join();

		threading = std::thread([this]
			{
				Check_Mutex.lock();
				log.saveMap("Test", tm, Nbr_Screen);
				log.loadMap("Test", tm, Nbr_Screen);
				ga.Init("Test");
				state = State::Play;
				Check_Mutex.unlock();
			});
		});

	[[maybe_unused]] Button& tab12 = button.Add("Quit"_hash, func::rectangle(fondT.Get_Position(), { 64,64 },
		sf::Color::White, sf::Color::White, 0.f), sf::Text("QUIT", *font, 20)).setOrigin()
		.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("logo_quit1")).setTextureState("logo_quit1", "logo_quit1", "logo_quit2").Resize();
	tab12.CenterText();
	tab12.CreateFunctionCall([this] {
		m_music.play();
			fsmManager->popState();
		});

	button.setOrigin();
	button.CenterText();
	button.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("logo_load1"));
	button.setTextureState("logo_load1", "logo_load1", "logo_load1");
	tab1.CenterText();

	button.CreateFunctionCall([this]() {
		Parent.clear();

		int i = 0;
		for (const auto& Current : Map.Get_Map())
		{
			Emplace_Map.insert(std::make_pair(Current.getName(), std::make_pair(func::CreateText(Current.getName(), *font, 35,
				sf::Vector2f(960, 540 - ((Map.Get_Map().size() / 2) * 40) + (i * 40)), sf::Color::White),
				RoundedRectangleShape({ 200,35 }, 2.f, 2, sf::Color(30, 127, 203, 180), sf::Color(0, 0, 0, 255), 2.f, sf::Vector2f(960, 540 - ((Map.Get_Map().size() / 2) * 40) + (i * 40))))));
			Emplace_Map[Current.getName()].first.setOrigin(Emplace_Map[Current.getName()].first.getLocalBounds().left + Emplace_Map[Current.getName()].first.getLocalBounds().width / 2,
				Emplace_Map[Current.getName()].first.getLocalBounds().top + Emplace_Map[Current.getName()].first.getLocalBounds().height / 2);
			Emplace_Map[Current.getName()].second.setOrigin(Emplace_Map[Current.getName()].second.getGlobalBounds().width / 2, Emplace_Map[Current.getName()].second.getGlobalBounds().height / 2);
			i++;
		}

		Parent.push_back(Button(nullptr, RoundedRectangleShape({ 300,35 }, 2.f, 2, sf::Color(30, 127, 203, 180),
			sf::Color(0, 0, 0, 255), 2.f, sf::Vector2f(960, 890)), sf::Text("NOUVEAU NIVEAU", *font, 35)));
		Parent.back().setOrigin().CenterText();
		Parent.back().setColorState(sf::Color(30, 127, 203, 180), sf::Color::Red, sf::Color::Black);
		Parent.back().CreateFunctionCall([this] {
			Emplace_Map.clear();
			Nbr_Screen = { 1,1 };
			tm.Reset();
			state = State::Editor;
			if (selectedLayer == 1 || selectedLayer == 2) {
				Tiles_Select = Tiles("TilesE", sf::Vector2f(sf::Mouse::getPosition()), { 128,128 }, { 64,64 });
				Tiles_Select.getShape().setOrigin(32, 32);
			}
			else {
				Tiles_Select = Tiles("Cables", sf::Vector2f(sf::Mouse::getPosition()));
			}
			});

		Parent.push_back(Button(nullptr, RoundedRectangleShape({ 300,35 }, 2.f, 2, sf::Color(30, 127, 203, 180),
			sf::Color(0, 0, 0, 255), 2.f, sf::Vector2f(960, 940)), sf::Text("RETOUR", *font, 35)));
		Parent.back().setOrigin().CenterText();
		Parent.back().setColorState(sf::Color(30, 127, 203, 180), sf::Color::Red, sf::Color::Black);
		Parent.back().CreateFunctionCall([this] {
			Emplace_Map.clear();
			state = State::Editor;
			if (selectedLayer == 1 || selectedLayer == 2) {
				Tiles_Select = Tiles("TilesE", sf::Vector2f(sf::Mouse::getPosition()), { 128,128 }, { 64,64 });
				Tiles_Select.getShape().setOrigin(32, 32);
			}
			else {
				Tiles_Select = Tiles("Cables", sf::Vector2f(sf::Mouse::getPosition()));
			}
			});

		state = State::Load;
		});

	tab1.CreateFunctionCall([this]() {
		Parent.clear();

		int i = 0;
		for (const auto& Current : Map.Get_Map())
		{
			Emplace_Map.insert(std::make_pair(Current.getName(), std::make_pair(func::CreateText(Current.getName(), *font, 35,
				sf::Vector2f(960, 540 - ((Map.Get_Map().size() / 2) * 40) + (i * 40)), sf::Color::White),
				RoundedRectangleShape({ 200,35 }, 2.f, 2, sf::Color(30, 127, 203, 180), sf::Color(0, 0, 0, 255), 2.f, sf::Vector2f(960, 540 - ((Map.Get_Map().size() / 2) * 40) + (i * 40))))));
			Emplace_Map[Current.getName()].first.setOrigin(Emplace_Map[Current.getName()].first.getLocalBounds().left + Emplace_Map[Current.getName()].first.getLocalBounds().width / 2,
				Emplace_Map[Current.getName()].first.getLocalBounds().top + Emplace_Map[Current.getName()].first.getLocalBounds().height / 2);
			Emplace_Map[Current.getName()].second.setOrigin(Emplace_Map[Current.getName()].second.getGlobalBounds().width / 2, Emplace_Map[Current.getName()].second.getGlobalBounds().height / 2);
			i++;
		}

		Parent.push_back(Button(nullptr, RoundedRectangleShape({ 400,35 }, 2.f, 2, sf::Color(30, 127, 203, 180),
			sf::Color(0, 0, 0, 255), 2.f, sf::Vector2f(960, 890)), sf::Text("NOUVELLE SAUVEGARDE", *font, 35)));
		Parent.back().setOrigin().CenterText();
		Parent.back().setColorState(sf::Color(30, 127, 203, 180), sf::Color::Red, sf::Color::Black);
		Parent.back().CreateFunctionCall([this] {
			if (state == State::Save) {
				input_text.clear();
				Emplace_Map.clear();
				state = State::New_Save;
			}
			else if (state == State::New_Save) {
				if (input_text.size() > 0) {
					Map.AddMap(input_text);
					log.saveMap(input_text, tm, Nbr_Screen);
					Map.SaveMap();
					input_text.clear();

					state = State::Editor;
					if (selectedLayer == 1 || selectedLayer == 2) {
						Tiles_Select = Tiles("TilesE", sf::Vector2f(sf::Mouse::getPosition()), { 128,128 }, { 64,64 });
						Tiles_Select.getShape().setOrigin(32, 32);
					}
					else {
						Tiles_Select = Tiles("Cables", sf::Vector2f(sf::Mouse::getPosition()));
					}
				}
			}
			});

		Parent.push_back(Button(nullptr, RoundedRectangleShape({ 300,35 }, 2.f, 2, sf::Color(30, 127, 203, 180),
			sf::Color(0, 0, 0, 255), 2.f, sf::Vector2f(960, 940)), sf::Text("RETOUR", *font, 35)));
		Parent.back().setOrigin().CenterText();
		Parent.back().setColorState(sf::Color(30, 127, 203, 180), sf::Color::Red, sf::Color::Black);
		Parent.back().CreateFunctionCall([this] {
			if (state == State::Save) {
				Emplace_Map.clear();
			}
			else if (state == State::New_Save) {
				input_text.clear();
			}
			state = State::Editor;
			if (selectedLayer == 1 || selectedLayer == 2) {
				Tiles_Select = Tiles("TilesE", sf::Vector2f(sf::Mouse::getPosition()), { 128,128 }, { 64,64 });
				Tiles_Select.getShape().setOrigin(32, 32);
			}
			else {
				Tiles_Select = Tiles("Cables", sf::Vector2f(sf::Mouse::getPosition()));
			}
			});

		state = State::Save;
		});

	m_buttons.insert(std::make_pair("Bouclier"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("BonusBouclier"))));
	m_buttons["Bouclier"_hash].setOrigin({ m_buttons["Bouclier"_hash].getGlobalBounds().width / 2,m_buttons["Bouclier"_hash].getGlobalBounds().height / 2 });

	m_buttons.insert(std::make_pair("Special"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("BonusEpee"))));
	m_buttons["Special"_hash].setOrigin({ m_buttons["Special"_hash].getGlobalBounds().width / 2,m_buttons["Special"_hash].getGlobalBounds().height / 2 });

	m_buttons.insert(std::make_pair("montre1"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("montre1"))));
	m_buttons["montre1"_hash].setOrigin({ m_buttons["montre1"_hash].getGlobalBounds().width / 2,m_buttons["montre1"_hash].getGlobalBounds().height / 2 });

	m_buttons.insert(std::make_pair("montre2"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("montre2"))));
	m_buttons["montre2"_hash].setOrigin({ m_buttons["montre2"_hash].getGlobalBounds().width / 2,m_buttons["montre2"_hash].getGlobalBounds().height / 2 });

	m_buttons.insert(std::make_pair("montre3"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("montre3"))));
	m_buttons["montre3"_hash].setOrigin({ m_buttons["montre3"_hash].getGlobalBounds().width / 2,m_buttons["montre3"_hash].getGlobalBounds().height / 2 });


	m_buttons.insert(std::make_pair("Araigner"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("araignerI"))));
	m_buttons["Araigner"_hash].setOrigin({ m_buttons["Araigner"_hash].getGlobalBounds().width / 2,m_buttons["Araigner"_hash].getGlobalBounds().height / 2 });

	m_buttons.insert(std::make_pair("Drone"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("DroneI"))));
	m_buttons["Drone"_hash].setOrigin({ m_buttons["Drone"_hash].getGlobalBounds().width / 2,m_buttons["Drone"_hash].getGlobalBounds().height / 2 });

	m_buttons.insert(std::make_pair("GardeI"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("GardeI"))));
	m_buttons["GardeI"_hash].setOrigin({ m_buttons["GardeI"_hash].getGlobalBounds().width / 2,m_buttons["GardeI"_hash].getGlobalBounds().height / 2 });

	m_buttons.insert(std::make_pair("BossI"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("BossI"))));
	m_buttons["BossI"_hash].setOrigin({ m_buttons["BossI"_hash].getGlobalBounds().width / 2,m_buttons["BossI"_hash].getGlobalBounds().height / 2 });

	m_buttons.insert(std::make_pair("icone_player"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("icone_player"))));
	m_buttons["icone_player"_hash].setOrigin({ m_buttons["icone_player"_hash].getGlobalBounds().width / 2,m_buttons["icone_player"_hash].getGlobalBounds().height / 2 });

	m_buttons.insert(std::make_pair("FinNiveau"_hash, sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>("FinNiveau"))));
	m_buttons["FinNiveau"_hash].setOrigin({ m_buttons["FinNiveau"_hash].getGlobalBounds().width / 2,m_buttons["FinNiveau"_hash].getGlobalBounds().height / 2 });
}

EditorState::~EditorState()
{
	if (threading.joinable())
		threading.join();
}

void EditorState::GoBackPreviousState()
{
}

void EditorState::update(const float& dt)
{
	if (!Check_Mutex) {
		if (state != State::Play) {
			text.at("ScreenX"_hash).setString(std::to_string(Nbr_Screen.x));
			text.at("ScreenY"_hash).setString(std::to_string(Nbr_Screen.y));
			text.at("ScreenX"_hash).setOrigin(text.at("ScreenX"_hash).getGlobalBounds().width / 2, text.at("ScreenX"_hash).getGlobalBounds().height / 2);
			text.at("ScreenY"_hash).setOrigin(text.at("ScreenY"_hash).getGlobalBounds().width / 2, text.at("ScreenY"_hash).getGlobalBounds().height / 2);

			if (sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheHori)) < -30 ||
				sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheHori)) > 30 ||
				sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheVert)) < -30 ||
				sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheVert)) > 30)
				sf::Mouse::setPosition(sf::Vector2i(sf::Mouse::getPosition().x + sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheHori)) * 7 * dt,
					sf::Mouse::getPosition().y + sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheVert)) * 7 * dt));

			if (state == State::Editor || state == State::Collectible || state == State::Set_Enemy || state == State::Traps) {
				//selecting shape by click on it
				if (static_cast<sf::Vector2f>(sf::Mouse::getPosition()).y > rounded.at("backFond"_hash).getPosition().y + rounded.at("backFond"_hash).getSize().y - 50 &&
					window->mapPixelToCoords(sf::Mouse::getPosition(), view).x > 0 && window->mapPixelToCoords(sf::Mouse::getPosition(), view).y > 0 &&
					window->mapPixelToCoords(sf::Mouse::getPosition(), view).x < Nbr_Screen.x * 1920 && window->mapPixelToCoords(sf::Mouse::getPosition(), view).y < Nbr_Screen.y * 1080 &&
					!Move.GlobalBounds().intersects(Tiles_Select.getShape().getGlobalBounds())) {
					if ((sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Joystick::isButtonPressed(0, 0)) && BlockPose) {
						if (state == State::Editor) {
							if (selectedLayer == 1 || selectedLayer == 2) {
								CheckLayersForTiles();
								if (findMap == false) {
									tm.AddTiles(Tiles(Tiles_Select.GetFilepath(), sf::Vector2f((static_cast<int>(window->mapPixelToCoords(sf::Mouse::getPosition(), view).x / 64)) * 64,
										(static_cast<int>(window->mapPixelToCoords(sf::Mouse::getPosition(), view).y / 64)) * 64),
										Tiles_Select.GetRect(), { 64,64 }), static_cast<TilesMap::layers>(selectedLayer));
								}
							}
							else {
								tm.AddTiles(Tiles(Tiles_Select.GetFilepath(), window->mapPixelToCoords(sf::Mouse::getPosition(), view)), static_cast<TilesMap::layers>(selectedLayer));
							}
						}
						if (state == State::Traps) {
							CheckTraps();
							if (findMap == false) {
								tm.AddTrap(Tiles(Tiles_Select.GetFilepath(), sf::Vector2f((static_cast<int>(window->mapPixelToCoords(sf::Mouse::getPosition(), view).x / 64)) * 64,
									(static_cast<int>(window->mapPixelToCoords(sf::Mouse::getPosition(), view).y / 64)) * 64),
									Tiles_Select.GetRect(), { 64,64 }));
							}
						}
						else if (state == State::Collectible) {
							if (collectible == Collect::Montre) {
								if (tm.Montre.size() < 3) {
									tm.Montre.push_back(window->mapPixelToCoords(sf::Mouse::getPosition(), view));
									BlockPose = false;
								}
								if (tm.Montre.size() > 2) {
									Tiles_Select = Tiles("TilesE", sf::Vector2f(sf::Mouse::getPosition()), { 128,128 }, { 64,64 });
									Tiles_Select.getShape().setOrigin(32, 32);

									state = State::Editor;
								}
							}
							else if (collectible == Collect::Bouclier) {
								tm.Bouclier.push_back(window->mapPixelToCoords(sf::Mouse::getPosition(), view));
								BlockPose = false;
							}
							else if (collectible == Collect::Special) {
								tm.Special.push_back(window->mapPixelToCoords(sf::Mouse::getPosition(), view));
								BlockPose = false;
							}
						}
						else if (state == State::Set_Enemy) {
							switch (Enemies)
							{
							case Enemy::Garde:
								tm.Garde.push_back(window->mapPixelToCoords(sf::Mouse::getPosition(), view));
								BlockPose = false;
								break;
							case Enemy::Drone:
								tm.Drone.push_back(window->mapPixelToCoords(sf::Mouse::getPosition(), view));
								BlockPose = false;
								break;
							case Enemy::Spider:
								tm.Araigne.push_back(window->mapPixelToCoords(sf::Mouse::getPosition(), view));
								BlockPose = false;
								break;
							case Enemy::Boss:
								Tiles_Select = Tiles("TilesE", sf::Vector2f(sf::Mouse::getPosition()), { 128,128 }, { 64,64 });
								Tiles_Select.getShape().setOrigin(32, 32);
								state = State::Editor;
								BlockPose = false;
								break;
							default:
								break;
							}
						}
					}
					else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) || sf::Joystick::isButtonPressed(0, 1)) {
						if (state == State::Editor) {
							tm.RemoveTiles(window->mapPixelToCoords(sf::Mouse::getPosition(), view), static_cast<TilesMap::layers>(selectedLayer));
						}
						if (state == State::Traps) {
							tm.RemoveTrap(window->mapPixelToCoords(sf::Mouse::getPosition(), view));
						}
						else if (state == State::Collectible) {
							if (collectible == Collect::Montre) {
								tm.RemoveVector(window->mapPixelToCoords(sf::Mouse::getPosition(), view), tm.Montre);
							}
							else if (collectible == Collect::Bouclier) {
								tm.RemoveVector(window->mapPixelToCoords(sf::Mouse::getPosition(), view), tm.Bouclier);
							}
							else if (collectible == Collect::Special) {
								tm.RemoveVector(window->mapPixelToCoords(sf::Mouse::getPosition(), view), tm.Special);
							}
						}
						else if (state == State::Set_Enemy) {
							switch (Enemies)
							{
							case Enemy::Garde:
								tm.RemoveVector(window->mapPixelToCoords(sf::Mouse::getPosition(), view), tm.Garde);
								break;
							case Enemy::Drone:
								tm.RemoveVector(window->mapPixelToCoords(sf::Mouse::getPosition(), view), tm.Drone);
								break;
							case Enemy::Spider:
								tm.RemoveVector(window->mapPixelToCoords(sf::Mouse::getPosition(), view), tm.Araigne);
								break;
							case Enemy::Boss:
								if (func::Circle_Collision(tm.Boss, window->mapPixelToCoords(sf::Mouse::getPosition(), view), 150, 150))
									tm.Boss = { -5000,-5000 };
								break;
							default:
								break;
							}
						}
					}
				}

				if (state == State::Set_Enemy && Enemies == Enemy::Boss) {
					tm.Boss = window->mapPixelToCoords(sf::Mouse::getPosition(), view);
				}

				if ((!sf::Mouse::isButtonPressed(sf::Mouse::Left) && !sf::Joystick::isButtonPressed(0, 0))) {
					BlockPose = true;
				}

				if (sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(5)) < -30) {
					Zoom = true;
					Dezoom = false;
				}
				else if (sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(5)) > 30) {
					Zoom = false;
					Dezoom = true;
				}

				if (Zoom == true) {
					view.setSize(view.getSize() + sf::Vector2f(1920 * dt, 1080 * dt));
					Zoom = false;
					Dezoom = false;
				}
				else if (Dezoom == true) {
					view.setSize(view.getSize() + sf::Vector2f(-1920 * dt, -1080 * dt));
					Zoom = false;
					Dezoom = false;
				}
			}
			else if (state == State::Save) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Joystick::isButtonPressed(0, 0)) {
					for (auto& Current : Emplace_Map) {
						if (Current.second.second.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition()))) {
							log.saveMap(Current.second.first.getString(), tm, Nbr_Screen);
							Emplace_Map.clear();
							state = State::Editor;
							if (selectedLayer == 1 || selectedLayer == 2) {
								Tiles_Select = Tiles("TilesE", sf::Vector2f(sf::Mouse::getPosition()), { 128,128 }, { 64,64 });
								Tiles_Select.getShape().setOrigin(32, 32);
							}
							else {
								Tiles_Select = Tiles("Cables", sf::Vector2f(sf::Mouse::getPosition()), { 0,0 }, { 0,0 });
								Tiles_Select.getShape().setOrigin(Tiles_Select.getShape().getGlobalBounds().width / 2, Tiles_Select.getShape().getGlobalBounds().height / 2);
							}
							break;
						}
					}
				}
			}
			else if (state == State::New_Save) {
				static float timerCursor{ 0.f };
				timerCursor += dt;
				if (timerCursor >= 0.5f) {
					cursorText = !cursorText;
					timerCursor = 0.f;
				}

				static float timerSaisie{ 0.f };
				timerSaisie += dt;

				if (sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(5)) > 30 && timerSaisie > 0.2f) {
					if (Joystick == false) Joystick = true;
					Caracter--;
					if (Caracter < 'a' && Caracter > 'Z') { Caracter = 'Z'; }
					if (Caracter < 'A') { Caracter = 'z'; }

					timerSaisie = 0;
				}
				if (sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(5)) < -30 && timerSaisie > 0.2f) {
					if (Joystick == false) Joystick = true;
					Caracter++;
					if (Caracter > 'Z' && Caracter < 'a') { Caracter = 'a'; }
					if (Caracter > 'z') { Caracter = 'A'; }

					timerSaisie = 0;
				}
				if (sf::Joystick::isButtonPressed(0, 0) && timerSaisie > 0.2f && !Parent.at(0).MouseInButton() && Joystick) {
					input_text += Caracter;
					timerSaisie = 0;
				}
				if (sf::Joystick::isButtonPressed(0, 1) && timerSaisie > 0.2f && Joystick) {
					if (!input_text.empty())
						input_text.pop_back();

					timerSaisie = 0;
				}
			}
			else if (state == State::Load) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Joystick::isButtonPressed(0, 0)) {
					for (auto& Current : Emplace_Map) {
						if (Current.second.second.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition()))) {
							log.loadMap(Current.second.first.getString(), tm, Nbr_Screen);
							Emplace_Map.clear();
							state = State::Editor;
							if (selectedLayer == 1 || selectedLayer == 2) {
								Tiles_Select = Tiles("TilesE", sf::Vector2f(sf::Mouse::getPosition()), { 128,128 }, { 64,64 });
								Tiles_Select.getShape().setOrigin(32, 32);
							}
							else {
								Tiles_Select = Tiles("Cables", sf::Vector2f(sf::Mouse::getPosition()), { 0,0 }, { 0,0 });
								Tiles_Select.getShape().setOrigin(Tiles_Select.getShape().getGlobalBounds().width / 2, Tiles_Select.getShape().getGlobalBounds().height / 2);
							}
							break;
						}
					}
				}
			}
			else if (state == State::Set_Start) {
				if (static_cast<sf::Vector2f>(sf::Mouse::getPosition()).y > rounded.at("backFond"_hash).getPosition().y + rounded.at("backFond"_hash).getSize().y + 50 &&
					window->mapPixelToCoords(sf::Mouse::getPosition(), view).x > 0 && window->mapPixelToCoords(sf::Mouse::getPosition(), view).y > 0 &&
					window->mapPixelToCoords(sf::Mouse::getPosition(), view).x < Nbr_Screen.x * 1920 && window->mapPixelToCoords(sf::Mouse::getPosition(), view).y < Nbr_Screen.y * 1080) {
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Joystick::isButtonPressed(0, 0)) {
						state = State::Editor;
						if (selectedLayer == 1 || selectedLayer == 2) {
							Tiles_Select = Tiles("TilesE", sf::Vector2f(sf::Mouse::getPosition()), { 128,128 }, { 64,64 });
							Tiles_Select.getShape().setOrigin(32, 32);
						}
						else {
							Tiles_Select = Tiles("Cables", sf::Vector2f(sf::Mouse::getPosition()), { 0,0 }, { 0,0 });
							Tiles_Select.getShape().setOrigin(Tiles_Select.getShape().getGlobalBounds().width / 2, Tiles_Select.getShape().getGlobalBounds().height / 2);
						}
					}
				}

				tm.Startpos = window->mapPixelToCoords(sf::Mouse::getPosition(), view);
			}
			else if (state == State::Set_End) {
				if (static_cast<sf::Vector2f>(sf::Mouse::getPosition()).y > rounded.at("backFond"_hash).getPosition().y + rounded.at("backFond"_hash).getSize().y + 50 &&
					window->mapPixelToCoords(sf::Mouse::getPosition(), view).x > 0 && window->mapPixelToCoords(sf::Mouse::getPosition(), view).y > 0 &&
					window->mapPixelToCoords(sf::Mouse::getPosition(), view).x < Nbr_Screen.x * 1920 && window->mapPixelToCoords(sf::Mouse::getPosition(), view).y < Nbr_Screen.y * 1080) {
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Joystick::isButtonPressed(0, 0)) {
						state = State::Editor;
						if (selectedLayer == 1 || selectedLayer == 2) {
							Tiles_Select = Tiles("TilesE", sf::Vector2f(sf::Mouse::getPosition()), { 128,128 }, { 64,64 });
							Tiles_Select.getShape().setOrigin(32, 32);
						}
						else {
							Tiles_Select = Tiles("Cables", sf::Vector2f(sf::Mouse::getPosition()), { 0,0 }, { 0,0 });
							Tiles_Select.getShape().setOrigin(Tiles_Select.getShape().getGlobalBounds().width / 2, Tiles_Select.getShape().getGlobalBounds().height / 2);
						}
					}
				}

				tm.Endpos = window->mapPixelToCoords(sf::Mouse::getPosition(), view);
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Right) || sf::Joystick::isButtonPressed(0, 1)) {
				if (m_buttons.find("TilesMenu"_hash) != m_buttons.end()) {
					m_buttons.erase(m_buttons.find("TilesMenu"_hash));
					Select_Tiles.Children_Clear();
					state = State::Editor;
					if (selectedLayer == 1 || selectedLayer == 2) {
						Tiles_Select = Tiles("TilesE", sf::Vector2f(sf::Mouse::getPosition()), { 128,128 }, { 64,64 });
						Tiles_Select.getShape().setOrigin(32, 32);
					}
					else {
						Tiles_Select = Tiles("Cables", sf::Vector2f(sf::Mouse::getPosition()), { 0,0 }, { 0,0 });
						Tiles_Select.getShape().setOrigin(Tiles_Select.getShape().getGlobalBounds().width / 2, Tiles_Select.getShape().getGlobalBounds().height / 2);
					}
				}
			}

			if (func::IsDirectionPressed(sf::Keyboard::Key::LControl, func::PadAxis::JoystickGachettes, 30)) {
				//if (view.getCenter().x > 960 && view.getCenter().x < Nbr_Screen.x * 1920) {
				view.setCenter(view.getCenter() + lastpass - sf::Vector2f(sf::Mouse::getPosition()));
				//}
				lastpass = sf::Vector2f(sf::Mouse::getPosition());
			}
			else {
				lastpass = sf::Vector2f(sf::Mouse::getPosition());
			}

			tm.Update({ 0,0 }, dt, view);
			Tiles_Select.setPos(sf::Vector2f(sf::Mouse::getPosition()));
		}
		else {
			ga.update(dt);

			if (ga.Return) {
				if (selectedLayer == 1 || selectedLayer == 2) {
					Tiles_Select = Tiles("TilesE", sf::Vector2f(sf::Mouse::getPosition()), { 128,128 }, { 64,64 });
					Tiles_Select.getShape().setOrigin(32, 32);
				}
				else {
					Tiles_Select = Tiles("Cables", sf::Vector2f(sf::Mouse::getPosition()), { 0,0 }, { 0,0 });
					Tiles_Select.getShape().setOrigin(Tiles_Select.getShape().getGlobalBounds().width / 2, Tiles_Select.getShape().getGlobalBounds().height / 2);
				}
				state = State::Editor;
				return;
			}

			if (ga.Reset) {
				ga.Restart();
				return;
			}

			if (ga.ReturnAfterLose) {
				ga.Restart();
				return;
			}

			if (ga.ReturnMenuAfterLose) {
				state = State::Editor;
				return;
			}

			if (ga.NewScoreToScoreState) {
				if (threading.joinable())
					threading.join();

				threading = std::thread([this] {
					Check_Mutex.lock();
					ptr = FSMManager::create<ScoreState>(*fsmManager, *window, *font, ScoreState::Return::Editor, false);
					Check_Mutex.unlock();
					});
				return;
			}
		}

		if (state != State::Editor && state != State::Collectible && state != State::Set_Enemy && state != State::Traps) {
			if (BlockPose) {
				BlockPose = false;
			}
		}
	}
	else {
		m_loadingScreen.at("g1"_hash).rotate(200 * dt);
		m_loadingScreen.at("g2"_hash).rotate(-150 * dt);
		m_loadingScreen.at("g3"_hash).rotate(110 * dt);
		m_loadingScreen.at("g4"_hash).rotate(60 * dt);
		timerCursor += dt;
	}
}

void EditorState::processEvents(const sf::Event& e, const float& dt)
{
	if (!Check_Mutex) {
		if (state != State::Play) {
			switch (e.type)
			{
			case sf::Event::TextEntered: {
				if (std::isprint(e.text.unicode)) {
					if (Joystick == true)
						Joystick = false;
					input_text += e.text.unicode;
				}
				break;
			}
			case sf::Event::KeyPressed: {
				switch (e.key.code) {
				case sf::Keyboard::Key::BackSpace: {
					if (!input_text.empty())
						input_text.pop_back();
					break;
				}
				case sf::Keyboard::Key::Enter: {
					if (state == State::New_Save) {
						Map.AddMap(input_text);
						log.saveMap(input_text, tm, Nbr_Screen);
						input_text.clear();
					}
					break;
				}
				default:
					break;
				}
				break;
			}
			case sf::Event::MouseWheelScrolled: {
				switch (e.mouseWheelScroll.wheel) {
				case sf::Mouse::VerticalWheel: {
					if (e.mouseWheelScroll.delta < 0) {
						Zoom = true;
						Dezoom = false;
					}
					else if (e.mouseWheelScroll.delta > 0) {
						Zoom = false;
						Dezoom = true;
					}
					break;
				}
				default:
					break;
				}
				break;
			}
			default:
				break;
			}

			if (!HideMenu) {
				button.processEvents(e);
				Select_Tiles.processEvents(e);
			}
			Move.processEvents(e);

			if (state == State::Save || state == State::Load || state == State::New_Save)
				for (auto& Current : Parent)
					Current.processEvents(e);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Joystick::isButtonPressed(0, 0)) {
				if (m_buttons.find("TilesMenu"_hash) != m_buttons.end()) {
					if (!m_buttons["TilesMenu"_hash].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition()))) {
						m_buttons.erase(m_buttons.find("TilesMenu"_hash));
						Select_Tiles.Children_Clear();
						state = State::Editor;
					}
				}
			}
		}
		else {
			ga.processEvents(e, dt);
		}
	}
}

void EditorState::draw(sf::RenderWindow& window)
{
	if (!Check_Mutex) {
		if (state != State::Play) {
			tm.Draw_BackGround(window);

			window.setView(view);

			tm.Draw_BackPlan(window);

			tm.draw(window, TilesMap::layers::back);
			tm.draw(window, TilesMap::layers::player);
			tm.draw(window, TilesMap::layers::deco);
			tm.drawTraps(window, &view);
			drawCollectible();
			drawEnnemies();
			tm.draw(window, TilesMap::layers::front);
			tm.Draw_FrontPlan(window);

			window.draw(func::GridOfBox(0, 1920 * Nbr_Screen.x, 1080 * Nbr_Screen.y, 64, { 0, 0, 0, 255 }));

			m_buttons["icone_player"_hash].setPosition(tm.Startpos);
			window.draw(m_buttons["icone_player"_hash]);

			m_buttons["FinNiveau"_hash].setPosition(tm.Endpos);
			window.draw(m_buttons["FinNiveau"_hash]);

			window.setView(window.getDefaultView());
			window.draw(rounded.at("backFond"_hash));

			if (!HideMenu) {
				window.draw(rounded.at("InfoScreenX"_hash));
				window.draw(rounded.at("InfoScreenY"_hash));
				window.draw(text.at("ScreenX"_hash));
				window.draw(text.at("ScreenY"_hash));

				if (rounded.at("InfoScreenX"_hash).getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition()))) {

					sf::Text text = func::CreateText("NOMBRES D'ECRANS EN X", *font, 20, sf::Vector2f(sf::Mouse::getPosition().x - 65, sf::Mouse::getPosition().y + 37), sf::Color::White);
					RoundedRectangleShape rOne({ text.getGlobalBounds().width + 10,text.getGlobalBounds().height + 10 }, 2.f, 20, { 30, 127, 203, 180 }, { 30, 127, 203, 180 }, 2.f, { (float)sf::Mouse::getPosition().x + 50.f, (float)sf::Mouse::getPosition().y + 50.f });
					rOne.setOrigin(rOne.getGlobalBounds().width / 2, rOne.getGlobalBounds().height / 2);
					window.draw(rOne);
					window.draw(text);
				}
				if (rounded.at("InfoScreenY"_hash).getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition()))) {
					sf::Text text2 = func::CreateText("NOMBRES D'ECRANS EN Y", *font, 20, sf::Vector2f(sf::Mouse::getPosition().x - 65, sf::Mouse::getPosition().y + 37), sf::Color::White);
					RoundedRectangleShape rTwo({ text2.getGlobalBounds().width + 10,text2.getGlobalBounds().height + 10 }, 2.f, 20, { 30, 127, 203, 180 }, { 30, 127, 203, 180 }, 2.f, { (float)sf::Mouse::getPosition().x + 50.f, (float)sf::Mouse::getPosition().y + 50.f });
					rTwo.setOrigin(rTwo.getGlobalBounds().width / 2, rTwo.getGlobalBounds().height / 2);
					window.draw(rTwo);
					window.draw(text2);
				}

				window.draw(m_buttons.at("Font_Layer"_hash));
				drawlayer();

				if (m_buttons.find("TilesMenu"_hash) != m_buttons.end()) {
					window.draw(m_buttons.at("TilesMenu"_hash));
				}

				button.drawEditor(window);
				Select_Tiles.drawEditor(window);
			}

			Move.drawEditor(window);

			if (state == State::Save || state == State::Load) {
				for (auto& Current : Parent) {
					Current.draw(window);
				}
				for (auto& Current : Emplace_Map) {
					window.draw(Current.second.second);
					window.draw(Current.second.first);
				}
			}
			if (state == State::New_Save) {
				for (auto& Current : Parent) {
					Current.draw(window);
				}

				sf::Text tmp;
				if (Joystick) tmp = func::CreateText(input_text + Caracter + (cursorText ? '|' : ' '), *font, 35, { 960, 540 }, sf::Color::Black);
				else tmp = func::CreateText(input_text + (cursorText ? '|' : ' '), *font, 35, { 960, 540 }, sf::Color::Black);

				tmp.setOrigin(tmp.getLocalBounds().left + tmp.getLocalBounds().width / 2, 25);
				sf::RectangleShape tmp2 = func::rectangle({ 960,520 }, { 428,108 }, sf::Color::White,
					sf::Color::White, 0.f);
				tmp2.setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>("encadrement_nom"));
				tmp2.setOrigin(tmp2.getGlobalBounds().width / 2, tmp2.getGlobalBounds().height / 2);
				window.draw(tmp2);
				window.draw(tmp);
			}

			if (static_cast<sf::Vector2f>(sf::Mouse::getPosition()).y > rounded.at("backFond"_hash).getPosition().y + rounded.at("backFond"_hash).getSize().y - 50 && !Move.MouseInButton()
				&& state != State::Set_Start) {
				Tiles_Select.draw(window);
			}
		}
		else {
			ga.draw(window);
		}
	}
	else {
		window.draw(m_fond_d);
		func::drawUnorderMap(m_loadingScreen, &window);
		window.draw(m_fond);
		window.draw(textL);

		if (timerCursor >= 0.5f)
			window.draw(std::get<0>(m_points));
		if (timerCursor >= 1.f)
			window.draw(std::get<1>(m_points));
		if (timerCursor >= 1.5f)
			window.draw(std::get<2>(m_points));
		if (timerCursor >= 2.f)
			timerCursor = 0.f;
	}
}

void EditorState::CheckLayersForTiles()
{
	findMap = false;
	for (auto& Current_Map : tm.GetLayer(static_cast<TilesMap::layers>(selectedLayer))) {
		if (Current_Map.getShape().getGlobalBounds().contains(window->mapPixelToCoords(sf::Mouse::getPosition(), view))) {
			Current_Map = Tiles(Tiles_Select.GetFilepath(), Current_Map.GetPos(), Tiles_Select.GetRect(),
				sf::Vector2i(Tiles_Select.getShape().getGlobalBounds().width, Tiles_Select.getShape().getGlobalBounds().height));
			findMap = true;
			break;
		}
	}
}

void EditorState::Load_Tiles()
{
	Button& Tile_E = Select_Tiles.Add("TilesE"_hash, func::rectangle({ 480,540 }, { 0,0 },
		sf::Color::White, sf::Color::White, 0.f));
	Tile_E.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("TilesE")).setTextureState("TilesE", "TilesE", "TilesE").Resize().setOrigin();

	Tile_E.CreateFunctionCall([this] {
		sf::Vector2i MousePos = sf::Mouse::getPosition();
		sf::Vector2i PosBouton = sf::Vector2i(
			Select_Tiles.get("TilesE"_hash).Get_Position().x - Select_Tiles.get("TilesE"_hash).GlobalBounds().width / 2,
			Select_Tiles.get("TilesE"_hash).Get_Position().y - Select_Tiles.get("TilesE"_hash).GlobalBounds().height / 2);

		Tiles_Select = Tiles("TilesE", sf::Vector2f(MousePos), sf::Vector2i(((MousePos.x - PosBouton.x) / 64) * 64, ((MousePos.y - PosBouton.y) / 64) * 64), { 64,64 });
		Tiles_Select.getShape().setOrigin(32, 32);

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});

	Button& Tile_V = Select_Tiles.Add("TilesV"_hash, func::rectangle({ 960,540 }, { 0,0 },
		sf::Color::White, sf::Color::White, 0.f));
	Tile_V.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("TilesV")).setTextureState("TilesV", "TilesV", "TilesV").Resize().setOrigin();

	Tile_V.CreateFunctionCall([this] {
		sf::Vector2i MousePos = sf::Mouse::getPosition();
		sf::Vector2i PosBouton = sf::Vector2i(
			Select_Tiles.get("TilesV"_hash).Get_Position().x - Select_Tiles.get("TilesV"_hash).GlobalBounds().width / 2,
			Select_Tiles.get("TilesV"_hash).Get_Position().y - Select_Tiles.get("TilesV"_hash).GlobalBounds().height / 2);

		Tiles_Select = Tiles("TilesV", sf::Vector2f(MousePos), sf::Vector2i(((MousePos.x - PosBouton.x) / 64) * 64, ((MousePos.y - PosBouton.y) / 64) * 64), { 64,64 });
		Tiles_Select.getShape().setOrigin(32, 32);

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});

	Button& Tile_S = Select_Tiles.Add("TilesS"_hash, func::rectangle({ 1440,540 }, { 0,0 },
		sf::Color::White, sf::Color::White, 0.f));
	Tile_S.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("TilesS")).setTextureState("TilesS", "TilesS", "TilesS").Resize().setOrigin();

	Tile_S.CreateFunctionCall([this] {
		sf::Vector2i MousePos = sf::Mouse::getPosition();
		sf::Vector2i PosBouton = sf::Vector2i(
			Select_Tiles.get("TilesS"_hash).Get_Position().x - Select_Tiles.get("TilesS"_hash).GlobalBounds().width / 2,
			Select_Tiles.get("TilesS"_hash).Get_Position().y - Select_Tiles.get("TilesS"_hash).GlobalBounds().height / 2);

		Tiles_Select = Tiles("TilesS", sf::Vector2f(MousePos), sf::Vector2i(((MousePos.x - PosBouton.x) / 64) * 64, ((MousePos.y - PosBouton.y) / 64) * 64), { 64,64 });
		Tiles_Select.getShape().setOrigin(32, 32);

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});
}

void EditorState::Load_Deco()
{
	Button& Tile_E = Select_Tiles.Add("TilesDecorE"_hash, func::rectangle({ 480,540 }, { 0,0 },
		sf::Color::White, sf::Color::White, 0.f));
	Tile_E.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("TilesDecoE")).setTextureState("TilesDecoE", "TilesDecoE", "TilesDecoE").Resize().setOrigin();

	Tile_E.CreateFunctionCall([this] {
		sf::Vector2i MousePos = sf::Mouse::getPosition();
		sf::Vector2i PosBouton = sf::Vector2i(
			Select_Tiles.get("TilesDecorE"_hash).Get_Position().x - Select_Tiles.get("TilesDecorE"_hash).GlobalBounds().width / 2,
			Select_Tiles.get("TilesDecorE"_hash).Get_Position().y - Select_Tiles.get("TilesDecorE"_hash).GlobalBounds().height / 2);

		Tiles_Select = Tiles("TilesDecoE", sf::Vector2f(MousePos), sf::Vector2i(((MousePos.x - PosBouton.x) / 64) * 64, ((MousePos.y - PosBouton.y) / 64) * 64), { 64,64 });
		Tiles_Select.getShape().setOrigin(32, 32);

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});

	Button& Tile_V = Select_Tiles.Add("TilesDecorV"_hash, func::rectangle({ 960,540 }, { 0,0 },
		sf::Color::White, sf::Color::White, 0.f));
	Tile_V.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("TilesDecoV")).setTextureState("TilesDecoV", "TilesDecoV", "TilesDecoV").Resize().setOrigin();

	Tile_V.CreateFunctionCall([this] {
		sf::Vector2i MousePos = sf::Mouse::getPosition();
		sf::Vector2i PosBouton = sf::Vector2i(
			Select_Tiles.get("TilesDecorV"_hash).Get_Position().x - Select_Tiles.get("TilesDecorV"_hash).GlobalBounds().width / 2,
			Select_Tiles.get("TilesDecorV"_hash).Get_Position().y - Select_Tiles.get("TilesDecorV"_hash).GlobalBounds().height / 2);

		Tiles_Select = Tiles("TilesDecoV", sf::Vector2f(MousePos), sf::Vector2i(((MousePos.x - PosBouton.x) / 64) * 64, ((MousePos.y - PosBouton.y) / 64) * 64), { 64,64 });
		Tiles_Select.getShape().setOrigin(32, 32);

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});
}

void EditorState::Load_Asset()
{
	Button& Asset1 = Select_Tiles.Add("Asset1"_hash, func::rectangle({ 1920 / 7,470 }, { 64,64 },
		sf::Color::White, sf::Color::White, 0.f));
	Asset1.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("Cables")).setTextureState("Cables", "Cables", "Cables").setOrigin();

	Asset1.CreateFunctionCall([this] {
		Tiles_Select = Tiles("Cables", sf::Vector2f(sf::Mouse::getPosition()));

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});

	Button& Asset2 = Select_Tiles.Add("Asset2"_hash, func::rectangle({ 1920 / 7 * 2,470 }, { 64,64 },
		sf::Color::White, sf::Color::White, 0.f));
	Asset2.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("Valve")).setTextureState("Valve", "Valve", "Valve").setOrigin();

	Asset2.CreateFunctionCall([this] {
		Tiles_Select = Tiles("Valve", sf::Vector2f(sf::Mouse::getPosition()));

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});

	Button& Asset3 = Select_Tiles.Add("Asset3"_hash, func::rectangle({ 1920 / 7 * 3,470 }, { 64,64 },
		sf::Color::White, sf::Color::White, 0.f));
	Asset3.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("RembardeE")).setTextureState("RembardeE", "RembardeE", "RembardeE").setOrigin();

	Asset3.CreateFunctionCall([this] {
		Tiles_Select = Tiles("RembardeE", sf::Vector2f(sf::Mouse::getPosition()));

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});

	Button& Asset4 = Select_Tiles.Add("Asset4"_hash, func::rectangle({ 1920 / 7 * 4,470 }, { 64,64 },
		sf::Color::White, sf::Color::White, 0.f));
	Asset4.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("PoteauE")).setTextureState("PoteauE", "PoteauE", "PoteauE").setOrigin();

	Asset4.CreateFunctionCall([this] {
		Tiles_Select = Tiles("PoteauE", sf::Vector2f(sf::Mouse::getPosition()));

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});

	Button& Asset5 = Select_Tiles.Add("Asset5"_hash, func::rectangle({ 1920 / 7 * 5,470 }, { 64,64 },
		sf::Color::White, sf::Color::White, 0.f));
	Asset5.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("Pompe")).setTextureState("Pompe", "Pompe", "Pompe").setOrigin();

	Asset5.CreateFunctionCall([this] {
		Tiles_Select = Tiles("Pompe", sf::Vector2f(sf::Mouse::getPosition()));

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});

	Button& Asset6 = Select_Tiles.Add("Asset6"_hash, func::rectangle({ 1920 / 7 * 6,470 }, { 64,64 },
		sf::Color::White, sf::Color::White, 0.f));
	Asset6.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("Poubelle")).setTextureState("Poubelle", "Poubelle", "Poubelle").setOrigin();

	Asset6.CreateFunctionCall([this] {
		Tiles_Select = Tiles("Poubelle", sf::Vector2f(sf::Mouse::getPosition()));

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});

	Button& Asset7 = Select_Tiles.Add("Asset7"_hash, func::rectangle({ 1920 / 7,540 }, { 64,64 },
		sf::Color::White, sf::Color::White, 0.f));
	Asset7.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("DomeVerre")).setTextureState("DomeVerre", "DomeVerre", "DomeVerre").setOrigin();

	Asset7.CreateFunctionCall([this] {
		Tiles_Select = Tiles("DomeVerre", sf::Vector2f(sf::Mouse::getPosition()));

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});

	Button& Asset8 = Select_Tiles.Add("Asset8"_hash, func::rectangle({ 1920 / 7 * 2,540 }, { 64,64 },
		sf::Color::White, sf::Color::White, 0.f));
	Asset8.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("PoteauV")).setTextureState("PoteauV", "PoteauV", "PoteauV").setOrigin();

	Asset8.CreateFunctionCall([this] {
		Tiles_Select = Tiles("PoteauV", sf::Vector2f(sf::Mouse::getPosition()));

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});

	Button& Asset9 = Select_Tiles.Add("Asset9"_hash, func::rectangle({ 1920 / 7 * 3,540 }, { 64,64 },
		sf::Color::White, sf::Color::White, 0.f));
	Asset9.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("RembardeV")).setTextureState("RembardeV", "RembardeV", "RembardeV").setOrigin();

	Asset9.CreateFunctionCall([this] {
		Tiles_Select = Tiles("RembardeV", sf::Vector2f(sf::Mouse::getPosition()));

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});

	Button& Asset10 = Select_Tiles.Add("Asset10"_hash, func::rectangle({ 1920 / 7 * 4,540 }, { 64,64 },
		sf::Color::White, sf::Color::White, 0.f));
	Asset10.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("Toit")).setTextureState("Toit", "Toit", "Toit").setOrigin();

	Asset10.CreateFunctionCall([this] {
		Tiles_Select = Tiles("Toit", sf::Vector2f(sf::Mouse::getPosition()));

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});

	Button& Asset11 = Select_Tiles.Add("Asset11"_hash, func::rectangle({ 1920 / 7 * 5,540 }, { 64,64 },
		sf::Color::White, sf::Color::White, 0.f));
	Asset11.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("Toitdebut")).setTextureState("Toitdebut", "Toitdebut", "Toitdebut").setOrigin();

	Asset11.CreateFunctionCall([this] {
		Tiles_Select = Tiles("Toitdebut", sf::Vector2f(sf::Mouse::getPosition()));

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});

	Button& Asset12 = Select_Tiles.Add("Asset12"_hash, func::rectangle({ 1920 / 7 * 6,540 }, { 64,64 },
		sf::Color::White, sf::Color::White, 0.f));
	Asset12.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("ToitFin")).setTextureState("ToitFin", "ToitFin", "ToitFin").setOrigin();

	Asset12.CreateFunctionCall([this] {
		Tiles_Select = Tiles("ToitFin", sf::Vector2f(sf::Mouse::getPosition()));

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});

	Button& Asset13 = Select_Tiles.Add("Asset13"_hash, func::rectangle({ 1920 / 7 * 2,610 }, { 64,64 },
		sf::Color::White, sf::Color::White, 0.f));
	Asset13.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("asset_3_1")).setTextureState("asset_3_1", "asset_3_1", "asset_3_1").setOrigin();

	Asset13.CreateFunctionCall([this] {
		Tiles_Select = Tiles("asset_3_1", sf::Vector2f(sf::Mouse::getPosition()));

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});

	Button& Asset14 = Select_Tiles.Add("Asset14"_hash, func::rectangle({ 1920 / 7 * 3,610 }, { 64,64 },
		sf::Color::White, sf::Color::White, 0.f));
	Asset14.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("asset_3_2")).setTextureState("asset_3_2", "asset_3_2", "asset_3_2").setOrigin();

	Asset14.CreateFunctionCall([this] {
		Tiles_Select = Tiles("asset_3_2", sf::Vector2f(sf::Mouse::getPosition()));

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});

	Button& Asset15 = Select_Tiles.Add("Asset15"_hash, func::rectangle({ 1920 / 7 * 4,610 }, { 64,64 },
		sf::Color::White, sf::Color::White, 0.f));
	Asset15.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("asset_3_2_V2")).setTextureState("asset_3_2_V2", "asset_3_2_V2", "asset_3_2_V2").setOrigin();

	Asset15.CreateFunctionCall([this] {
		Tiles_Select = Tiles("asset_3_2_V2", sf::Vector2f(sf::Mouse::getPosition()));

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});

	Button& Asset16 = Select_Tiles.Add("Asset16"_hash, func::rectangle({ 1920 / 7 * 5,610 }, { 64,64 },
		sf::Color::White, sf::Color::White, 0.f));
	Asset16.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("asset_3_3")).setTextureState("asset_3_3", "asset_3_3", "asset_3_3").setOrigin();

	Asset16.CreateFunctionCall([this] {
		Tiles_Select = Tiles("asset_3_3", sf::Vector2f(sf::Mouse::getPosition()));

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});
}

void EditorState::Load_Traps()
{
	Button& Tile_E = Select_Tiles.Add("TilesPiquesE"_hash, func::rectangle({ 480,540 }, { 0,0 },
		sf::Color::White, sf::Color::White, 0.f));
	Tile_E.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("TilesPiquesE")).setTextureState("TilesPiquesE", "TilesPiquesE", "TilesPiquesE").Resize().setOrigin();

	Tile_E.CreateFunctionCall([this] {
		sf::Vector2i MousePos = sf::Mouse::getPosition();
		sf::Vector2i PosBouton = sf::Vector2i(
			Select_Tiles.get("TilesPiquesE"_hash).Get_Position().x - Select_Tiles.get("TilesPiquesE"_hash).GlobalBounds().width / 2,
			Select_Tiles.get("TilesPiquesE"_hash).Get_Position().y - Select_Tiles.get("TilesPiquesE"_hash).GlobalBounds().height / 2);

		Tiles_Select = Tiles("TilesPiquesE", sf::Vector2f(MousePos), sf::Vector2i(((MousePos.x - PosBouton.x) / 64) * 64, ((MousePos.y - PosBouton.y) / 64) * 64), { 64,64 });
		Tiles_Select.getShape().setOrigin(32, 32);

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Traps;
		});

	Button& Tile_V = Select_Tiles.Add("TilesPiquesV"_hash, func::rectangle({ 960,540 }, { 0,0 },
		sf::Color::White, sf::Color::White, 0.f));
	Tile_V.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("TilesPiquesV")).setTextureState("TilesPiquesV", "TilesPiquesV", "TilesPiquesV").Resize().setOrigin();

	Tile_V.CreateFunctionCall([this] {
		sf::Vector2i MousePos = sf::Mouse::getPosition();
		sf::Vector2i PosBouton = sf::Vector2i(
			Select_Tiles.get("TilesPiquesV"_hash).Get_Position().x - Select_Tiles.get("TilesPiquesV"_hash).GlobalBounds().width / 2,
			Select_Tiles.get("TilesPiquesV"_hash).Get_Position().y - Select_Tiles.get("TilesPiquesV"_hash).GlobalBounds().height / 2);

		Tiles_Select = Tiles("TilesPiquesV", sf::Vector2f(MousePos), sf::Vector2i(((MousePos.x - PosBouton.x) / 64) * 64, ((MousePos.y - PosBouton.y) / 64) * 64), { 64,64 });
		Tiles_Select.getShape().setOrigin(32, 32);

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Traps;
		});

	Button& Tile_S = Select_Tiles.Add("TilesPiquesS"_hash, func::rectangle({ 1440,540 }, { 0,0 },
		sf::Color::White, sf::Color::White, 0.f));
	Tile_S.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("TilesPiquesS")).setTextureState("TilesPiquesS", "TilesPiquesS", "TilesPiquesS").Resize().setOrigin();

	Tile_S.CreateFunctionCall([this] {
		sf::Vector2i MousePos = sf::Mouse::getPosition();
		sf::Vector2i PosBouton = sf::Vector2i(
			Select_Tiles.get("TilesPiquesS"_hash).Get_Position().x - Select_Tiles.get("TilesPiquesS"_hash).GlobalBounds().width / 2,
			Select_Tiles.get("TilesPiquesS"_hash).Get_Position().y - Select_Tiles.get("TilesPiquesS"_hash).GlobalBounds().height / 2);

		Tiles_Select = Tiles("TilesPiquesS", sf::Vector2f(MousePos), sf::Vector2i(((MousePos.x - PosBouton.x) / 64) * 64, ((MousePos.y - PosBouton.y) / 64) * 64), { 64,64 });
		Tiles_Select.getShape().setOrigin(32, 32);

		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Traps;
		});
}

void EditorState::Load_World()
{
	Button& tab56 = Select_Tiles.Add("Egout"_hash, func::rectangle({ 480,540 }, { 384,216 }, sf::Color::White, sf::Color::White, 0.f)).setOrigin();
	tab56.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("BoutonNiveauE")).setTextureState("BoutonNiveauE", "BoutonNiveauE", "BoutonNiveauE");
	tab56.CreateFunctionCall([this] {
		Biome = World::Egouts;
		Load_Biome();
		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});

	Button& tab57 = Select_Tiles.Add("Ville"_hash, func::rectangle({ 960,540 }, { 384,216 }, sf::Color::White, sf::Color::White, 0.f)).setOrigin();
	tab57.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("BoutonNiveauV")).setTextureState("BoutonNiveauV", "BoutonNiveauV", "BoutonNiveauV");
	tab57.CreateFunctionCall([this] {
		Biome = World::Ville;
		Load_Biome();
		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});

	Button& tab58 = Select_Tiles.Add("Serre"_hash, func::rectangle({ 1440,540 }, { 384,216 }, sf::Color::White, sf::Color::White, 0.f)).setOrigin();
	tab58.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("BoutonNiveauS")).setTextureState("BoutonNiveauS", "BoutonNiveauS", "BoutonNiveauS");
	tab58.CreateFunctionCall([this] {
		Biome = World::Serre;
		Load_Biome();
		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		state = State::Editor;
		});
}

void EditorState::Load_Biome()
{
	if (Biome == World::Egouts)
	{
		tm.Background.first = "7emeplanE";
		tm.Plan5.first = "6emeplanE";
		tm.Plan4.first = "5emeplanE";
		tm.Plan3.first = "4emeplanE";
		tm.Plan1.first = "1erplanE";
	}
	if (Biome == World::Ville)
	{
		tm.Background.first = "7emeplanV";
		tm.Plan5.first = "6emeplanV";
		tm.Plan4.first = "5emeplanV";
		tm.Plan3.first = "4emeplanV";
		tm.Plan1.first = "1erplanV";
	}
	if (Biome == World::Serre)
	{
		tm.Background.first = "7emeplanS";
		tm.Plan5.first = "6emeplanS";
		tm.Plan4.first = "5emeplanS";
		tm.Plan3.first = "4emeplanS";
		tm.Plan1.first = "1erplanS";
	}

	tm.Background.second = sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>(tm.Background.first));
	tm.Plan5.second = std::pair<sf::Sprite,
		sf::Sprite>(sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>(tm.Plan5.first)), sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>(tm.Plan5.first)));
	tm.Plan4.second = std::pair<sf::Sprite,
		sf::Sprite>(sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>(tm.Plan4.first)), sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>(tm.Plan4.first)));
	tm.Plan3.second = std::pair<sf::Sprite,
		sf::Sprite>(sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>(tm.Plan3.first)), sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>(tm.Plan3.first)));
	tm.Plan1.second = std::pair<sf::Sprite,
		sf::Sprite>(sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>(tm.Plan1.first)), sf::Sprite(gm::CacheSystem::AddAnyRessources<sf::Texture>(tm.Plan1.first)));

	tm.Plan5.second.first.setPosition(0, 0);
	tm.Plan5.second.second.setPosition(tm.Plan5.second.second.getGlobalBounds().width, 0);
	tm.Plan4.second.first.setPosition(0, 0);
	tm.Plan4.second.second.setPosition(tm.Plan4.second.second.getGlobalBounds().width, 0);
	tm.Plan3.second.first.setPosition(0, 0);
	tm.Plan3.second.second.setPosition(tm.Plan3.second.second.getGlobalBounds().width, 0);
	tm.Plan1.second.first.setPosition(0, 0);
	tm.Plan1.second.second.setPosition(tm.Plan1.second.second.getGlobalBounds().width, 0);
}

void EditorState::ChooseCollect()
{
	Button& montre = Select_Tiles.Add("montre"_hash, func::rectangle({ 480,540 }, { 100,100 }, sf::Color::White, sf::Color::White, 0.f)).setOrigin();
	montre.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("montre")).setTextureState("montre", "montre", "montre");
	montre.CreateFunctionCall([this] {
		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();

		Tiles_Select = Tiles("montre", sf::Vector2f(sf::Mouse::getPosition()));
		collectible = Collect::Montre;
		});

	Button& BonusEpee = Select_Tiles.Add("BonusEpee"_hash, func::rectangle({ 960,540 }, { 100,100 }, sf::Color::White, sf::Color::White, 0.f)).setOrigin();
	BonusEpee.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("BonusEpee")).setTextureState("BonusEpee", "BonusEpee", "BonusEpee");
	BonusEpee.CreateFunctionCall([this] {
		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();

		Tiles_Select = Tiles("BonusEpee", sf::Vector2f(sf::Mouse::getPosition()));

		collectible = Collect::Special;
		});

	Button& BonusBouclier = Select_Tiles.Add("BonusBouclier"_hash, func::rectangle({ 1440,540 }, { 100,100 }, sf::Color::White, sf::Color::White, 0.f)).setOrigin();
	BonusBouclier.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("BonusBouclier")).setTextureState("BonusBouclier", "BonusBouclier", "BonusBouclier");
	BonusBouclier.CreateFunctionCall([this] {
		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();

		Tiles_Select = Tiles("BonusBouclier", sf::Vector2f(sf::Mouse::getPosition()));

		collectible = Collect::Bouclier;
		});
}

void EditorState::ChooseEnemy()
{
	Button& DroneI = Select_Tiles.Add("DroneI"_hash, func::rectangle({ 384,540 }, { 100,100 }, sf::Color::White, sf::Color::White, 0.f));
	DroneI.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("DroneI")).setTextureState("DroneI", "DroneI", "DroneI").Resize().setOrigin();
	DroneI.CreateFunctionCall([this] {
		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();

		Tiles_Select = Tiles("DroneI", sf::Vector2f(sf::Mouse::getPosition()));
		Enemies = Enemy::Drone;
		});

	Button& araignerI = Select_Tiles.Add("araignerI"_hash, func::rectangle({ 768,540 }, { 100,100 }, sf::Color::White, sf::Color::White, 0.f));
	araignerI.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("araignerI")).setTextureState("araignerI", "araignerI", "araignerI").Resize().setOrigin();
	araignerI.CreateFunctionCall([this] {
		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();

		Tiles_Select = Tiles("araignerI", sf::Vector2f(sf::Mouse::getPosition()));
		Enemies = Enemy::Spider;
		});

	Button& GardeI = Select_Tiles.Add("GardeI"_hash, func::rectangle({ 1152,540 }, { 100,100 }, sf::Color::White, sf::Color::White, 0.f));
	GardeI.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("GardeI")).setTextureState("GardeI", "GardeI", "GardeI").Resize().setOrigin();
	GardeI.CreateFunctionCall([this] {
		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();

		Tiles_Select = Tiles("GardeI", sf::Vector2f(sf::Mouse::getPosition()));
		Enemies = Enemy::Garde;
		});

	Button& BossI = Select_Tiles.Add("BossI"_hash, func::rectangle({ 1536,540 }, { 100,100 }, sf::Color::White, sf::Color::White, 0.f));
	BossI.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("BossI")).setTextureState("BossI", "BossI", "BossI").Resize().setOrigin();
	BossI.CreateFunctionCall([this] {
		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();

		Tiles_Select = Tiles("BossI", sf::Vector2f(sf::Mouse::getPosition()));
		Enemies = Enemy::Boss;
		});
}

void EditorState::StartOrEnd()
{
	Button& Player = Select_Tiles.Add("player"_hash, func::rectangle({ 640,540 }, { 100,100 }, sf::Color::White, sf::Color::White, 0.f));
	Player.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("icone_player")).setTextureState("icone_player", "icone_player", "icone_player").Resize().setOrigin();
	Player.CreateFunctionCall([this] {
		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		Tiles_Select = Tiles("icone_player", sf::Vector2f(sf::Mouse::getPosition()));
		state = State::Set_Start;
		});

	Button& FinNiveau = Select_Tiles.Add("FinNiveau"_hash, func::rectangle({ 1280,540 }, { 100,100 }, sf::Color::White, sf::Color::White, 0.f));
	FinNiveau.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("FinNiveau")).setTextureState("FinNiveau", "FinNiveau", "FinNiveau").Resize().setOrigin();
	FinNiveau.CreateFunctionCall([this] {
		m_buttons.erase(m_buttons.find("TilesMenu"_hash));
		Select_Tiles.Children_Clear();
		Tiles_Select = Tiles("FinNiveau", sf::Vector2f(sf::Mouse::getPosition()));
		state = State::Set_End;
		});
}

void EditorState::drawlayer()
{
	sf::Text tmp2 = func::CreateText("", *font, 20, { 960,105 }, sf::Color::Black);
	switch (selectedLayer)
	{
	case 0:
		window->draw(m_buttons.at("Back_Layer"_hash));
		tmp2.setString("Back_Layer");
		tmp2.setOrigin(func::Get_Middle(tmp2));
		window->draw(tmp2);
		break;
	case 1:
		window->draw(m_buttons.at("Player_Layer"_hash));
		tmp2.setString("Player_Layer");
		tmp2.setOrigin(func::Get_Middle(tmp2));
		window->draw(tmp2);
		break;
	case 2:
		window->draw(m_buttons.at("Deco_Layer"_hash));
		tmp2.setString("Deco_Layer");
		tmp2.setOrigin(func::Get_Middle(tmp2));
		window->draw(tmp2);
		break;
	case 3:
		window->draw(m_buttons.at("Front_Layer"_hash));
		tmp2.setString("Front_Layer");
		tmp2.setOrigin(func::Get_Middle(tmp2));
		window->draw(tmp2);
		break;
	default:
		break;
	}
}

void EditorState::CheckTraps()
{
	findMap = false;
	for (auto& Current_Map : tm.Traps) {
		if (Current_Map.getShape().getGlobalBounds().contains(window->mapPixelToCoords(sf::Mouse::getPosition(), view))) {
			Current_Map = Tiles(Tiles_Select.GetFilepath(), Current_Map.GetPos(), Tiles_Select.GetRect(),
				sf::Vector2i(Tiles_Select.getShape().getGlobalBounds().width, Tiles_Select.getShape().getGlobalBounds().height));
			findMap = true;
			break;
		}
	}
}

void EditorState::drawCollectible()
{
	if (tm.Montre.size() > 0) {
		m_buttons["montre1"_hash].setPosition(tm.Montre.at(0));
		window->draw(m_buttons["montre1"_hash]);
	}
	if (tm.Montre.size() > 1) {
		m_buttons["montre2"_hash].setPosition(tm.Montre.at(1));
		window->draw(m_buttons["montre2"_hash]);
	}
	if (tm.Montre.size() > 2) {
		m_buttons["montre3"_hash].setPosition(tm.Montre.at(2));
		window->draw(m_buttons["montre3"_hash]);
	}

	for (sf::Vector2f& Current : tm.Bouclier) {
		m_buttons["Bouclier"_hash].setPosition(Current);
		window->draw(m_buttons["Bouclier"_hash]);
	}
	for (sf::Vector2f& Current : tm.Special) {
		m_buttons["Special"_hash].setPosition(Current);
		window->draw(m_buttons["Special"_hash]);
	}
}

void EditorState::drawEnnemies()
{
	for (sf::Vector2f& Current : tm.Drone) {
		m_buttons["Drone"_hash].setPosition(Current);
		window->draw(m_buttons["Drone"_hash]);
	}
	for (sf::Vector2f& Current : tm.Garde) {
		m_buttons["GardeI"_hash].setPosition(Current);
		window->draw(m_buttons["GardeI"_hash]);
	}
	for (sf::Vector2f& Current : tm.Araigne) {
		m_buttons["Araigner"_hash].setPosition(Current);
		window->draw(m_buttons["Araigner"_hash]);
	}

	if (sf::FloatRect(0, 0, Nbr_Screen.x * 1920, Nbr_Screen.y * 1080).contains(tm.Boss)) {
		m_buttons["BossI"_hash].setPosition(tm.Boss);
		window->draw(m_buttons["BossI"_hash]);
	}
}
