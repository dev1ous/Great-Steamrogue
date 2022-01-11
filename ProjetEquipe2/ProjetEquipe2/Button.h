#ifndef BUTTON_H
#define BUTTON_H

#include "Container.h"
#include <string>
#include <functional>
#include "Overload.h"
#include <array>
#include "Ellipse.h"
#include "RoundedRectangleShape.h"

class Button : public Container<Button>
{
public:
	using Shapes = std::variant<sf::CircleShape, sf::RectangleShape, Ellipse, RoundedRectangleShape>;
	using CI_button = Container<Button>;

	template<typename B1, typename B2 = sf::Text>
	requires std::is_convertible_v<B1, Shapes>
		Button(Button* parent, B1&& _shapes, B2&& _text = sf::Text())noexcept :CI_button(parent), m_shapes(std::forward<B1>(_shapes)), text(std::forward<B2>(_text)) {
	}
	~Button() = default;

	void CreateFunctionCall(std::function<void()>)noexcept;
	void processEvents(const sf::Event&)noexcept;
	void draw(sf::RenderWindow&)noexcept;
	void drawEditor(sf::RenderWindow&)noexcept;
	Button& setTexture(const sf::Texture&)noexcept;
	Button& setColorState(sf::Color, sf::Color, sf::Color)noexcept;
	Button& setTextureState(const std::string&, const std::string&,const std::string&)noexcept;
	Button& Resize()noexcept;
	void CenterText()noexcept;
	Button& changeDefaultColor(sf::Color)noexcept;
	Button& setPosition(sf::Vector2f)noexcept;
	Button& setOrigin()noexcept;
	Button& setRotation(float)noexcept;
	sf::Vector2f Get_Position();
	bool MouseInButton();
	sf::FloatRect GlobalBounds();

private:
	void select()noexcept;
	void deselect()noexcept;
	void activate()noexcept;
	void desactivate()noexcept;
	
	Shapes m_shapes{};
	sf::Text text{};
	std::function<void()>Click{};
	std::array<sf::Color, 3> col{};
	std::array<sf::Texture, 3> filepath{};
	sf::Vector2f CurrentTexture;

	bool toggle{ false };
	bool choose{ false };								
};


#endif
