#include "VisionCone.h"

VisionCone::VisionCone()
{
	m_visionCone.setPointCount(8);
	// triangle


	m_visionCone.setPoint(0, { 0,0 });
	m_visionCone.setPoint(1, { 200, -50 });
	m_visionCone.setPoint(2, { 200, 50 });


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
	//m_visionCone.setRotation(tankCenter);


	if (m_visionCone.getRotation() == 0)
	{
		m_visionCone.setRotation(359);
	}
	switch (t_aiState)
	{
	case AIState::Patrol_Map:
		
		float coneRotation = m_visionCone.getRotation();
		
			
		if ( coneRotation > tankCenter + 30)// && m_visionCone.getRotation() > tankCenter + 30)
		{
			rotateRight = false;
		}
		else if (coneRotation < tankCenter - 30)
		{
			rotateRight = true;
		}

		if (rotateRight)
		{
			m_visionCone.setRotation(m_visionCone.getRotation() + 0.5);
		}
		else
		{
			m_visionCone.setRotation(m_visionCone.getRotation() - 0.5);
		}


		break;
	}
}
