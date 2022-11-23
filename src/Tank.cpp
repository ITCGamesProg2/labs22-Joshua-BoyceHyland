#include "Tank.h"
#include <iostream>

Tank::Tank(sf::Texture const & texture, sf::Vector2f const pos)
: m_texture(texture)
{
	initSprites(pos);
}

void Tank::update(double dt)
{	
	handleKeyInput(); 
	centreTurret();
	float radianRotation = m_tankRotation * DEG_TO_RAD; 

	 m_position.x = m_position.x + std::cos(radianRotation) * m_speed * (dt/ 1000);
	 m_position.y = m_position.y + std::sin(radianRotation) * m_speed * (dt / 1000);

	m_tankBase.setPosition(m_position);
	m_turret.setPosition(m_position); 
	
	m_tankBase.setRotation(m_tankRotation); 
	m_turret.setRotation(m_turretRotation); 

	
	m_speed = std::clamp(m_speed, MAX_REVERSE_SPEED, MAX_FORWARD_SPEED);
	
    m_speed = m_speed * 0.99; 
	
	

	std::cout << m_speed << std::endl; 
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
	m_speed += 2;
}

void Tank::decreaseSpeed()
{
	m_speed -= 2; 
}

void Tank::increaseRotation()
{
	m_tankRotation += 2; 

	if (m_tankRotation == 360.0)
	{
		m_tankRotation = 0; 
	}
}


void Tank::decreaseRotation()
{
	m_tankRotation -= 2; 

	if (m_tankRotation == 0.0)
	{
		m_tankRotation = 359.0; 
	}
}

void Tank::handleKeyInput()
{
	// tank base 
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		increaseSpeed();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		decreaseSpeed();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		increaseRotation();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		decreaseRotation();
	}

	//turret
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
	{
		increaseTurretRotation(); 
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		decreaseTurretRotation(); 
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
	{
		centering = true; 
	}
}

void Tank::increaseTurretRotation()
{
	m_turretRotation += 2;

	/*if (m_tankRotation == 360.0)
	{
		m_turretRotation = 0;
	}*/
}



void Tank::decreaseTurretRotation()
{
	m_turretRotation -= 2;

	/*if (m_turretRotation == 0.0)
	{
		m_turretRotation = 360.0;
	}*/
}

void Tank::centreTurret()
{	
	static float startAngle;
	int destAngle = static_cast<int>(m_tankRotation);
	int bottomOfTank = static_cast<int>(m_tankRotation) - 180; 
	
	if (!centering)
	{
		startAngle = m_turretRotation;
	}

	else 
	{
		/*if (m_turretRotation>bottomOfTank)
		{
			m_turretRotation--; 
		}
		else
		{
			m_turretRotation++; 
		}

		if (static_cast<int>(m_turretRotation)%180 == 0 )
		{
			centering = false; 
		}*/

		if (startAngle < destAngle)
		{
			m_turretRotation++;
		}
		else
		{
			m_turretRotation--; 
		}

		if (static_cast<int>(m_turretRotation) % 360 == 0)
		{
			centering = false; 
		}
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