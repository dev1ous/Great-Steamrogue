#pragma once
#include "SFML/Graphics.hpp"
#include <assert.h>
#include <vector>
#include <unordered_map>
#include <span>

namespace func {
	[[nodiscard]] inline sf::Sprite* CreateSprite(sf::Vector2f pos, sf::Texture& texture)noexcept
	{
		auto sprite = new sf::Sprite(texture);
		sprite->setPosition(pos);
		return sprite;
	}

	[[nodiscard]] inline sf::Text CreateText(std::string _name, sf::Font& _font, int _size, sf::Vector2f _pos, sf::Color col)noexcept
	{
		sf::Text text(_name, _font, _size);
		text.setFillColor(col);
		text.setPosition(_pos);
		return text;
	}

	[[nodiscard]] inline sf::RectangleShape rectangle(sf::Vector2f pos, sf::Vector2f size, sf::Color col, sf::Color ouC, float ouT)noexcept
	{
		sf::RectangleShape shape;
		shape.setPosition(pos);
		shape.setSize(size);
		shape.setFillColor(col);
		shape.setOutlineColor(ouC);
		shape.setOutlineThickness(ouT);

		return shape;
	}

	[[nodiscard]] inline sf::ConvexShape line(sf::Color col, sf::Color ouC, float ouT, sf::Vector2f dirT, sf::Vector2f dirR)noexcept
	{
		sf::ConvexShape convex;
		convex.setPointCount(4);
		convex.setFillColor(col);
		convex.setOutlineColor(ouC);
		convex.setOutlineThickness(ouT);
		convex.setPoint(0, dirT);
		convex.setPoint(1, dirR);
		convex.setPoint(2, dirR);
		convex.setPoint(3, dirT);

		return convex;
	}

	[[nodiscard]] inline sf::ConvexShape triangle(sf::Color col, sf::Color ouC, float ouT, sf::Vector2f posR, sf::Vector2f posL, sf::Vector2f posT)noexcept
	{
		sf::ConvexShape tri;
		tri.setPointCount(3);
		tri.setFillColor(col);
		tri.setOutlineColor(ouC);
		tri.setOutlineThickness(ouT);
		tri.setPoint(0, posR);
		tri.setPoint(1, posL);
		tri.setPoint(2, posT);
		return tri;
	}

	[[nodiscard]] inline sf::VertexArray Box(sf::Vector2i box0, sf::Vector2i box1, sf::Vector2i box2, sf::Vector2i box3, sf::Color col, sf::RenderWindow& window)noexcept
	{
		sf::VertexArray box(sf::Quads, 4);
		box[0].position = window.mapPixelToCoords(box0);
		box[1].position = window.mapPixelToCoords(box1);
		box[2].position = window.mapPixelToCoords(box2);
		box[3].position = window.mapPixelToCoords(box3);
		for (int i = 0; i < 4; i++)
			box[i].color = col;
		return box;
	}

	[[nodiscard]] inline sf::VertexArray GridOfBox(int start, int Largeur, int Hauteur, int SizeBox, sf::Color col)noexcept
	{
		sf::VertexArray Lines(sf::Lines, (Largeur + Hauteur) / (SizeBox / 2));
		int i = 0;
		for (int x = 0; x < Largeur; x += SizeBox)
		{
			Lines[i].position = sf::Vector2f(x, start);
			Lines[i++].color = col;
			Lines[i].position = sf::Vector2f(x, Hauteur);
			Lines[i++].color = col;
		}
		for (int n = SizeBox; n < Hauteur; n += SizeBox)
		{
			Lines[i].position = sf::Vector2f(start, n);
			Lines[i++].color = col;
			Lines[i].position = sf::Vector2f(Largeur, n);
			Lines[i++].color = col;
		}
		return Lines;
	}

	[[nodiscard]] inline sf::CircleShape circle(int PointCount, float radius, sf::Color col, sf::Color ouC, float ouT, sf::Vector2f pos)noexcept
	{
		sf::CircleShape cir;
		cir.setPointCount(PointCount);
		cir.setRadius(radius);
		cir.setFillColor(col);
		cir.setOutlineColor(ouC);
		cir.setOutlineThickness(ouT);
		cir.setPosition(pos);

		return cir;
	}

