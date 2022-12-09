#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class Target
{
public:
	Target();
	void draw(sf::RenderWindow &t_window);
	void despawn(); 
	void setPosition(sf::Vector2f t_position); 
	sf::Sprite getBody(); 
private:
	sf::Sprite m_target;
	sf::Texture m_texture;
	sf::Vector2f m_position;
	bool alive = true; 
};