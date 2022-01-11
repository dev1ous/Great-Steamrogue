#ifndef FSM_H
#define FSM_H

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Font.hpp"
#include <memory>
#include <unordered_map>
#include <thread>
#include <mutex>
#include "SFML/Graphics/Sprite.hpp"
#include "Hash.h"
#include "SFML/Graphics/Text.hpp"
#include <array>
#include "SFML/Audio.hpp"

class FSMManager; 

class FSM
{
	size_t size{ 4 };
protected:
	sf::Sound m_music;
	sf::SoundBuffer buffer;
	FSMManager* fsmManager;
	sf::RenderWindow* window;
	sf::Font* font;
	std::unique_ptr<FSM> ptr;
	bool m_changed;
	std::unordered_map<HashID, sf::Sprite> m_loadingScreen;
	sf::Sprite m_fond;
	sf::Sprite m_fond_d;
	sf::Text text{ "Loading",*font,50 };
	std::array < sf::Text, 3 > m_points;
	float timerCursor{ 0.f };

	std::thread threading;
	std::mutex m;
	std::unique_lock<std::mutex> Check_Mutex = std::unique_lock<std::mutex>(m, std::defer_lock);
public:
	FSM(FSMManager& _fsmManager,sf::RenderWindow& _window,sf::Font& _font, bool _change = true);
	virtual ~FSM() = default;
	FSM(const FSM&) = delete;
	FSM& operator=(const FSM&) = delete;

	virtual void GoBackPreviousState() = 0;
	virtual void processEvents(const sf::Event& e,const float& dt) = 0;
	virtual void update(const float& dt) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;

	//passing by value to switch state everytime is the most useful way here
	std::unique_ptr<FSM> changeState();

	bool isChanged()const;
};
#endif

