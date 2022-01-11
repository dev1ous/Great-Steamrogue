#include "Game.h"
#include "OptionState.h"
#include "Boss.h"
#include "GameState.h"

Game::Game(sf::RenderWindow* _window, sf::Font& _font) : m_montres(), chargeSword(), chargeShield(), hud(), Menu_IG{ nullptr,func::rectangle({ 1025, 250},{373,109},sf::Color::White,sf::Color::White,0.f),
func::CreateText("CONTINUER",_font,30,{},sf::Color::Black) }, window{ _window }, font{ _font }, FinDeNiv{ nullptr, func::rectangle({ 1200,950 }, { 346,101 }, sf::Color::White, sf::Color::White, 0.f), func::CreateText("MENU", font, 40, {}, sf::Color::Black) },
retourIG{ nullptr,func::rectangle({1650,970},{481,101},sf::Color::White,sf::Color::White,0.f),func::CreateText("RETOUR",font,50,{},sf::Color::Black) }{}

void Game::Init(std::string _path)
{
	chronom.stopChronometre();
	tm.AddLayer();
	Map.LoadMap();
	log.loadMap(_path, tm, Size);
	Create_MaskCollid();
	view.setSize(1920, 1080);
	pause = false;
	GameOver = false;
	Return = false;
	Reset = false;
	NewScoreToScoreState = false;
	ReturnAfterLose = false;
	ReturnMenuAfterLose = false;
	BlockPauseWhenDie = false;
	chargeSword.NbrCharge = 0;
	End.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("FinNiveau"));
	End.setOrigin(End.getGlobalBounds().width / 2, End.getGlobalBounds().height / 2);
	End.setPosition(tm.Endpos);

	this->player = std::make_unique<Player>();
	this->player->init("sprite_sheet_joueur", "sprite_sheet_joueurattack", tm.Startpos);

	InitEnemy();
	InitCollectible();

	view.setCenter(960, (static_cast<int>(player->GetPos().y / 1080)) * 1080 + 540);

	std::get<0>(menu_ig).setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("rouagemenu1"));
	std::get<0>(menu_ig).setOrigin({ (289 / 2), (293 / 2) });
	std::get<0>(menu_ig).setPosition({ 1250, 950 });
	std::get<1>(menu_ig).setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("rouagemenu2"));
	std::get<1>(menu_ig).setOrigin({ (161 / 2),(163 / 2) });
	std::get<1>(menu_ig).setPosition({ 725,500 });
	std::get<2>(menu_ig).setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("rouagemenu3"));
	std::get<2>(menu_ig).setOrigin({ (151 / 2),(153 / 2) });
	std::get<2>(menu_ig).setPosition({ 755,965 });
	std::get<3>(menu_ig).setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("rouagemenu4"));
	std::get<3>(menu_ig).setOrigin({ (235 / 2),(238 / 2) });
	std::get<3>(menu_ig).setPosition({ 1280,400 });
	std::get<4>(menu_ig).setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("rouagemenu5"));
	std::get<4>(menu_ig).setOrigin({ (118 / 2),(123 / 2) });
	std::get<4>(menu_ig).setPosition({ 1000,85 });

	FinDeNiv.setOrigin().CenterText();
	FinDeNiv.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("menu_1")).setTextureState("menu_1", "menu_2", "menu_3");
	FinDeNiv.CreateFunctionCall([this]() {
		ReturnMenuAfterLose = true;
		});
	Button& recommencer_ = FinDeNiv.Add("recommencerDefaite"_hash, func::rectangle({ 520,950 }, { 481, 101 }, sf::Color::White, sf::Color::White, 0.f),
		func::CreateText("RECOMMENCER", font, 40, {}, sf::Color::Black));
	recommencer_.setOrigin().CenterText();
	recommencer_.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("recommencer_1"));
	recommencer_.setTextureState("recommencer_1", "recommencer_2", "recommencer_3");
	recommencer_.CreateFunctionCall([this]() {
		ReturnAfterLose = true;
		});

	Menu_IG.setOrigin().CenterText();
	Menu_IG.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("boutton1")).setTextureState("boutton1", "boutton2", "boutton3");
	Menu_IG.CreateFunctionCall([this]() {
		pause = false;
		});
	Button& recommencer = Menu_IG.Add("rec"_hash, func::rectangle({ 1025,450 }, { 373, 109 }, sf::Color::White, sf::Color::White, 0.f), func::CreateText("RECOMMENCER", font, 30, {}, sf::Color::Black));
	recommencer.setOrigin().CenterText();
	recommencer.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("boutton1")).setTextureState("boutton1", "boutton2", "boutton2");
	recommencer.CreateFunctionCall([this, _path]() {
		Reset = true;
		});
	Button& options = Menu_IG.Add("opt"_hash, func::rectangle({ 1025,650 }, { 373,109 }, sf::Color::White, sf::Color::White, 0.f), func::CreateText("OPTIONS", font, 30, {}, sf::Color::Black));
	options.setOrigin().CenterText();
	options.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("boutton1")).setTextureState("boutton1", "boutton2", "boutton2");
	options.CreateFunctionCall([this]() {
		EnterOptionsIG = true;
		});
	Button& menuJ = Menu_IG.Add("menujeu"_hash, func::rectangle({ 1025,850 }, { 373,109 }, sf::Color::White, sf::Color::White, 0.f), func::CreateText("MENU", font, 30, {}, sf::Color::Black));
	menuJ.setOrigin().CenterText();
	menuJ.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("boutton1")).setTextureState("boutton1", "boutton2", "boutton2");
	menuJ.CreateFunctionCall([this]() {
		Return = true;
		});

	std::get<0>(m_sprite).setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("ecran_defaite"));
	std::get<0>(m_sprite).setPosition({ -1500, 0 });
	std::get<1>(m_sprite).setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("ecranmortD"));
	std::get<1>(m_sprite).setPosition({ 1500,0 });
	std::get<2>(m_sprite).setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("ecran_victoire"));
	std::get<2>(m_sprite).setPosition({ -1500,0 });
	fondNoir.setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>("fond_noir"));
	fondNoir.setSize({ 1920,1080 });
	fondNoir.setPosition({ 0,0 });

	m_shield = Shield("shield", player->GetPos(), sf::Vector2f(player->getOrigin().x, player->getOrigin().y));
	m_shield.inLife = false;

	m_spriteIG.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("menuoptions"));
	m_spriteIG.setPosition({ 0,0 });
	m_volumeIG.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("menuoptionsvolume"));
	m_volumeIG.setPosition({ 300,50 });
	VsyncIG.setPosition({ 440 , 710 }); FullscreenIG.setPosition({ 390,865 }); FULLSCREENwIG.setPosition({ 390 ,865 }); VsyncDisabledIG.setPosition({ 440,710 });
	VsyncIG.setFillColor(sf::Color::Black); FullscreenIG.setFillColor(sf::Color::Black); FULLSCREENwIG.setFillColor(sf::Color::Black); VsyncDisabledIG.setFillColor(sf::Color::Black);

	retourIG.setOrigin().CenterText();
	retourIG.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("niveau_suivant_1")).setTextureState("niveau_suivant_1", "niveau_suivant_2", "niveau_suivant_3");
	retourIG.CreateFunctionCall([this]() {
		EnterOptionsIG = false;
		});

	[[maybe_unused]] Button& flecheG = retourIG.Add("flecheGauche"_hash, func::rectangle({ 350,890 }, { 37,68 }, sf::Color::White, sf::Color::White, 0.f));
	flecheG.setOrigin().setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("fleche2_L1")).setTextureState("fleche2_L1", "fleche2_L1", "fleche2_L2");
	flecheG.CreateFunctionCall([this]() {
		window->create({ data::HEIGHT,data::WIDTH,32 }, "Great Steamrogue", sf::Style::Fullscreen);
		selectedModeIG = false;
		});
	[[maybe_unused]] Button& flecheD = retourIG.Add("flecheDroite"_hash, func::rectangle({ 845,890 }, { 37,68 }, sf::Color::White, sf::Color::White, 0.f));
	flecheD.setOrigin().setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("fleche2_R1")).setTextureState("fleche2_R1", "fleche2_R1", "fleche2_R2");
	flecheD.CreateFunctionCall([this]() {
		window->create({ data::HEIGHT,data::WIDTH,32 }, "Great Steamrogue", sf::Style::Default);
		selectedModeIG = true;
		});
	[[maybe_unused]] Button& VsyncG = retourIG.Add("VGauche"_hash, func::rectangle({ 350,730 }, { 37,68 }, sf::Color::White, sf::Color::White, 0.f));
	VsyncG.setOrigin().setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("fleche2_L1")).setTextureState("fleche2_L1", "fleche2_L1", "fleche2_L2");
	VsyncG.CreateFunctionCall([this]() {
		window->setVerticalSyncEnabled(true);
		selectedVSYNCIG = false;
		});
	[[maybe_unused]] Button& VsyncD = retourIG.Add("VDroite"_hash, func::rectangle({ 845,730 }, { 37,68 }, sf::Color::White, sf::Color::White, 0.f));
	VsyncD.setOrigin().setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("fleche2_R1")).setTextureState("fleche2_R1", "fleche2_R1", "fleche2_R2");
	VsyncD.CreateFunctionCall([this]() {
		window->setVerticalSyncEnabled(false);
		selectedVSYNCIG = true;
		});
}

