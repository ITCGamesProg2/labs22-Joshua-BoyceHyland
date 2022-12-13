#include "Target.h"

Target::Target(sf::Texture const &t_texture, sf::Vector2f t_position, float t_offSet)
{
	m_target.setTexture(t_texture);


	// settinf up what side of the offset it will initially set up on 
	bool hasMinusOffset = rand() % 1+1;
	m_position = t_position;
	m_offSet = t_offSet;
	if (hasMinusOffset)
	{
		m_position = { m_position.x - m_offSet, m_position.y - m_offSet };
	}
	else
	{
		m_position = { m_position.x + m_offSet, m_position.y + m_offSet };
	}
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
