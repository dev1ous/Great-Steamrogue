#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "SFML/Graphics/Shape.hpp"

class Ellipse :public sf::Shape
{
public:
    explicit Ellipse(const sf::Vector2f& radius, unsigned cornerPointCount, sf::Color col, sf::Color colT, float T, sf::Vector2f pos);
    Ellipse() = default;
    ~Ellipse() = default;

    void setRadius(const sf::Vector2f& radius);
    [[nodiscard]] const sf::Vector2f& getRadius() const { return m_radius; }
    void setCornerPointCount(unsigned count);
    [[nodiscard]] virtual std::size_t getPointCount() const { return CornerPointCount; }
    virtual sf::Vector2f getPoint(std::size_t index) const;

private:
    sf::Vector2f m_radius;
    unsigned CornerPointCount;
};
#endif
