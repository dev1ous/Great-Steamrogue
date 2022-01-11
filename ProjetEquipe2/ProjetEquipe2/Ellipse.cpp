#include "Ellipse.h"
#include <cmath>
#include "Data.h"

Ellipse::Ellipse(const sf::Vector2f& radius, unsigned cornerPointCount, sf::Color col, sf::Color colT, float T, sf::Vector2f pos) :
    m_radius(radius), CornerPointCount(cornerPointCount)
{
    setFillColor(col);
    setOutlineColor(colT);
    setOutlineThickness(T);
    setPosition(pos);
    update();
}

void Ellipse::setRadius(const sf::Vector2f& radius)
{
    m_radius = radius;
    update();
}

void Ellipse::setCornerPointCount(unsigned count)
{
    CornerPointCount = count;
    update();
}

sf::Vector2f Ellipse::getPoint(std::size_t index) const
{
    float angle = index * 2 * data::pi / CornerPointCount - data::pi / 2;
    float x = cos(angle) * m_radius.x;
    float y = sin(angle) * m_radius.y;

    return { m_radius.x + x, m_radius.y + y };
}