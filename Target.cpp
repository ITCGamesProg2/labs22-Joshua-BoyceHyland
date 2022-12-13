#include "Target.h"

Target::Target(sf::Texture const &t_texture, sf::Vector2f t_position, float t_offSet)
{
	
	m_position = t_position;
	m_offSet = t_offSet;
	
	m_target.setTexture(t_texture);
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
	m_beenShot = true;
}

void Target::respawn()
{
	alive = true;
	int randOffset = rand()%100 + 1;
	if (randOffset <=40)
	{
		m_position.x = m_position.x + randOffset;
	}
	else if ((randOffset > 40)&&(randOffset<80))
	{
		m_position.x = m_position.x - randOffset;
	}
	else
	{
		m_position = m_position;
	}
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

bool Target::isAlive()
{
	return alive;
}

bool Target::beenShot()
{
	return m_beenShot;
}
