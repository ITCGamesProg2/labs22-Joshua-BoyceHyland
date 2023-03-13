#pragma once
#include "Tank.h"
#include "CollisionDetector.h"
class RepairKit 
{
	public:
		RepairKit();
		void draw(sf::RenderWindow& t_window); ;
		bool collisionCheck(sf::Sprite t_playerTankBase); 

	private:
		sf::Texture m_texture; 
		sf::Sprite m_body; 
};