#pragma once
#include <SFML/Graphics.hpp>

#include "EnemyStates.h"
class VisionCone
{
	public:
		VisionCone();
		void draw(sf::RenderWindow& t_window);
		void update(AIState& t_aiState, sf::Vector2f t_tankPosition, float t_tankCenter);
		sf::ConvexShape getShape();
		sf::Vector2f getCurrentPoint(AIState t_aiState, int t_desiredPoint);

	private:
		sf::ConvexShape m_visionCone;

		sf::Vector2f patrolCone[3] = {
			{ 0,0 },
			{ 200, -75 },
			{ 200, 75 }
		};

		sf::Vector2f detectionCone[3] = {
			{ 0,0 },
			{ 300, -50 },
			{ 300, 50 }
		};

		sf::Vector2f attackCone[3] = {
			{ 0,0 },
			{ 125, -125 },
			{ 125, 125}
		};
		float coneRotation;

		int tankCenter;
		bool rotateRight{ true };
};