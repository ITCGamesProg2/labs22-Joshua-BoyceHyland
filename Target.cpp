#include "Target.h"
#include <iostream>

int Target::m_leftOverTime; 

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
	if ((alive)&&(!m_blinkingState))
	{
		t_window.draw(m_target);
	}
	if ((m_blinkingState)&&(alive))
	{
		// 7 good // 9 consistent ///  5 quick // 4 decent // 
		if (m_timedLife.getElapsedTime().asMilliseconds() % 2 ==0)
		{
			t_window.draw(m_target);
		}
	}

}

void Target::despawn()
{
	m_leftOverTime = abs(m_timedLife.getElapsedTime().asSeconds() - m_lifeSpan.asSeconds());
	std::cout <<"Left over time: " + m_leftOverTime << std::endl;
	alive = false; 
	m_beenShot = true;
}

void Target::respawn()
{
	alive = true;
	int randOffset = rand()%100 + 1;
	if (randOffset <=40)
	{
		m_position.x = m_position.x + m_offSet;
		m_position.y = m_position.y + m_offSet;
	}
	else if ((randOffset > 40)&&(randOffset<80))
	{
		m_position.x = m_position.x - m_offSet;
		m_position.y = m_position.y - m_offSet;
	}
	else
	{
		m_position = m_position;
	}
	m_target.setPosition(m_position); 
	//int
	// adds tge remaining time from the last target to the current one if there is on
	sf::Time newLifeSpan = m_lifeSpan  + sf::seconds(m_leftOverTime);
	m_timedLife.restart();
	m_blinkingState = false; 
	m_lifeSpan = newLifeSpan; 
	std::cout << "newLifeSpan: " + std::to_string(m_lifeSpan.asSeconds()) << std::endl;
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

void Target::updateTimer()
{
	if (alive)
	{
		
		if (m_timedLife.getElapsedTime().asSeconds() > m_blinkingTime.asSeconds())
		{
			m_blinkingState = true;
		}
		
		if (m_timedLife.getElapsedTime().asSeconds() > m_lifeSpan.asSeconds())
		{
			alive = false; 
			m_leftOverTime = 0; // reset so it doesnt keep adding on even tho you waited 
		}
		
	}
}


