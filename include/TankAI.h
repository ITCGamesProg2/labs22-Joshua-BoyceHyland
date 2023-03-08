#pragma once
#include "MathUtility.h"
#include "Tank.h"
#include "EnemyStates.h"
#include "VisionCone.h"
#include <SFML/Graphics.hpp>
#include <Thor/Vectors.hpp>
#include <iostream>
#include <queue>



class TankAi
{
public:

	

	/// <summary>
	/// @brief Constructor that stores a reference to the obstacle container.
	/// Initialises steering behaviour to seek (player) mode, sets the AI tank position and
	///  initialises the steering vector to (0,0) meaning zero force magnitude.
	/// </summary>
	/// <param name="texture">A reference to the sprite sheet texture</param>
	///< param name="wallSprites">A reference to the container of wall sprites</param>
	TankAi(sf::Texture const & texture, std::vector<sf::Sprite> & wallSprites);

	/// <summary>
	/// @brief Steers the AI tank towards the player tank avoiding obstacles along the way.
	/// Gets a vector to the player tank and sets steering and velocity vectors towards
	/// the player if current behaviour is seek. If behaviour is stop, the velocity vector
	/// is set to 0. Then compute the correct rotation angle to point towards the player tank. 
	/// If the distance to the player tank is < MAX_SEE_AHEAD, then the behaviour is changed from seek to stop.
	/// Finally, recalculate the new position of the tank base and turret sprites.
	/// </summary>
	/// <param name="playerTank">A reference to the player tank</param>
	/// <param name="dt">update delta time</param>
	void update(Tank const & playerTank, double dt);

	/// <summary>
	/// @brief Draws the tank base and turret.
	///
	/// </summary>
	/// <param name="window">The SFML Render window</param>
	void render(sf::RenderWindow & window);

	/// <summary>
	/// @brief Initialises the obstacle container and sets the tank base/turret sprites to the specified position.
	/// <param name="position">An x,y position</param>
	/// </summary>
	void init(sf::Vector2f position);


	/// <summary>
/// @brief Checks for collision between the AI and player tanks.
///
/// </summary>
/// <param name="player">The player tank instance</param>
/// <returns>True if collision detected between AI and player tanks.</returns>
	bool collidesWithPlayer(Tank const& playerTank) const;


	void applyDamage(int t_damageAmount);

	sf::Sprite getTankBase();

	void reset();

	bool IsAlive();
	enum class AiType
	{
		AI_ID_NONE,
		AI_ID_SEEK_SHOOT_AT_PLAYER
	};

private:
	void initSprites();

	void updateMovement(double dt);

	sf::Vector2f seek(sf::Vector2f playerPosition) const;

	sf::Vector2f collisionAvoidance();

	/// <summary>
	/// updates all 3 of the tanks heads which what out for collisions 
	/// </summary>
	void updateHeads();

	/// <summary>
	/// find the most threating circle shape (wall) so it can avoid it
	/// </summary>
	/// <returns></returns>
	const sf::CircleShape findMostThreateningObstacle();

	/// <summary>
	/// lets the ai tank know that there is a collision happening on its left or right side
	/// </summary>
	/// <param name="t_ahead"></param>
	/// <param name="t_halfAhead"></param>
	/// <returns></returns>
	const bool isColliding(sf::Vector2f t_ahead, sf::Vector2f t_halfAhead);

	/// <summary>
	/// generates a location within the a range of the tanks designated area
	/// all tanks will have a start area to which they will be somewhat be bound to 
	/// to avoid a cluster of tanks in one part of the map
	/// </summary>
	/// <returns></returns>
	sf::Vector2f randomPatrolLocation();

	/// <summary>
	/// checks if the tank has come into range of its randomly generated patrol target
	/// </summary>
	/// <returns>whether it has reached target</returns>
	bool checkForTargetReached();
	
	/// <summary>
	/// checks if vision cone has collided with player
	/// </summary>
	/// <returns>whether the cone has collided</returns>
	bool coneCollisionWithPlayer(sf::Vector2f t_playerPosition);

	// A reference to the sprite sheet texture.
	sf::Texture const & m_texture;

	// A sprite for the tank base.
	sf::Sprite m_tankBase;

	// A sprite for the turret
	sf::Sprite m_turret;

	// shave for detection cone
	

	// A reference to the container of wall sprites.
	std::vector<sf::Sprite> & m_wallSprites;

	// The current rotation as applied to tank base and turret.
	double m_rotation{ 0.0 };

	// Current velocity.
	sf::Vector2f m_velocity;

	// Steering vector.
	sf::Vector2f m_steering;

	// The ahead vector for front of tank.
	sf::Vector2f m_aheadFront;

	// The half-ahead vector  for front of tank.
	sf::Vector2f m_halfAheadFront;

	// The ahead vector  for Right of tank.
	sf::Vector2f m_aheadRight; 

	// The half-ahead vector  for Right of tank.
	sf::Vector2f m_halfAheadRight; 

	// The ahead vector  for Right of tank.
	sf::Vector2f m_aheadLeft;

	// The half-ahead vector  for Right of tank.
	sf::Vector2f m_halfAheadLeft;

	sf::Vector2f heads[3][2]; 

	// The maximum see ahead range.
	static float constexpr MAX_SEE_AHEAD{ 50.0f };

	// The maximum avoidance turn rate.
	static float constexpr MAX_AVOID_FORCE{ 50.0f };

	// 
	static float constexpr MAX_FORCE{ 10.0f };

	// The maximum speed for this tank.
	float MAX_SPEED = 50.0f;

	// A container of circles that represent the obstacles to avoid.
	std::vector<sf::CircleShape> m_obstacles;

	
	bool m_headOnCollision{ false };

	

	sf::Vector2f m_avoidance{ 0,0 };

	// mass of the tank to add to the turning 
	const float MASS{ 5.0f };
	
	// the location tank sstarts at, member variable for use of generating random targets in a range
	sf::Vector2f m_startLocation;

	// tanks current state
	AIState m_currentState;

	// the tanks range of detection
	VisionCone m_visionCone;
	
	int health = 5;

	// variables for patrolling 
	sf::Vector2f m_patrolDestination;
	bool m_reachedPatrolTarget{true};



	

	
};

