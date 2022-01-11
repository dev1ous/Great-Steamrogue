#include "Button.h"
#include "Data.h"
#include "CacheSystem.h"
#include "Functions.h"

void Button::CreateFunctionCall(std::function<void()> onClick)noexcept
{
	Click = std::move(onClick);
}

void Button::processEvents(const sf::Event& e)noexcept
{
	sf::FloatRect rect;

	match(m_shapes,
		[&rect](sf::RectangleShape& r) {
			rect = r.getGlobalBounds(); },
		[&rect](sf::CircleShape& c) {
				rect = c.getGlobalBounds(); },
				[&rect](Ellipse& e) {
					rect = e.getGlobalBounds(); },
					[&rect](RoundedRectangleShape& r) {
						rect = r.getGlobalBounds(); }
					);

	switch (e.type)
	{
	case sf::Event::MouseButtonPressed:
		if (rect.contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y)))
		{
			if (!choose)
				changeDefaultColor(std::get<2>(col));
			else
				setTexture(std::get<2>(filepath));
		}
		break;
	case sf::Event::MouseButtonReleased:
		if (rect.contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y)))
		{
			if (Click)
				Click();

			desactivate();
		}
		break;
	case sf::Event::MouseMoved:
		if (rect.contains(static_cast<float>(e.mouseMove.x), static_cast<float>(e.mouseMove.y)))
		{
			select();
		}
		else
		{
			deselect();
		}
		break;
	case sf::Event::JoystickButtonPressed:
		switch (e.joystickButton.button) {
		case 0:
			if (rect.contains(sf::Vector2f(sf::Mouse::getPosition()))) {
				if (!choose)
					changeDefaultColor(std::get<2>(col));
				else
					setTexture(std::get<2>(filepath));
			}
			break;
		default:
			break;
		}
		break;
	case sf::Event::JoystickButtonReleased:
		switch (e.joystickButton.button) {
		case 0:
			if (rect.contains(sf::Vector2f(sf::Mouse::getPosition()))) {
				if (Click)
					Click();

				desactivate();
			}
			break;
		}
		break;
	default:
		break;
	}

	std::for_each(std::begin(getchild()),std::end(getchild()), [&e](pair& h) {h.second.processEvents(e); });
}

void Button::select()noexcept
{
	if (!choose)
		changeDefaultColor(std::get<1>(col));
	else
		setTexture(std::get<1>(filepath));
}

void Button::deselect()noexcept
{
	if (!choose)
		changeDefaultColor(std::get<0>(col));
	else
		setTexture(std::get<0>(filepath));
}

void Button::activate()noexcept
{
	if (!choose)
		changeDefaultColor(std::get<2>(col));
	else
		setTexture(std::get<2>(filepath));
	if (Click)
		Click();
}

void Button::desactivate()noexcept
{
	if (!choose)
		changeDefaultColor(std::get<1>(col));
	else
		setTexture(std::get<1>(filepath));
}

void Button::draw(sf::RenderWindow& window)noexcept
{
	std::for_each(std::begin(getchild()), std::end(getchild()), [&window](pair& h) {h.second.draw(window); });

	match(m_shapes,
		[&window](sf::RectangleShape& r) { window.draw(r); },
		[&window](sf::CircleShape& c) { window.draw(c); },
		[&window](Ellipse& e) {	window.draw(e);	},
		[&window](RoundedRectangleShape& r) {window.draw(r); }
	);

	window.draw(text);
}

void Button::drawEditor(sf::RenderWindow& window) noexcept
{
	std::for_each(std::begin(getchild()), std::end(getchild()), [&window](pair& h) {h.second.drawEditor(window); });

	match(m_shapes,
		[&window](sf::RectangleShape& r) { window.draw(r); },
		[&window](sf::CircleShape& c) { window.draw(c); },
		[&window](Ellipse& e) {	window.draw(e);	},
		[&window](RoundedRectangleShape& r) {window.draw(r); }
	);

	if (MouseInButton() && !text.getString().isEmpty()) {
		RoundedRectangleShape shape = RoundedRectangleShape({ text.getGlobalBounds().width + 10,text.getGlobalBounds().height + 10 }, 2.f, 20.f, { 30, 127, 203, 180 }, { 30, 127, 203, 180 }, 2.f, { (float)sf::Mouse::getPosition().x + 50.f, (float)sf::Mouse::getPosition().y + 50.f });
		shape.setOrigin(shape.getGlobalBounds().width / 2, shape.getGlobalBounds().height / 2);
		text.setPosition(sf::Mouse::getPosition().x + 50, sf::Mouse::getPosition().y + 50);
		window.draw(shape);
		window.draw(text);
	}
}

Button& Button::setTexture(const sf::Texture& path)noexcept
{
	choose = true;
	CurrentTexture = sf::Vector2f(path.getSize());

	match(m_shapes,
		[&path](sf::RectangleShape& r) {r.setTexture(&path); },
		[&path](sf::CircleShape& c) {c.setTexture(&path); },
		[&path](Ellipse& e) {e.setTexture(&path); },
		[&path](RoundedRectangleShape& r) {r.setTexture(&path); }
	);
	return *this;
}

Button& Button::setColorState(sf::Color noMouseOn, sf::Color mouseMovedOn, sf::Color mouseClickedOn)noexcept
{
	choose = false;
	std::get<0>(col) = noMouseOn;
	std::get<1>(col) = mouseMovedOn;
	std::get<2>(col) = mouseClickedOn;
	return *this;
}

