#ifndef ROUNDEDRECTANGLESHAPE_H
#define ROUNDEDRECTANGLESHAPE_H

#include "SFML/Graphics/Shape.hpp"

class RoundedRectangleShape : public sf::Shape
{
public:
    explicit RoundedRectangleShape(const sf::Vector2f& size, float radius, unsigned cornerPointCount, sf::Color col, sf::Color colT,
        float T, sf::Vector2f pos);
    RoundedRectangleShape() = default;
    ~RoundedRectangleShape() = default;
    void setSize(const sf::Vector2f& size) { mySize = size; update(); }
    [[nodiscard]] const sf::Vector2f& getSize() const { return mySize; }
    void setCornersRadius(float radius) { myRadius = radius; update(); }
    [[nodiscard]] float getCornersRadius() const { return myRadius; }
    void setCornerPointCount(unsigned count) { myCornerPointCount = count; update(); }
    [[nodiscard]] virtual std::size_t getPointCount() const { return myCornerPointCount * 4; }
    virtual sf::Vector2f getPoint(std::size_t index) const;
    [[nodiscard]] bool getleft() { return LeftR; }
    [[nodiscard]] bool getright() { return RightR; }
    [[nodiscard]] bool getup() { return UpR; }
    [[nodiscard]] bool getdown() { return downR; }
private:
    sf::Vector2f mySize;
    float myRadius;
    unsigned myCornerPointCount;
    bool UpR = false;
    bool downR = false;
    bool LeftR = false;
    bool RightR = false;
};
#endif
