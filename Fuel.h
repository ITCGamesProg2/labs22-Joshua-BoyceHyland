#pragma once

#include "Tank.h"
#include "CollisionDetector.h"

class Fuel
{
	public:
		Fuel(); 
		void draw(sf::RenderWindow& t_window); ;
		bool collisionCheck(sf::Sprite t_playerTankBase);
		void setPosition(sf::Vector2f t_position); 
	private:
		sf::Sprite m_body; 
		sf::Texture m_texture; 
};