#include "Fuel.h"

Fuel::Fuel()
{
	if (!m_texture.loadFromFile("resources/images/fuel.png"))
	{
		std::cout << "Could not load fuel" << std::endl;
	}
	m_body.setTexture(m_texture);
	m_body.setPosition({ 300, 100 });
	m_body.setScale({ 0.1, 0.1});
}

void Fuel::draw(sf::RenderWindow& t_window)
{
	t_window.draw(m_body);
}

bool Fuel::collisionCheck(sf::Sprite t_playerTankBase)
{
	if (CollisionDetector::collision(m_body, t_playerTankBase))
	{
		m_body.setPosition({ -100, -100 });
		return true;
	}

	return false;
}

void Fuel::setPosition(sf::Vector2f t_position)
{
	m_body.setPosition(t_position);
}
