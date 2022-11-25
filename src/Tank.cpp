#include "Tank.h"
#include <iostream>

Tank::Tank(sf::Texture const & texture, std::vector<sf::Sprite>& t_wallSprites)
: m_texture(texture),
  m_wallSprites(t_wallSprites)
{
	initSprites();
}

void Tank::update(double dt)
{	
	handleKeyInput(); 
	centreTurret();
	if (checkCWallCollision())
	{
		deflect(); 
	}

	m_previousPosition = m_position; 
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
	m_previousSpeed = m_speed;
	m_speed += 2;
}

void Tank::decreaseSpeed()
{
	m_previousSpeed = m_speed;
	m_speed -= 2; 
}

void Tank::increaseRotation()
{
	m_previousRotation = m_tankRotation; 
	m_tankRotation += 2; 
	m_turretRotation += 2;

	if (m_tankRotation == 360.0)
	{
		m_tankRotation = 0; 
	}
}


void Tank::decreaseRotation()
{
	m_previousRotation = m_tankRotation;
	m_tankRotation -= 2; 
	m_turretRotation -= 2; 

	if (m_tankRotation == 0.0)
	{
		m_tankRotation = 359.0; 
	}
	
}

void Tank::handleKeyInput()
{
	// tank base 
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		increaseSpeed();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		decreaseSpeed();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		increaseRotation();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		decreaseRotation();
	}

	//turret
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
	{
		increaseTurretRotation(); 
	}
	/*if (sf::Mouse::Wheel::)
	{
		increaseTurretRotation();
	}*/

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		decreaseTurretRotation(); 
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
	{
		if (m_tankRotation != m_turretRotation)
		{
			centering = true; 
		}
		
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		std::cout << "~~~~~~~shhhhhhooot" << std::endl;
		m_speed = m_speed - 8;
	}
}

void Tank::increaseTurretRotation()
{
	m_turretRotation += 2;

	if (m_tankRotation == 360.0)
	{
		m_turretRotation = 0;
	}
}



void Tank::decreaseTurretRotation()
{
	m_turretRotation -= 2;

	if (m_turretRotation == 0.0)
	{
		m_turretRotation = 360.0;
	}
}

void Tank::centreTurret()
{	
	float startAngle = m_turretRotation;
	int destAngle = static_cast<int>(m_tankRotation);
	
	// if not centering assigns the angle its beginning from 
	if(centering)
	{
		

		if (startAngle < destAngle)
		{
			m_turretRotation++;
		}
		else 
		{
			m_turretRotation--; 
		}

		if ((startAngle - destAngle)>180)
		{
			m_turretRotation--;
		}

		// if the turret rotation reaches the destination it stops 
		if (static_cast<int>(m_turretRotation) == destAngle)
		{
			centering = false; 
		}
	}
}

bool Tank::checkCWallCollision()
{
	for (sf::Sprite const& wall : m_wallSprites)
	{
		if((CollisionDetector::collision(m_turret, wall))||(CollisionDetector::collision(m_tankBase, wall)))
		{ 
			return true; 
		}
	}

	return false;
}

void Tank::deflect()
{
	// accomadates for if tank is rotating
	//adjustRotation();

	// if the tank is moving 

	if (m_speed != 0)
	{
		// no rotation when there is a collision
		m_enableRotation = false; 

		// backs up to position in previous frame
		m_tankBase.setPosition(m_previousPosition); 

		// applying small force in oppositie direction fo travle

		if (m_previousSpeed < 0)
		{
			m_speed = 8; 
		}
		else
		{
			m_speed = -8; 
		}
	}
}

void Tank::initSprites()
{
	// Initialise the tank base
	m_tankBase.setTexture(m_texture);
	sf::IntRect baseRect(2, 43, 79, 43);
	m_tankBase.setTextureRect(baseRect);
	m_tankBase.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);
	//m_tankBase.setPosition(pos);

	// Initialise the turret
	m_turret.setTexture(m_texture);
	sf::IntRect turretRect(19, 1, 83, 31);
	m_turret.setTextureRect(turretRect);
	m_turret.setOrigin(turretRect.width / 3.0, turretRect.height / 2.0);
	//m_turret.setPosition(pos);

}