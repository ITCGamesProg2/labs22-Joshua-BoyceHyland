#pragma once
#include <SFML/Graphics.hpp>
#include "CollisionDetector.h"
class PickUps
{
	public:
		PickUps(sf::Vector2f t_position, sf::Texture &t_texture);
		bool checkForCollision(sf::Sprite playerSprite); 
		void draw(sf::RenderWindow& t_window); 

	private:
		sf::Sprite m_body; 
		sf::Texture &m_texture; 
};