	[[nodiscard]] inline float length(sf::Vector2f vector)noexcept
	{
		return sqrtf(vector.x * vector.x + vector.y * vector.y);
	}

	[[nodiscard]] inline sf::Vector2f perpendicularVector(sf::Vector2f vector)noexcept
	{
		return sf::Vector2f(-vector.x, vector.y);
	}

	[[nodiscard]] inline sf::Vector2f normalize(const sf::Vector2f& v)noexcept {
		assert(v != sf::Vector2f(0.f, 0.f));
		return v / length(v);
	}

	[[nodiscard]] inline float distance(const sf::Vector2f& x, const sf::Vector2f& y)noexcept {
		return length(x - y);
	}

	template<typename T1, typename T2>
	[[nodiscard]] bool isCollide(T1 _type1, T2 _type2) {
		return _type1.getGlobalBounds().intersects(_type2.getGlobalBounds());
	}

	[[nodiscard]] inline sf::ConvexShape Line(sf::Vector2f direction, sf::Color color, float thickness, sf::Color outlineColor, float outlineThickness) noexcept
	{
		sf::Vector2f perpendicular = normalize(perpendicularVector(direction)) * 0.5f * thickness;

		sf::ConvexShape line;
		line.setFillColor(color);
		line.setOutlineColor(outlineColor);
		line.setOutlineThickness(outlineThickness);
		line.setPointCount(4);
		line.setPoint(0, -perpendicular);
		line.setPoint(1, perpendicular);
		line.setPoint(2, direction + perpendicular);
		line.setPoint(3, direction - perpendicular);

		return line;
	}

	template<typename T>
	void drawVector(std::vector<T>& vec,sf::RenderWindow& window)noexcept {
		for (auto& v : vec) {
			window.draw(v);
		}
	}

	template<typename K, typename V>
	void drawUnorderMap(std::unordered_map<K, V>& map, sf::RenderWindow* window)noexcept {
		for (auto [k, v] : map) {
			window->draw(v);
		}
	}

	template<typename T>
	void drawSpan(std::span<T> data, sf::RenderWindow& window)noexcept {
		for (const T e : data) {
			window.draw(e);
		}
	}
	
	[[nodiscard]] inline bool IsButtonPressed(sf::Keyboard::Key _key, unsigned _button, unsigned _joystick = 0)noexcept
	{
		return sf::Keyboard::isKeyPressed(_key) || sf::Joystick::isButtonPressed(_joystick, _button);
	}

	enum PadAxis
	{
		JoystickGaucheHori,
		JoystickGaucheVert,

		JoystickGachettes,

		JoystickDroitVert,
		JoystickDroitHori,

		JoystickCroixHori = 6,
		JoystickCroixVert
	};

	[[nodiscard]] inline bool IsDirectionPressed(sf::Keyboard::Key _key, PadAxis _axis, float _start, unsigned _joystick = 0)noexcept
	{
		bool isPress = false;
		if (sf::Keyboard::isKeyPressed(_key))
			isPress = true;

		if (_start > 0) {
			if (sf::Joystick::getAxisPosition(_joystick, static_cast<sf::Joystick::Axis>(_axis)) > _start)
				isPress = true;
		}
		else {
			if (sf::Joystick::getAxisPosition(_joystick, static_cast<sf::Joystick::Axis>(_axis)) < _start)
				isPress = true;
		}

		return isPress;
	}

	[[nodiscard]] inline sf::Vector2f Get_Middle(sf::Text _text)noexcept
	{
		return sf::Vector2f(_text.getLocalBounds().left + _text.getLocalBounds().width / 2,
			_text.getLocalBounds().top + _text.getLocalBounds().height / 2);
	}

	[[nodiscard]] inline bool Circle_Collision(sf::Vector2f Pos1, sf::Vector2f Pos2, float Rayon1, float Rayon2)noexcept
	{
		if (sqrt(pow(Pos1.x - Pos2.x, 2) + pow(Pos1.y - Pos2.y, 2)) < Rayon1 + Rayon2)
		{
			return true;
		}
		else return false;
	}
}