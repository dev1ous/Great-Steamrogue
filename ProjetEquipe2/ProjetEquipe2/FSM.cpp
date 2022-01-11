#include "FSM.h"
#include "CacheSystem.h"

FSM::FSM(FSMManager& _fsmManager, sf::RenderWindow& _window, sf::Font& _font, const bool change) :fsmManager{ &_fsmManager },
window{ &_window }, font{ &_font }, m_changed{ change }
{

	//reserve the exact number i need and use insert instead of emplace to use this number and not increase the capacity of my unordered map more than what i need
	//if you ask yourself why insert sometimes without reserving(later in the code), its cuz the real capacity of your map is not always what its printed
	//and cuz i will not give you an entire tuts about that, you can document yourself for more details about real capacity of stl containers :)
	m_loadingScreen.reserve(size);

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
	text.setOrigin(m_fond.getLocalBounds().left + m_fond.getLocalBounds().width / 2.f, m_fond.getLocalBounds().top + text.getLocalBounds().height / 2.f);
	text.setPosition({ m_fond.getPosition().x + 380, m_fond.getPosition().y + 60 });
	text.setFillColor(sf::Color::Black);
	std::get<0>(m_points) = { ".", *font,50 };
	std::get<0>(m_points).setPosition({ text.getPosition().x - 70,text.getPosition().y - 20 });
	std::get<0>(m_points).setFillColor(sf::Color::Black);
	std::get<1>(m_points) = { ".", *font,50 };
	std::get<1>(m_points).setPosition({ text.getPosition().x - 60,text.getPosition().y - 20 });
	std::get<1>(m_points).setFillColor(sf::Color::Black);
	std::get<2>(m_points) = { ".", *font,50 };
	std::get<2>(m_points).setPosition({ text.getPosition().x - 50,text.getPosition().y - 20 });
	std::get<2>(m_points).setFillColor(sf::Color::Black);
}

std::unique_ptr<FSM> FSM::changeState()
{
	return std::move(ptr);
}

bool FSM::isChanged() const
{
	return m_changed;
}

