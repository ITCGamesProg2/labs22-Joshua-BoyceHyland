#include "Tank.h"
#include <cmath>
#include <iostream>

Tank::Tank(sf::Texture const & texture, std::vector<sf::Sprite>& t_wallSprites,std::vector<Target>& t_target)
: m_texture(texture),
  m_wallSprites(t_wallSprites),
  m_target(t_target)
	
{
	initSprites();
}

void Tank::update(double dt)
{	
	handleKeyInput(); 
	if (centering)
	{
		centreTurret();
	}
	
	if (checkCWallCollision())
	{
		deflect(); 
	}
	checkBulletCollisions();

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
	
	for (int  i = 0; i < NUM_OF_BULLETS; i++)
	{
		bullets[i].move();
	}
	
	//std::cout <<"Position X: "<<m_position.x<< "Y: "<<m_position.y << std::endl;
}

void Tank::render(sf::RenderWindow & window) 
{
	window.draw(m_tankBase);
	window.draw(m_turret);
	for (int i = 0; i < NUM_OF_BULLETS; i++)
	{
		bullets[i].draw(window);
	}
	
}

void Tank::setPosition(sf::Vector2f t_position)
{
	m_position = t_position;
	m_turret.setPosition(m_position);
	m_tankBase.setPosition(m_position); 
	
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

	if ((sf::Mouse::isButtonPressed(sf::Mouse::Left))&&(m_readyToShoot))
	{
		std::cout << "~~~~~~~shhhhhhooot" << std::endl;
		for (int i = 0; i < NUM_OF_BULLETS; i++)
		{
			if (bullets[i].canSetStart(m_position, m_turretRotation))
			{
				m_speed = m_speed - 8;
				m_tillCanBeShot = 40;
				m_readyToShoot = false;
				break;
			}
		}
	}
	else
	{
		m_tillCanBeShot--; 
		if (m_tillCanBeShot < 0)
		{
			m_readyToShoot = true;
		}
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
	//int destAngle = static_cast<int>(m_tankRotation);
	float destAngle =m_tankRotation;
	// if not centering assigns the angle its beginning from 
	
		if (startAngle < destAngle)
		{
			m_turretRotation++;
		}
		else 
		{
			m_turretRotation--; 
		}

		if (abs(startAngle - destAngle)>180)
		{
			m_turretRotation--;
		}

		// if the turret rotation reaches the destination it stops 
		if (startAngle == destAngle)
		{
			centering = false; 
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

void Tank::checkBulletCollisions()
{
	for (int i = 0; i < NUM_OF_BULLETS; i++)
	{
		for (sf::Sprite const& wall : m_wallSprites)
		{
			if (CollisionDetector::collision(bullets[i].getBody(), wall))
			{
				std::cout << "BULLET HIT THE WALAL" << std::endl; 
				bullets[i].despawn();
			}
		}
	}

	for (int i = 0; i < NUM_OF_BULLETS; i++)
	{
		for (Target  &target : m_target)
		{
			if ((CollisionDetector::collision(bullets[i].getBody(), target.getBody()))&&target.isAlive())
			{
				std::cout << "You hit the target" << std::endl; 
				target.despawn();
				bullets[i].despawn();
				m_score++;
			}
		}
		
	}
	
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

int Tank::getScore()
{
	return m_score;
}

void Tank::initSprites()
{
	sf::IntRect baseRect(2, 43, 79, 43);

	if (m_position.x >700 )
	{
		m_tankRotation = 0;
		m_turretRotation = 0; 
	}
	else
	{
		m_tankRotation = 180;
		m_turretRotation = 180;
	}
	// initial is
	m_tankBase.setTexture(m_texture);
	m_tankBase.setTextureRect(baseRect);
	m_tankBase.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);
	m_tankBase.setRotation(m_tankRotation);

	// Initialise the turret
	m_turret.setTexture(m_texture);
	sf::IntRect turretRect(19, 1, 83, 31);
	m_turret.setTextureRect(turretRect);
	m_turret.setOrigin(turretRect.width / 3.0, turretRect.height / 2.0);
	m_turret.setRotation(m_turretRotation);
}