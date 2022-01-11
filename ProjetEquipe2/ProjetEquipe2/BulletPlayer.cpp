#include "BulletPlayer.h"
#include "CacheSystem.h"
#include "Data.h"

BulletPlayer::BulletPlayer(const std::string& texture, sf::Vector2f pos, sf::Vector2f origin)
{
	m_pos = pos;
    m_bullet.setPosition(m_pos);
	m_ori = origin;
    m_bullet.setOrigin(m_ori);
    m_bullet.setTexture(&gm::CacheSystem::AddAnyRessources<sf::Texture>(texture));
    animBul.emplace("animBullet", Animator(sf::IntRect(0, 0, 87, 48), 2, 0.15f));
}

bool BulletPlayer::Check_Collid(sf::Image& _collid, sf::Vector2f _pos)
{
    if (sf::FloatRect(0, 0, _collid.getSize().x, _collid.getSize().y).contains(_pos))
        return _collid.getPixel(_pos.x, _pos.y) == sf::Color::Black;

    return true;
}

void BulletPlayer::MoveSpeed(const float& dt)
{
    m_bullet.move({ speed * dt,0 });
}

void BulletPlayer::setSpeed(float _speed,float x)
{
    speed = _speed;
    m_bullet.setScale(x, 1);
}

void BulletPlayer::update(const float& dt)
{
    animBul.at("animBullet").Animation(m_bullet, dt);
}

void BulletPlayer::draw(sf::RenderWindow* window)
{
    window->draw(m_bullet);
}
