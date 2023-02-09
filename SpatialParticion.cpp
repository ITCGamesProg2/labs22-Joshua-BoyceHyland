#include "SpatialParticion.h"

SpatialParticion::SpatialParticion(int t_screenWith, int t_screenHeight)
{
	float rectWidth = t_screenWith / 8;
	float rectHeght = t_screenHeight / 8;
	
	sf::Vector2f t_startPos = { -rectWidth ,0 + 3};

	for (int i = 0; i < NUM_OF_RECTS; i++)
	{
		t_startPos.x += rectWidth;

		if (t_startPos.x >= t_screenWith)
		{
			t_startPos.x = -rectWidth;
			t_startPos.y += rectHeght;
		}

		m_screenRects[i].setSize({ rectWidth, rectHeght });
		m_screenRects[i].setPosition(t_startPos); 
		m_screenRects[i].setOutlineColor(sf::Color::Red);
		m_screenRects[i].setOutlineThickness(1);
		m_screenRects[i].setFillColor(sf::Color::Transparent);

	}

}

void SpatialParticion::draw(sf::RenderWindow& t_window)
{
	for (int i = 0; i < NUM_OF_RECTS; i++)
	{
		t_window.draw(m_screenRects[i]);
	}
}

sf::RectangleShape& SpatialParticion::getRect(int t_i)
{
	return m_screenRects[t_i];
}
