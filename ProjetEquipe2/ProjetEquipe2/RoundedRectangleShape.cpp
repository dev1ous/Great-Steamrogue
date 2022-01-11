#include "RoundedRectangleShape.h"
#include <cmath>
#include "Data.h"

RoundedRectangleShape::RoundedRectangleShape(const sf::Vector2f& size, float radius, unsigned cornerPointCount, sf::Color col, sf::Color colT,
    float T, sf::Vector2f pos) :myRadius(), myCornerPointCount()
{
    mySize = size;
    myRadius = radius;
    myCornerPointCount = cornerPointCount;
    setFillColor(col);
    setOutlineColor(colT);
    setOutlineThickness(T);
    setPosition(pos);
    update();
}

sf::Vector2f RoundedRectangleShape::getPoint(std::size_t index) const
{
    if (index >= myCornerPointCount * 4)
        return sf::Vector2f(0, 0);

    float deltaAngle = 90.0f / (myCornerPointCount - 1);
    sf::Vector2f center;
    unsigned int centerIndex = index / myCornerPointCount;

    switch (centerIndex)
    {
    case 0:
        center.x = mySize.x - myRadius;
        center.y = myRadius;
        break;
    case 1:
        center.x = myRadius;
        center.y = myRadius;
        break;
    case 2:
        center.x = myRadius;
        center.y = mySize.y - myRadius;
        break;
    case 3:
        center.x = mySize.x - myRadius;
        center.y = mySize.y - myRadius;
        break;
    }

    return { myRadius * cos(deltaAngle * (index - centerIndex) * data::pi / 180) + center.x,-myRadius * sin(deltaAngle * (index - centerIndex) * data::pi / 180) + center.y };
}