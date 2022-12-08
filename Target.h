#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class Target
{
public:
	Target();
	void draw(sf::RenderWindow &t_window);
private:
	sf::Sprite m_target;
	sf::Texture m_texture;
	sf::Vector2f m_position{ 200,200 };
};