void Game::Restart()
{
	chronom.stopChronometre();
	pause = false;
	GameOver = false;
	Return = false;
	Reset = false;
	NewScoreToScoreState = false;
	ReturnAfterLose = false;
	ReturnMenuAfterLose = false;
	BlockPauseWhenDie = false;
	chargeSword.NbrCharge = 0;

	tm.Plan5.second.first.setPosition(0, 0);
	tm.Plan5.second.second.setPosition(tm.Plan5.second.second.getGlobalBounds().width, 0);
	tm.Plan4.second.first.setPosition(0, 0);
	tm.Plan4.second.second.setPosition(tm.Plan4.second.second.getGlobalBounds().width, 0);
	tm.Plan3.second.first.setPosition(0, 0);
	tm.Plan3.second.second.setPosition(tm.Plan3.second.second.getGlobalBounds().width, 0);
	tm.Plan1.second.first.setPosition(0, 0);
	tm.Plan1.second.second.setPosition(tm.Plan1.second.second.getGlobalBounds().width, 0);

	this->player = std::make_unique<Player>();
	this->player->init("sprite_sheet_joueur", "sprite_sheet_joueurattack", tm.Startpos);

	InitEnemy();
	InitCollectible();
	m_shield.inLife = false;

	view.setCenter(960, (static_cast<int>(player->GetPos().y / 1080)) * 1080 + 540);

	std::get<0>(m_sprite).setPosition({ -1500, 0 });
	std::get<1>(m_sprite).setPosition({ 1500,0 });
	std::get<2>(m_sprite).setPosition({ -1500,0 });
}