Button& Button::setTextureState(const std::string& noMouseOn, const std::string& mouseMovedOn, const std::string& mouseClickedOn) noexcept
{
	choose = true;
	std::get<0>(filepath) = gm::CacheSystem::AddAnyRessources<sf::Texture>(noMouseOn);
	std::get<1>(filepath) = gm::CacheSystem::AddAnyRessources<sf::Texture>(mouseMovedOn);
	std::get<2>(filepath) = gm::CacheSystem::AddAnyRessources<sf::Texture>(mouseClickedOn);
	return *this;
}

Button& Button::Resize() noexcept
{
	if (choose)
	{
		match(m_shapes,
			[this](sf::RectangleShape& r) {r.setSize(CurrentTexture); },
			[this](sf::CircleShape& c) {c.setRadius(CurrentTexture.x); },
			[this](Ellipse& e) {e.setRadius(CurrentTexture); },
			[this](RoundedRectangleShape& r) {r.setSize(CurrentTexture); }
		);
	}

	return *this;
}

Button& Button::changeDefaultColor(sf::Color col)noexcept
{
	choose = false;
	match(m_shapes,
		[color = std::move(col)](sf::RectangleShape& r) mutable {r.setFillColor(std::move(color)); },
		[color = std::move(col)](sf::CircleShape& c) mutable {c.setFillColor(std::move(color)); },
		[color = std::move(col)](Ellipse& e) mutable {e.setFillColor(std::move(color)); },
		[color = std::move(col)](RoundedRectangleShape& r) mutable {r.setFillColor(std::move(color)); }
	);
	return *this;
}

Button& Button::setPosition(sf::Vector2f pos)noexcept
{
	match(m_shapes,
		[pos_ = std::move(pos)](sf::RectangleShape& r) mutable { r.setPosition(std::move(pos_)); },
		[pos_ = std::move(pos)](sf::CircleShape& c) mutable {c.setPosition(std::move(pos_)); },
		[pos_ = std::move(pos)](Ellipse& e) mutable {e.setPosition(std::move(pos_)); },
		[pos_ = std::move(pos)](RoundedRectangleShape& r) mutable {r.setPosition(std::move(pos_)); }
	);
	return *this;
}

Button& Button::setOrigin() noexcept
{
	match(m_shapes,
		[](sf::RectangleShape& r) {r.setOrigin(r.getGlobalBounds().width / 2.f, r.getGlobalBounds().height / 2.f); },
		[](sf::CircleShape& c) { c.setOrigin(c.getGlobalBounds().width / 2.f, c.getGlobalBounds().height / 2.f); },
		[](Ellipse& e) { e.setOrigin(e.getGlobalBounds().width / 2.f, e.getGlobalBounds().height / 2.f); },
		[](RoundedRectangleShape& r) { r.setOrigin(r.getGlobalBounds().width / 2.f, r.getGlobalBounds().height / 2.f); }
	);
	return *this;
}

Button& Button::setRotation(float rota) noexcept
{
	match(m_shapes,
		[rota](sf::RectangleShape& r) { r.setRotation(rota); },
		[rota](sf::CircleShape& c) {c.setRotation(rota); },
		[rota](Ellipse& e) {e.setRotation(rota); },
		[rota](RoundedRectangleShape& r) {r.setRotation(rota); }
		);
	return *this;
}

sf::Vector2f Button::Get_Position()
{
	sf::Vector2f Pos;

	match(m_shapes,
		[&Pos](sf::RectangleShape& r) {
			Pos = r.getPosition(); },
		[&Pos](sf::CircleShape& c) {
				Pos = c.getPosition(); },
				[&Pos](Ellipse& e) {
					Pos = e.getPosition(); },
					[&Pos](RoundedRectangleShape& r) {
						Pos = r.getPosition(); }
					);

	return Pos;
}

bool Button::MouseInButton()
{
	bool find{ false };

	match(m_shapes,
		[&find](sf::RectangleShape& r) {
			find = r.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition())); },
		[&find](sf::CircleShape& c) {
				find = c.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition())); },
				[&find](Ellipse& e) {
					find = e.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition())); },
					[&find](RoundedRectangleShape& r) {
						find = r.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition())); }
					);

	return find;
}

sf::FloatRect Button::GlobalBounds()
{
	sf::FloatRect Rect;

	match(m_shapes,
		[&Rect](sf::RectangleShape& r) {
			Rect = r.getGlobalBounds(); },
		[&Rect](sf::CircleShape& c) {
				Rect = c.getGlobalBounds(); },
				[&Rect](Ellipse& e) {
					Rect = e.getGlobalBounds(); },
					[&Rect](RoundedRectangleShape& r) {
						Rect = r.getGlobalBounds(); }
					);

	return Rect;
}

void Button::CenterText()noexcept
{
	sf::FloatRect rect = text.getLocalBounds();
	match(m_shapes,
		[this, &rect](sf::RectangleShape& r) {
			text.setOrigin(rect.left + rect.width / 2.f, rect.top + rect.height / 2.f);
			text.setPosition(sf::Vector2f(r.getPosition())); },
		[this, &rect](sf::CircleShape& c) {
				text.setOrigin(rect.left + rect.width / 2.f, rect.top + rect.height / 2.f);
				text.setPosition(sf::Vector2f(c.getPosition())); },
				[this, &rect](Ellipse& e) {
					text.setOrigin(rect.left + rect.width / 2.f, rect.top + rect.height / 2.f);
					text.setPosition(sf::Vector2f(e.getPosition())); },
					[this, &rect](RoundedRectangleShape& r) {
						text.setOrigin(rect.left + rect.width / 2.f, rect.top + rect.height / 2.f);
						text.setPosition(sf::Vector2f(r.getPosition())); }
					);
}