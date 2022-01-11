#include "OptionState.h"
#include "CacheSystem.h"
#include "Data.h"

OptionState::OptionState(FSMManager& _fsm, sf::RenderWindow& _window, sf::Font& _font, const bool change)
	:FSM{ _fsm,_window,_font,change }, retour{ nullptr,func::rectangle({1650,970},{481,101},sf::Color::White,sf::Color::White,0.f),func::CreateText("RETOUR",*font,50,{},sf::Color::Black) }
{
	/*m_music.play();*/
	m_sprite.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("menuoptions"));
	m_sprite.setPosition({ 0,0 });
	m_volume.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("menuoptionsvolume"));
	m_volume.setPosition({ 300,50 });
	Vsync.setPosition({ 440 , 710 }); Fullscreen.setPosition({ 390,865 }); FULLSCREENw.setPosition({ 390 ,865 }); VsyncDisabled.setPosition({ 440,710 });
	Vsync.setFillColor(sf::Color::Black); Fullscreen.setFillColor(sf::Color::Black); FULLSCREENw.setFillColor(sf::Color::Black); VsyncDisabled.setFillColor(sf::Color::Black);

	retour.setOrigin().CenterText();
	retour.setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("niveau_suivant_1")).setTextureState("niveau_suivant_1", "niveau_suivant_2", "niveau_suivant_3");
	retour.CreateFunctionCall([this]() {
		fsmManager->popState();
		});

	[[maybe_unused]] Button& flecheG = retour.Add("flecheGauche"_hash, func::rectangle({ 350,890 }, { 37,68 }, sf::Color::White, sf::Color::White, 0.f));
	flecheG.setOrigin().setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("fleche2_L1")).setTextureState("fleche2_L1", "fleche2_L1", "fleche2_L2");
	flecheG.CreateFunctionCall([this]() {
		window->create({ data::HEIGHT,data::WIDTH,32 }, "Great Steamrogue", sf::Style::Fullscreen);
		selectedMode = false;
		});
	[[maybe_unused]] Button& flecheD = retour.Add("flecheDroite"_hash, func::rectangle({ 845,890 }, { 37,68 }, sf::Color::White, sf::Color::White, 0.f));
	flecheD.setOrigin().setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("fleche2_R1")).setTextureState("fleche2_R1", "fleche2_R1", "fleche2_R2");
	flecheD.CreateFunctionCall([this]() {
		window->create({ data::HEIGHT,data::WIDTH,32 }, "Great Steamrogue", sf::Style::Default);
		selectedMode = true;
		});
	[[maybe_unused]] Button& VsyncG = retour.Add("VGauche"_hash, func::rectangle({ 350,730 }, { 37,68 }, sf::Color::White, sf::Color::White, 0.f));
	VsyncG.setOrigin().setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("fleche2_L1")).setTextureState("fleche2_L1", "fleche2_L1", "fleche2_L2");
	VsyncG.CreateFunctionCall([this]() {
		window->setVerticalSyncEnabled(true);
		selectedVSYNC = false;
		});
	[[maybe_unused]] Button& VsyncD = retour.Add("VDroite"_hash, func::rectangle({ 845,730 }, { 37,68 }, sf::Color::White, sf::Color::White, 0.f));
	VsyncD.setOrigin().setTexture(gm::CacheSystem::AddAnyRessources<sf::Texture>("fleche2_R1")).setTextureState("fleche2_R1", "fleche2_R1", "fleche2_R2");
	VsyncD.CreateFunctionCall([this]() {
		window->setVerticalSyncEnabled(false);
		selectedVSYNC = true;
		});
}

OptionState::~OptionState()
{
	if (threading.joinable())
		threading.join();
}

void OptionState::GoBackPreviousState()
{
}

void OptionState::processEvents(const sf::Event& e, const float& dt)
{
	sf::Vector2f p3 = static_cast<sf::Vector2f>(sf::Mouse::getPosition()) - p1;
	sf::IntRect rectVolume{ 415,260,565,50 };

	if (rectVolume.contains(sf::Mouse::getPosition()))
		volume = (p3.x / 510.f) * 100.f;

	retour.processEvents(e);
}

void OptionState::update(const float& dt)
{
	float p4 = (volume / 100.f) * 510.f;
	if (volume > 100)
		volume = 100;
	else
		m_volume.setPosition({ p1.x + p4,p1.y - 50 });
}

void OptionState::draw(sf::RenderWindow& window)
{
	window.draw(m_sprite);
	window.draw(m_volume);

	if(!selectedMode)
		window.draw(Fullscreen);
	else
		window.draw(FULLSCREENw);

	if (!selectedVSYNC)
		window.draw(Vsync);
	else
		window.draw(VsyncDisabled);

	retour.draw(window);
	window.draw(func::CreateText("VOLUME : " + std::to_string(static_cast<int>(volume)), *font, 30, { m_volume.getPosition().x -100,m_volume.getPosition().y + 150 }, sf::Color::Black));
}