#include "BackPack.h"

BackPack::BackPack()
{
	if (!m_texture.loadFromFile("resources/images/backpack.png"))
	{
		std::cout << "Could not load fuel" << std::endl;
	}
	m_body.setTexture(m_texture);
	m_body.setPosition({ 700, 2000 });
	m_body.setScale({ 0.5, 0.5 });
}

void BackPack::draw(sf::RenderWindow& t_window)
{
	t_window.draw(m_body);
}

bool BackPack::collisionCheck(sf::Sprite t_playerTankBase)
{
	if (CollisionDetector::collision(m_body, t_playerTankBase))
	{
		m_body.setPosition({ -100, -100 });
		return true;
	}

	return false;
}
