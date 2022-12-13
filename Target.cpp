#include "Target.h"

Target::Target()
{
	if (!m_texture.loadFromFile("resources/images/target.png"))
	{
		std::cout << "Could not load the texture for the target \n"; 
	}

	m_target.setTexture(m_texture);
	m_target.setPosition(m_position);
	m_target.setScale(0.1,0.1);
}

void Target::draw(sf::RenderWindow &t_window)
{
	if (alive)
	{
		t_window.draw(m_target);
	}
	 
}

void Target::despawn()
{
	alive = false; 
}

void Target::setPosition(sf::Vector2f t_position)
{
	m_position = t_position;
	m_target.setPosition(m_position);
}

void Target::setOffSet(float t_offSet)
{
	m_offSet = t_offSet; 
}

sf::Sprite Target::getBody()
{
	return m_target;
}
