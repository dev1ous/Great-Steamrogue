#pragma once
#include "Movable.h"
#include "Tiles.h"
#include "Player.h"
#include "BulletPlayer.h"

class BulletPlayer;
class Shield;
class Player; 

class AI :
    public Movable
{
protected: 
    int hp;
    sf::Sound dead_ai_sound;
    sf::SoundBuffer dead_ai_buffer;
    float timeExplode;

    sf::RectangleShape playerDetection;
    Player* player;
    std::vector<sf::RectangleShape> groundDetection;

    sf::Vector2f direction;
    sf::Vector2f normailizeDir;

    float movementSpeed;
    bool explode;
    bool canDamage{ false };
    float timerDamage{ 0.0f };
    bool receiveAttack;
    float timerReceiveAttack;

    sf::Vector2f diff;
    float theta;

    using Entity::InitShape;
    virtual void InitShape(std::string _filepath, float _posX, float _posY);
    virtual void InitComponent();
    virtual void InitAnim() = 0;
    void InitGroundDetection();

public:
    AI(float _posX, float _posY, Player& _player);
    ~AI() = default;

    virtual bool DetectPlayer(sf::RectangleShape _shape);
    virtual bool DetectPlayer(sf::CircleShape _shape);
    virtual bool DetectPlayerD(sf::CircleShape _shape);
    virtual void FollowPlayer(sf::Image& _collid, const float& _dt) = 0;
       
    bool isDead();
    bool hasExplode();

    virtual void Gravity(sf::Image& _collid, const float& _dt);
    virtual void Update(sf::Image& _collid, Shield*, std::vector<BulletPlayer>*, const float& _dt);
    virtual void UpdateAnim(const float& _dt) = 0;
    void UpdateGroundDetection();
    void ReceiveDamage(const float& _dt);

    bool isCollide(sf::Image& _collid, sf::RectangleShape _shape);

    bool Check_Colid(sf::Image& _collid, sf::Vector2f _pos);
    bool Get_NextPosLeft(sf::Image& _collid, const float& _dt);
    bool Get_NextPosRight(sf::Image& _collid, const float& _dt);
    bool Get_NextPosUp(sf::Image& _collid, const float& _dt);
    bool Get_NextPosDown(sf::Image& _collid, const float& _dt);

    inline const int& GetHp() const { return this->hp; }

    inline void SetHp(int _hp) { this->hp = _hp; }
};