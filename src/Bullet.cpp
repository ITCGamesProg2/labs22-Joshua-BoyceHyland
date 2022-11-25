#include "Bullet.h"

Bullet::Bullet()
{
	beenShot = false;
	sf::Vector2f start{ 100,200 };
	if (!bulletTexture.loadFromFile("ASSETS/IMAGES/bullet.png"))
	{
		std::cout << "Could not load bullet\n";
	}
	bulletBody.setTexture(bulletTexture);
	bulletBody.setScale(0.1, 0.1);
	bulletBody.setTextureRect(sf::IntRect(0, 0, 108, 414));
	bulletBody.setOrigin(sf::Vector2f(50, 300));
}

bool Bullet::setStart(sf::Vector2f t_playerPosition,sf::RenderWindow& t_window)
{
	if (!beenShot)
	{
		beenShot = true; 
		m_position = t_playerPosition;
		sf::Vector2f lengthBetween = t_playerPosition - static_cast<sf::Vector2f>(sf::Mouse::getPosition());
		m_speed = thor::unitVector(lengthBetween);
	}
	else
	{
		return false;
	}
}

void Bullet::move()
{
	if (beenShot)
	{
		m_position += m_speed;
		bulletBody.setPosition(m_position);
	}

	if (m_position.y < 0)
	{
		beenShot = false;
	}
}



void Bullet::draw(sf::RenderWindow& t_window)
{
	if (beenShot)
	{
		t_window.draw(bulletBody);
	}
}
bool Bullet::checkCollisionAgainst(sf::RectangleShape t_Enemy)
{
	if (bulletBody.getGlobalBounds().intersects(t_Enemy.getGlobalBounds()))
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