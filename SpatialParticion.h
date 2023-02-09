#pragma once
#include <SFML/Graphics.hpp>

class SpatialParticion
{
	public:
		SpatialParticion(int t_screenWith,int t_screenHeight);
		void draw(sf::RenderWindow& t_window); 
		sf::RectangleShape& getRect(int t_i);
		bool checkForCollisionAgainst(sf::Sprite t_sprite); 
	private:
		sf::RectangleShape m_screenRects[71]; 

		const  int NUM_OF_RECTS = 71;
};


