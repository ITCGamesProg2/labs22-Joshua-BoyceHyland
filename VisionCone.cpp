#include "VisionCone.h"
#include <iostream>
VisionCone::VisionCone()
{
	m_visionCone.setPointCount(8);
	// triangle


	m_visionCone.setPoint(0, { 0,0 });
	m_visionCone.setPoint(1, { 200, -50 -30 });
	m_visionCone.setPoint(2, { 200, 50 + 30 });

	
	sf::Color transparentRed = sf::Color::Red;
	transparentRed.a = 50;
	m_visionCone.setFillColor(transparentRed);
	m_visionCone.setOrigin(m_visionCone.getPoint(0));
	m_visionCone.setRotation(tankCenter);
}

void VisionCone::draw(sf::RenderWindow& t_window)
{
	t_window.draw(m_visionCone);
}

void VisionCone::update(AIState& t_aiState,sf::Vector2f t_tankPosition, float t_tankCenter)
{
	tankCenter = t_tankCenter;
	m_visionCone.setPosition({ t_tankPosition});
	
	switch (t_aiState)
	{
		case AIState::Patrol_Map:
			for (int i = 0; i < 3; i++)
			{
				m_visionCone.setPoint(i, patrolCone[i]);
			}
			if (m_visionCone.getRotation() > t_tankCenter)
			{
				m_visionCone.setRotation(m_visionCone.getRotation() + 1);
			}
			m_visionCone.setRotation({ t_tankCenter });

			break;
		case AIState::Player_Detected:
			for (int i = 0; i < 3; i++)
			{
				m_visionCone.setPoint(i, detectionCone[i]);
			}
			m_visionCone.setRotation(m_visionCone.getRotation() + 1);
			break;
		case AISTATE::
	}
}
