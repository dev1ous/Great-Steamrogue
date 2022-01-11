#ifndef MONTRESMANAGER_H
#define MONTRESMANAGER_H

#include "Montres.h"
#include <memory>
#include <map>
#include "montreHUD.h"
#include "Chronometre.h"

class MontresManager
{
public:
	using M_Montre = std::map<std::string, std::shared_ptr<Montres>>;
	using C_Montre = std::map<std::string, std::shared_ptr<montreHUD>>;

	MontresManager();
	~MontresManager() = default;

	void Montre1(sf::Vector2f _pos);
	void Montre2(sf::Vector2f _pos);
	void Montre3(sf::Vector2f _pos);

	void Clear();
	void GrabMontre(Player*, Chronometre*);
	void drawAllMontre(sf::RenderWindow*);
	void draw(sf::RenderWindow*);

private:
	M_Montre m_montre;
	C_Montre c_montre;
	bool b1{ false }, b2{ false }, b3{ false };
	int NbrDeMontres{ 0 };

	sf::Sound sound_m;
	sf::SoundBuffer buf_m;
};
#endif