void Game::DeleteAI(const float& _dt)
{
	int i = 0;
	for (auto& e : this->m_vectorAI)
	{
		bool dead = e->hasExplode();

		if (dead)
		{
			this->m_vectorAI.erase(this->m_vectorAI.begin() + i);
			break;
		}
		i++;
	}
}

void Game::processEvents(const sf::Event& e, const float& dt)
{
	switch (e.type)
	{
	case sf::Event::TextEntered:
		if (GameOver) {
			if (timerGameOver > 8.f) {
				if (std::isprint(e.text.unicode)) {
					if (Joystick)
						Joystick = false;
					input_text += e.text.unicode;
				}
			}
		}
		break;
	case sf::Event::KeyPressed:
		switch (e.key.code) {
		case sf::Keyboard::Key::Escape:
			if (!BlockPauseWhenDie)
				pause = true;
			break;
		case sf::Keyboard::Key::Enter:
			if (GameOver && timerGameOver > 8.f && input_text.size() > 0) {
				highscoreTable.NewScores(input_text, player->returnGold(), static_cast<int>(chronom.getStartMin()), static_cast<int>(chronom.getStartSec()));
				NewScoreToScoreState = true;
			}
			break;
		case sf::Keyboard::Key::BackSpace:
			if (GameOver)
				if (!input_text.empty())
					input_text.pop_back();
			break;
		default:
			break;
		}
		break;
	case sf::Event::JoystickButtonPressed:
		switch (e.joystickButton.button)
		{
		case 2:
			if (GameOver) {
				if (timerGameOver > 8.f && input_text.size() > 0) {
					highscoreTable.NewScores(input_text, player->returnGold(), static_cast<int>(chronom.getStartMin()), static_cast<int>(chronom.getStartSec()));
					NewScoreToScoreState = true;
				}
			}
			break;
		case 7:
			pause = true;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	if (pause) {
		if (!EnterOptionsIG)
			Menu_IG.processEvents(e);
		else {
			sf::Vector2f p3 = static_cast<sf::Vector2f>(sf::Mouse::getPosition()) - p1IG;
			sf::IntRect rectVolume{ 415,260,565,50 };

			if (rectVolume.contains(sf::Mouse::getPosition()))
				OptionState::volume = (p3.x / 510.f) * 100.f;

			retourIG.processEvents(e);
		}
	}
	if (player.get()->timer_mort > 5.5f)
		FinDeNiv.processEvents(e);
}

void Game::AIUpdate(const float& dt)
{
	std::for_each(std::begin(m_vectorAI), std::end(m_vectorAI), [&dt, this](std::vector<std::shared_ptr<AI>>::reference ai) {
		ai->Update(collid, &m_shield, &bullet_p, dt);
		});
}

void Game::update(const float& dt)
{
	if (!player->Life)
		m_shield.inLife = false;

	if (!pause && !GameOver)
	{
		int a = 0;
		for (auto& b : bullet_p) {
			if (b.Check_Collid(collid, b.getBShape().getPosition())) {
				if (a != 0)
					a--;
				bullet_p.erase(std::begin(bullet_p) + a);
				break;
			}
			a++;
		}

		if (player->Life) {
			chronom.startChronometre(dt);
		}
		m_shield.update(dt);

		if (player->GetPos().x > 960 && player->GetPos().x < Size.x * 1920 - 960)
			tm.Update(player->get_Speed(), dt, view);

		this->AIUpdate(dt);
		this->player->Update(collid, &bullet_p, &chargeSword, dt, font);
		if (player->GetPos().x > 960 && player->GetPos().x < Size.x * 1920 - 960)
			view.setCenter(player->GetPos().x, view.getCenter().y);
		else if (player->GetPos().x < 960)
			view.setCenter(960, view.getCenter().y);
		else if (player->GetPos().x > Size.x * 1920 - 960)
			view.setCenter(Size.x * 1920 - 960, view.getCenter().y);

		if (player->GetPos().y > 540 && player->GetPos().y < Size.y * 1080 - 540)
			view.setCenter(view.getCenter().x, player->GetPos().y);
		else if (player->GetPos().y < 540)
			view.setCenter(view.getCenter().x, 540);
		else if (player->GetPos().y > Size.y * 1080 - 540)
			view.setCenter(view.getCenter().x, Size.y * 1080 - 540);

		m_montres.GrabMontre(player.get(), &chronom);
		chargeShield.GrabChargeSword(player.get(), &m_shield);
		chargeShield.update(dt);
		chargeSword.GrabChargeSword(player.get());
		chargeSword.update(dt);

		printChronom = chronom.printCurrentChrono();

		this->DeleteAI(dt);
		if (player.get()->timer_mort > 1.f)
		{
			if (std::get<0>(m_sprite).getPosition().x < 0)
				std::get<0>(m_sprite).move({ 400 * dt, 0 });
			if (std::get<1>(m_sprite).getPosition().x > 0)
				std::get<1>(m_sprite).move({ -400 * dt, 0 });
		}
		if (player.get()->timer_mort > 5.5f)
		{
			if (sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheHori)) < -30 ||
				sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheHori)) > 30 ||
				sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheVert)) < -30 ||
				sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheVert)) > 30)
				sf::Mouse::setPosition(sf::Vector2i(sf::Mouse::getPosition().x + sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheHori)) * 7 * dt,
					sf::Mouse::getPosition().y + sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheVert)) * 7 * dt));

			m_score = func::CreateText(std::to_string(player->returnGold()), font, 30, { 600,500 }, sf::Color::Black);
			m_min = func::CreateText(std::to_string(static_cast<int>(chronom.getStartMin())) + "min", font, 30, { 600,580 }, sf::Color::Black);
			m_sec = func::CreateText(std::to_string(static_cast<int>(chronom.getStartSec())) + "sec ", font, 30, { 700,580 }, sf::Color::Black);
		}
		if (End.getGlobalBounds().intersects(player->GetBounds())) {
			GameOver = true;
		}
	}
	if (pause) {
		if (sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheHori)) < -30 ||
			sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheHori)) > 30 ||
			sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheVert)) < -30 ||
			sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheVert)) > 30)
			sf::Mouse::setPosition(sf::Vector2i(sf::Mouse::getPosition().x + sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheHori)) * 7 * dt,
				sf::Mouse::getPosition().y + sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheVert)) * 7 * dt));

		if (!EnterOptionsIG) {
			std::get<0>(menu_ig).rotate(200 * dt);
			std::get<1>(menu_ig).rotate(-150 * dt);
			std::get<2>(menu_ig).rotate(110 * dt);
			std::get<3>(menu_ig).rotate(-60 * dt);
			std::get<4>(menu_ig).rotate(100 * dt);
		}
		else {
			float p4 = (OptionState::volume / 100.f) * 510.f;
			if (OptionState::volume > 100)
				OptionState::volume = 100;
			else
				m_volumeIG.setPosition({ p1IG.x + p4,p1IG.y - 50 });
		}
		chronom.pauseChrono();
	}

	if (GameOver) {
		chronom.pauseChrono();

		timerGameOver += dt;
		if (std::get<2>(m_sprite).getPosition().x < 0)
			std::get<2>(m_sprite).move({ 400 * dt, 0 });
		if (std::get<1>(m_sprite).getPosition().x > 0)
			std::get<1>(m_sprite).move({ -400 * dt, 0 });

		if (timerGameOver > 5.5f) {
			m_score2 = func::CreateText(std::to_string(player->returnGold()), font, 30, { 600,500 }, sf::Color::Black);
			m_min2 = func::CreateText(std::to_string(static_cast<int>(chronom.getStartMin())) + "min", font, 30, { 600,580 }, sf::Color::Black);
			m_sec2 = func::CreateText(std::to_string(static_cast<int>(chronom.getStartSec())) + "sec ", font, 30, { 700,580 }, sf::Color::Black);
		}

		if (timerGameOver > 8.f) {

			if (sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheHori)) < -30 ||
				sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheHori)) > 30 ||
				sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheVert)) < -30 ||
				sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheVert)) > 30)
				sf::Mouse::setPosition(sf::Vector2i(sf::Mouse::getPosition().x + sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheHori)) * 7 * dt,
					sf::Mouse::getPosition().y + sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(func::JoystickGaucheVert)) * 7 * dt));


			static float timerCursor{ 0.f };
			timerCursor += dt;
			if (timerCursor >= 0.5f) {
				cursorText = !cursorText;
				timerCursor = 0.f;
			}

			static float timerSaisie{ 0.f };
			timerSaisie += dt;

			if (sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(5)) > 30 && timerSaisie > 0.2f) {
				if (Joystick == false)
					Joystick = true;
				Caracter--;
				if (Caracter < 'a' && Caracter > 'Z') { Caracter = 'Z'; }
				if (Caracter < 'A') { Caracter = 'z'; }

				timerSaisie = 0;
			}
			if (sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(5)) < -30 && timerSaisie > 0.2f) {
				if (Joystick == false)
					Joystick = true;
				Caracter++;
				if (Caracter > 'Z' && Caracter < 'a') { Caracter = 'a'; }
				if (Caracter > 'z') { Caracter = 'A'; }

				timerSaisie = 0;
			}
			if (sf::Joystick::isButtonPressed(0, 0) && timerSaisie > 0.2f && Joystick) {
				input_text += Caracter;
				timerSaisie = 0;
			}
			if (sf::Joystick::isButtonPressed(0, 1) && timerSaisie > 0.2f && Joystick) {
				if (!input_text.empty())
					input_text.pop_back();

				timerSaisie = 0;
			}
		}
	}
}

