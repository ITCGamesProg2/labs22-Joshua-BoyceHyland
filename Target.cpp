#include "Target.h"

Target::Target()
{
	if (m_texture.loadFromFile("resources/images/target2.png"))
	{
		std::cout << "Could not load the texture for the tank \n"; 
	}

	m_target.setTexture(m_texture);
	m_target.setPosition(m_position);
	m_target.setScale(0.1,0.1);
}

void Target::draw(sf::RenderWindow &t_window)
{
	t_window.draw(m_target); 
}

void Target::despawn()
{
	alive = false; 
}
