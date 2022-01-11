#pragma once
#include "Movable.h"
#include "Tiles.h"
#include "SFML/Audio.hpp"
#include "BulletPlayer.h"
#include "CSManager.h"

class BulletPlayer;
class AI;
class CSManager;

class Player :
    public Movable
{
private :
    enum class Dir { Left, Right };
    Dir Direction{ Dir::Right };
    float timer_dash{ 0 };
    float Cooldown{ 0.2 };
    int Jump_Number{ 2 };
    int Dash_Number{ 1 };
    bool ReJump{ true };
    bool Dash{ false };
    bool Jump{ false };
    bool Grab_Left{ false };
    bool Grab_Right{ false };
    bool WallJump{ false };
    bool FightHori{ false };
    bool FightDown{ false };
    bool FightTop{ false };
    bool SpeHori{ false };
    bool Shot{ false };
    bool IsShoot{ false };

    int gold{ 1000 };
    sf::Text gold_t;

    float Speed_run{ 250 };
    float Speed_Dash{ 900 };
    float Power_Jump{ 375 };
    float KnockBack_WallJump{ 500 };

    bool Right{ false };
    bool Left{ false };
    bool isParalise{ false };

    sf::Sound dash, run, grab, sword_s, attackSpe_s, pique_s;
    sf::SoundBuffer dashBuffer, runBuffer, grabBuffer, sword_b, attackSpe_b, pique_b;

    sf::RectangleShape m_shapeAttack{ {0.f,0.f} };
    sf::RectangleShape Sword{ {0,0} };

public:
    Player() = default;
    ~Player() = default;

    float timer_mort{ 0.f };
    bool Life{ true };

    using Entity::InitShape;//enleve le warning de ton initShape

    Player& init(std::string _filepath,std::string filepathAttack, sf::Vector2f _pos);
    virtual void InitShape(std::string _filepath, std::string filepathAttack, float _posX, float _posY);
    void Move_Dash(sf::Image& _collid, const float& _dt);
    void Move_Jump(sf::Image& _collid, const float& _dt);
    void Attack(sf::Image& _collid, std::vector<BulletPlayer>*, CSManager*, const float& _dt);
    void AffectGravity(sf::Image& _collid, const float& _dt);
    void Update(sf::Image&, std::vector<BulletPlayer>*,CSManager*, const float&, sf::Font&);
    void updateAnim(sf::Image& _collid, const float& dt);
    bool Check_Colid(sf::Image& _collid, sf::Vector2f _pos);
    bool Get_NextPosLeft(sf::Image& _collid, const float& _dt);
    bool Get_NextPosRight(sf::Image& _collid, const float& _dt);
    bool Get_NextPosUp(sf::Image& _collid, const float& _dt);
    bool Get_NextPosDown(sf::Image& _collid, const float& _dt);
    void drawTextGold(sf::RenderWindow&);
    //get
    sf::FloatRect GetHitbox() { return m_hitboxComponent->GetGlobalBounds(); }
    bool Get_Dash() { return Dash; }
    inline const sf::Vector2f& get_Speed() const { return m_movementComponent->GetVelocity(); }
    inline int returnGold() { return gold; }
    inline sf::Vector2f getOrigin() { return m_shape.getOrigin(); }
    inline sf::RectangleShape GetSword() { return this->Sword; }
    //set
    inline void setParalise(bool _isParalise) { this->isParalise = _isParalise; }

    using Movable::Render;
    void Render(sf::RenderWindow* _target,std::vector<BulletPlayer>*);

    //void ColSwordEnnemies(std::vector<std::shared_ptr<AI>>*);
};
