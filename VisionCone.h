#pragma once
#include <SFML/Graphics.hpp>

#include "EnemyStates.h"
class VisionCone
{
	public:
		VisionCone();
		void draw(sf::RenderWindow& t_window);
		void update(AIState& t_aiState, sf::Vector2f t_tankPosition, float t_tankCenter);
	private:
		sf::ConvexShape m_visionCone;

		sf::Vector2f patrolCone[3] = {
			{ 0,0 },
			{ 200, -50 },
			{ 200, 50 }
		};

		sf::Vector2f detectionCone[3] = {
			{ 0,0 },
			{ 280, -50 },
			{ 280, 50 }
		};

		sf::Vector2f attackCone[3] = {

		};
		float coneRotation;

		int tankCenter;
		bool rotateRight{ true };
};