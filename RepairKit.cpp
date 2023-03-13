#include "RepairKit.h"

RepairKit::RepairKit()
{
	if (!m_texture.loadFromFile("resources/images/repairKit.png"))
	{
		std::cout << "Could not load repair kit" << std::endl;
	}
	m_body.setTexture(m_texture); 
	m_body.setPosition({ 100, 100 }); 
	m_body.setScale({ 0.35, 0.35 }); 
}

void RepairKit::draw(sf::RenderWindow& t_window)
{
	t_window.draw(m_body); 
}

bool RepairKit::collisionCheck(sf::Sprite t_playerTankBase)
{
	if (CollisionDetector::collision(m_body, t_playerTankBase))
	{
		m_body.setPosition({ -100, -100 });
		return true; 
	}
	return false;
}




