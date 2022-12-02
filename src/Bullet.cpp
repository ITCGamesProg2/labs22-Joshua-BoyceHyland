#include "Bullet.h"

Bullet::Bullet()
{
	beenShot = false;
	sf::Vector2f start{ 600,800 };
	if (!bulletTexture.loadFromFile("resources/images/SpriteSheet.png"))
	{
		std::cout << "Could not load bullet\n";
	}
	bulletTexture.setSmooth(true);
	bulletBody.setTexture(bulletTexture);
	bulletBody.setScale(2, 2);
	bulletBody.setTextureRect(sf::IntRect(6, 175, 7, 11));
	bulletBody.setOrigin(sf::Vector2f(-15,15));
	//bulletBody.setPosition(start);
}

bool Bullet::setStart(sf::Vector2f t_playerPosition, float t_playerRotation)
{
	//if (!beenShot)
	//{
	//	m_speed = { 20,20 }; // resets the speed so it does multiply the unit vector my the last speed 
		beenShot = true;
		m_position = t_playerPosition; // make the start position the player
		
	//	//sf::Vector2f lengthBetween =   static_cast<sf::Vector2f>(sf::Mouse::getPosition(t_window)) - t_playerPosition;
		//sf::Vector2f unitVector;/// = thor::unitVector(lengthBetween);
		/*unitVector.x = cos(t_playerRotation);
		unitVector.y = sin(t_playerRotation);

		m_speed.x = m_speed.x * unitVector.x;
		m_speed.y = m_speed.y * unitVector.y;*/


		bulletBody.setPosition(m_position);
		bulletBody.setRotation(t_playerRotation+180);

		return true;
	//}
	///else
	//{
		//return false;
	//}
}

void Bullet::move()
{
//	std::cout << "POSITION X: " << m_position.x << "Y: " << m_position.y << std::endl;
	if (beenShot)
	{
		m_position += m_speed;
		bulletBody.setPosition(m_position);
	}

	if ((m_position.y < 0|| m_position.y > 800)||(m_position.x < 0 || m_position.y >800))
	{
		beenShot = false;
	}
}



void Bullet::draw(sf::RenderWindow& t_window)
{
	/*if (beenShot)
	{*/
		t_window.draw(bulletBody);
	//}
}
bool Bullet::checkCollisionAgainst(sf::RectangleShape t_Enemy)
{
	if ((bulletBody.getGlobalBounds().intersects(t_Enemy.getGlobalBounds()))&&(beenShot))
	{
		m_position = { 20, -100 }; 
		bulletBody.setPosition(m_position); 
		beenShot = false;
		return true;
	}
	else
	{
		return false;
	}
}

bool Bullet::checkCollisionAgainst2(sf::Sprite t_Enemy)
{
	if (bulletBody.getGlobalBounds().intersects(t_Enemy.getGlobalBounds()))
	{
		beenShot = false;
		return true;
	}
	else
	{
		return false;
	}
}


sf::Sprite Bullet::getBody() const
{
	return bulletBody;
}