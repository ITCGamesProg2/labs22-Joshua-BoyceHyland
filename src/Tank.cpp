#include "Tank.h"

Tank::Tank(sf::Texture const & texture, sf::Vector2f const pos)
: m_texture(texture)
{
	initSprites(pos);
}

void Tank::update(double dt)
{	
	
	float radianRotation = m_rotation * DEG_TO_RAD; 

	int newXPos = m_position.x + std::cos(radianRotation) * m_speed * (dt/ 1000);
	int newYPos = m_position.y + std::sin(radianRotation) * m_speed * (dt / 1000);

	m_tankBase.setPosition(newXPos, newYPos);
	m_turret.setPosition(newXPos, newYPos); 
	
	m_speed = std::clamp(m_speed, MAX_REVERSE_SPEED, MAX_FORWARD_SPEED);
}

void Tank::render(sf::RenderWindow & window) 
{
	window.draw(m_tankBase);
	window.draw(m_turret);
}

void Tank::setPosition(sf::Vector2f t_position)
{
	m_tankBase.setPosition(t_position); 
}

void Tank::increaseSpeed()
{
	m_speed += 1;
}

void Tank::decreaseSpeed()
{
	m_speed -= 1; 
}

void Tank::increaseRotation()
{
	m_rotation += 1; 

	if (m_rotation == 360.0)
	{
		m_rotation = 0; 
	}
}


void Tank::decreaseRotation()
{
	m_rotation -= 1; 

	if (m_rotation == 0.0)
	{
		m_rotation = 359.0; 
	}
}

void Tank::initSprites(sf::Vector2f pos)
{
	// Initialise the tank base
	m_tankBase.setTexture(m_texture);
	sf::IntRect baseRect(2, 43, 79, 43);
	m_tankBase.setTextureRect(baseRect);
	m_tankBase.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);
	m_tankBase.setPosition(pos);

	// Initialise the turret
	m_turret.setTexture(m_texture);
	sf::IntRect turretRect(19, 1, 83, 31);
	m_turret.setTextureRect(turretRect);
	m_turret.setOrigin(turretRect.width / 3.0, turretRect.height / 2.0);
	m_turret.setPosition(pos);

}