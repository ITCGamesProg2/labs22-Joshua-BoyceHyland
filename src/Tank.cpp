#include "Tank.h"
#include <cmath>
#include <iostream>

Tank::Tank(sf::Texture const & texture, std::vector<sf::Sprite>& t_wallSprites,std::vector<Target>& t_target)
: m_texture(texture),
  m_wallSprites(t_wallSprites),
  m_target(t_target)
  //grid(1440, 900) 
{
	initSprites();
}

void Tank::update(double dt, std::function<void(int)>& t_funcApplyDamage, std::function<void(float)>& t_decrementHudFuel,  sf::Sprite t_tankBaseAI)
{	
	handleKeyInput(t_decrementHudFuel); 

	if (centering)
	{
		centreTurret();
	}
	
	if (checkCWallCollision())
	{
		deflect(); 
	}
	checkBulletCollisions(t_funcApplyDamage, t_tankBaseAI);

	m_previousPosition = m_position; 
	float radianRotation = m_tankRotation * DEG_TO_RAD; 
	m_position.x = m_position.x + std::cos(radianRotation) * m_speed * (dt/ 1000);
	m_position.y = m_position.y + std::sin(radianRotation) * m_speed * (dt / 1000);
	
	m_tankBase.setPosition(m_position);
	m_turret.setPosition(m_position); 
	
	m_tankBase.setRotation(m_tankRotation); 
	m_turret.setRotation(m_turretRotation); 

	if (m_turretRequiresRepair)
	{
		m_turretRotation -= 0.25;
	}
	
	m_speed = std::clamp(m_speed, MAX_REVERSE_SPEED, MAX_FORWARD_SPEED);
	
    m_speed = m_speed * 0.99; 
	
	m_bulletPool.update();
	
}

void Tank::render(sf::RenderWindow & window) 
{
	
	//grid.draw(window);
	window.draw(m_tankBase);
	window.draw(m_turret);
	m_bulletPool.draw(window);
	
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

void Tank::handleKeyInput(std::function<void(float)>& t_decrementHudFuel)
{
	// tank base 
	if (fuelSupply > -1)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			increaseSpeed();
			decrementFuelSupply(t_decrementHudFuel);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			decreaseSpeed();
			decrementFuelSupply(t_decrementHudFuel);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			increaseRotation();
			decrementFuelSupply(t_decrementHudFuel);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			decreaseRotation();
			decrementFuelSupply(t_decrementHudFuel);
		}
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

	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::C)) && (!m_turretRequiresRepair))
	{
		if (m_tankRotation != m_turretRotation)
		{
			centering = true; 
		}
		/*else if (centering)
		{
			centering = false; 
		}*/
		
	}
	
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::R)) && (m_turretRequiresRepair))
	{
		if (m_repairsMade < 1)
		{
			m_repairsMade = 30; 
			m_turretRequiresRepair = false; 
		}
		m_repairsMade--; 
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		std::cout << "~~~~~~~shhhhhhooot" << std::endl;

		if (m_bulletPool.setStartOfNextAvailableBullet(m_position, m_turretRotation))
		{
			m_bulletsFired++;
				if (m_bulletsFired % 10 == 0)
				{
					m_turretRequiresRepair = true;
				}
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
	int startAngle = m_turretRotation;
	//int destAngle = static_cast<int>(m_tankRotation);
	int destAngle =m_tankRotation;
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
	/*if (m_tankBase.getGlobalBounds().intersects(grid.getRect(8).getGlobalBounds()))
	{
		grid.getRect(8).setFillColor(sf::Color::Red);
	}
	else
	{
		grid.getRect(8).setFillColor(sf::Color::Transparent);
	}*/

	for (sf::Sprite const& wall : m_wallSprites)
	{
		if((CollisionDetector::collision(m_turret, wall))||(CollisionDetector::collision(m_tankBase, wall)))
		{ 
			return true; 
		}
	}

	return false;
}

void Tank::checkBulletCollisions(std::function<void(int)>& t_funcApplyDamage, sf::Sprite t_tankBase)
{
	for (int i = 0; i < NUM_OF_BULLETS; i++)
	{
		for (sf::Sprite const& wall : m_wallSprites)
		{
			if (CollisionDetector::collision(m_bulletPool.getBullet(i).getBody(), wall))
			{
				std::cout << "BULLET HIT THE WALAL" << std::endl; 
				m_bulletPool.getBullet(i).despawn();
			}
		}
	}

	for (int i = 0; i < NUM_OF_BULLETS; i++)
	{
		for (Target  &target : m_target)
		{
			if ((CollisionDetector::collision(m_bulletPool.getBullet(i).getBody(), target.getBody()))&&target.isAlive())
			{
				std::cout << "You hit the target" << std::endl; 
				target.despawn();
				m_bulletPool.getBullet(i).despawn();
				m_score++;
			}
		}
		
	}

	for (int i = 0; i < NUM_OF_BULLETS; i++)
	{
		
		if ((CollisionDetector::collision(m_bulletPool.getBullet(i).getBody(), t_tankBase)) )
		{
			std::cout << "You hit the Enemy" << std::endl;
			m_bulletPool.getBullet(i).despawn();
			t_funcApplyDamage(1);
			
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

float Tank::getAccuracy() const
{
	return m_score/m_bulletsFired;
}

bool Tank::needsRepair()
{
	return m_turretRequiresRepair;
}

sf::Vector2f Tank::getPosition() const
{
	return m_position;
}

sf::Sprite Tank::getTurret() const
{
	return m_turret;
}

sf::Sprite Tank::getBase() const
{
	return m_tankBase;
}

void Tank::decrementHealth(int t_decrementation)
{
	m_fuelLeak = true;

	m_health -= t_decrementation;

}

bool Tank::isAlive()
{
	return m_health > 0;
}

void Tank::initSprites()
{
	sf::IntRect baseRect(2, 43, 79, 43);

	if (m_position.x != 79)
	{
		m_tankRotation = 180;
		m_turretRotation = 180;
	}
	else
	{

		m_tankRotation = 0;
		m_turretRotation = 0;
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

void Tank::decrementFuelSupply(std::function<void(float)>& t_decrementHudFuel)
{
	std::cout << "Fuel: " << fuelSupply << std::endl;
	
	if (m_fuelLeak)
	{
		fuelSupply -= 0.12;
		t_decrementHudFuel(0.15f);
	}
	else
	{
		fuelSupply -= 0.04;
		t_decrementHudFuel(0.05f);
	}

}