void Game::draw(sf::RenderWindow& window)
{
	window.setView(window.getDefaultView());
	tm.Draw_BackGround(window);
	window.setView(view);
	tm.Draw_BackPlan(window);
	tm.draw(window, TilesMap::layers::back, &view);
	tm.draw(window, TilesMap::layers::player, &view);
	tm.draw(window, TilesMap::layers::deco, &view);
	tm.drawTraps(window, &view);
	this->renderAI(&window);
	m_montres.drawAllMontre(&window);
	chargeShield.draw(&window);
	chargeSword.draw(&window);
	window.draw(End);
	this->renderPlayer(&window);
	m_shield.drawShield(&window, player.get());
	tm.draw(window, TilesMap::layers::front, &view);
	tm.Draw_FrontPlan(window);
	window.setView(window.getDefaultView());
	hud.draw(&window);
	m_montres.draw(&window);
	chargeSword.drawChargeHUD(&window);
	window.draw(printChronom);
	player.get()->drawTextGold(window);

	if (pause) {
		if (!EnterOptionsIG) {
			window.draw(fondNoir);
			func::drawSpan(std::span(menu_ig.data(), menu_ig.size()), window);
			window.draw(*func::CreateSprite({ 550,60 }, gm::CacheSystem::AddAnyRessources<sf::Texture>("menupausegame")));
			Menu_IG.draw(window);
		}
		else {
			window.draw(m_spriteIG);
			window.draw(m_volumeIG);

			if (!selectedModeIG)
				window.draw(FullscreenIG);
			else
				window.draw(FULLSCREENwIG);

			if (!selectedVSYNCIG)
				window.draw(VsyncIG);
			else
				window.draw(VsyncDisabledIG);

			retourIG.draw(window);
		}
	}
	if (player->timer_mort > 0.f)
		BlockPauseWhenDie = true;
	if (player.get()->timer_mort > 1.f) {
		window.draw(fondNoir);
		window.draw(std::get<0>(m_sprite));
		window.draw(std::get<1>(m_sprite));
	}
	if (player.get()->timer_mort > 5.5f) {
		FinDeNiv.draw(window);
		window.draw(m_score);
		window.draw(m_min);
		window.draw(m_sec);
	}

	if (GameOver) {

		window.draw(fondNoir);
		window.draw(std::get<2>(m_sprite));
		window.draw(std::get<1>(m_sprite));

		if (timerGameOver > 5.5f) {
			window.draw(m_score2);
			window.draw(m_min2);
			window.draw(m_sec2);
		}

		if (timerGameOver > 8.f) {
			sf::Text tmp;

			if (Joystick)
				tmp = func::CreateText(input_text + Caracter + (cursorText ? '|' : ' '), font, 35, { 960, 540 }, sf::Color::White);
			else
				tmp = func::CreateText(input_text + (cursorText ? '|' : ' '), font, 35, { 960, 540 }, sf::Color::White);

			tmp.setOrigin(tmp.getLocalBounds().left + tmp.getLocalBounds().width / 2, 25);
			sf::RectangleShape tmp2 = func::rectangle({ 940,520 }, { 500,108 }, sf::Color::White, sf::Color::White, 0.f);
			tmp2.setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>("encadrement_nom"));
			tmp2.setOrigin(tmp2.getGlobalBounds().width / 2, tmp2.getGlobalBounds().height / 2);
			window.draw(tmp2);
			window.draw(tmp);
		}
	}
}

void Game::renderAI(sf::RenderWindow* window)
{
	std::for_each(std::begin(m_vectorAI), std::end(m_vectorAI), [&window](std::vector<std::shared_ptr<AI>>::reference ai) {
		ai->Render(window);
		});
}

void Game::renderPlayer(sf::RenderWindow* window)
{
	this->player->Render(window, &bullet_p);
}

void Game::InitEnemy()
{
	m_vectorAI.clear();

	for (auto& current : tm.Drone) {
		m_vectorAI.emplace_back(std::make_shared<RemoteAI>(current.x, current.y, *this->player));
	}

	for (auto& current : tm.Garde) {
		m_vectorAI.emplace_back(std::make_shared<EliteAI>(current.x, current.y, *this->player));
	}

	for (auto& current : tm.Araigne) {
		m_vectorAI.emplace_back(std::make_shared<BasicAI>(current.x, current.y, *this->player));
	}

	if (tm.Boss.x > 0 && tm.Boss.x < Size.x * 1920 && tm.Boss.y > 0 && tm.Boss.y < Size.y * 1080)
		m_vectorAI.emplace_back(std::make_shared<Boss>(tm.Boss.x, tm.Boss.y, *this->player));
}

void Game::InitCollectible()
{
	m_montres.Clear();
	chargeShield.Clear();
	chargeSword.Clear();

	int i = 0;
	for (auto& current : tm.Montre) {
		if (i == 0) {
			m_montres.Montre1(current);
		}
		if (i == 1) {
			m_montres.Montre2(current);
		}
		if (i == 2) {
			m_montres.Montre3(current);
		}
		i++;
	}

	for (auto& current : tm.Special) {
		chargeSword.AddCharge(current);
	}

	for (auto& current : tm.Bouclier) {
		chargeShield.AddShield(current);
	}
}

void Game::Create_MaskCollid()
{
	sf::RenderTexture colid;
	colid.create(Size.x * 1920, Size.y * 1080);

	colid.clear(sf::Color::Transparent);

	tm.draw(colid, TilesMap::layers::player);
	tm.drawTrapsT(colid);

	colid.display();

	colid.getTexture().copyToImage().saveToFile("../Ressources/Mabite.png");
	collid = colid.getTexture().copyToImage();
}
