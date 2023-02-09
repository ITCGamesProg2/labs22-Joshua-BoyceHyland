#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Math.hpp>
#include "CollisionDetector.h"
#include "Bullet.h"
#include "Target.h"
#include "Pool.h"
#include "SpatialParticion.h"
/// <summary>
/// @brief A simple tank controller.
/// 
/// This class will manage all tank movement and rotations.
/// </summary>
class Tank
{
public:
	/// <summary>
/// @brief Constructor that stores drawable state (texture, sprite) for the tank.
/// Stores references to the texture and container of wall sprites. 
/// Creates sprites for the tank base and turret from the supplied texture.
/// </summary>
/// <param name="t_texture">A reference to the sprite sheet texture</param>
///< param name="t_wallSprites">A reference to the container of wall sprites</param>  
	Tank(sf::Texture const& texture, std::vector<sf::Sprite>& t_wallSprites, std::vector<Target>& t_target);
	void update(double dt);
	void render(sf::RenderWindow& window);
	void setPosition(sf::Vector2f t_position);

	/// <summary>
	/// @brief Increases the speed by 1.
	/// </summary>
	void increaseSpeed();

	/// <summary>
	/// @brief Decreases the speed by 1.
	/// </summary>
	void decreaseSpeed();

	/// <summary>
	/// @brief Increases the rotation by 1 degree, wraps to 0 degrees after 359.
	/// </summary>
	void increaseRotation();

	/// <summary>
	/// @brief Decreases the rotation by 1 degree, wraps to 359 degrees after 0. 
	/// </summary>
	void decreaseRotation();

	/// <summary>
	/// processed controle keys and applies speed /rotation as appropiatee
	/// </summary>
	void handleKeyInput();

	/// <summary>
	/// increases rotation the turret on its own
	/// </summary>
	void increaseTurretRotation();


	/// <summary>
	/// decreases the rotation turret on its own
	/// </summary>
	void decreaseTurretRotation();

	/// <summary>
	/// centers the turret to allign with base
	/// </summary>
	void centreTurret();

	/// <summary>
	/// checs for collision between the walls and the tank 
	/// </summary>
	/// <returns>true if there is a collison between tanks and one of the wall</returns>
	bool checkCWallCollision();

	/// <summary>
	/// handles the collisions between bullets and sprites
	/// </summary>
	void checkBulletCollisions();

	/// <summary>
	/// /// @brief Stops the tank if moving and applies a small increase in speed in the opposite direction of travel.
	/// If the tank speed is currently 0, the rotation is set to a value that is less than the previous rotation value
	///  (scenario: tank is stopped and rotates into a wall, so it gets rotated towards the opposite direction).
	/// If the tank is moving, further rotations are disabled and the previous tank position is restored.
	/// The tank speed is adjusted so that it will travel slowly in the opposite direction. The tank rotation 
	///  is also adjusted as above if necessary (scenario: tank is both moving and rotating, upon wall collision it's 
	///  speed is reversed but with a smaller magnitude, while it is rotated in the opposite direction of it's 
	///  pre-collision rotation).
	/// </summary>
	void deflect();

	/// <summary>
	/// return score to score text in game.cpp so it can be displayed
	/// </summary>
	/// <returns>current score, corresponding to the amount of targets shot</returns>
	int getScore();

	/// <summary>
	/// returns the average of bullets shot to targets hit
	/// </summary>
	/// <returns></returns>
	float getAccuracy() const;

	/// <summary>
	/// tells game to display text notiffying player to repair turret
	/// </summary>
	bool needsRepair(); 

	/// <summary>
	/// reads the position of the tank
	/// </summary>
	/// <returns>tank current position</returns>
	sf::Vector2f getPosition() const; 

private:
	void initSprites();

	// tank variables 
	sf::Sprite m_tankBase;
	double m_speed{ 0.0 };// The tank speed.
	double m_tankRotation{ 0.0 };// The current rotation as applied to tank base.
	sf::Vector2f m_position;

	// turret variables 
	sf::Sprite m_turret;
	double m_turretRotation{ 0.0 };
	bool centering{ false };

	// colliosion variables
	bool m_enableRotation{ true };
	sf::Vector2f m_previousPosition;
	double m_previousSpeed;
	double m_previousRotation;

	// reference to containter of the wall sprites 
	std::vector<sf::Sprite>& m_wallSprites;
	sf::Texture const& m_texture;

	double const DEG_TO_RAD = thor::Pi / 180.0f;
	double const MAX_FORWARD_SPEED{ 100 };
	double const MAX_REVERSE_SPEED{- 100};

	// shooting variables
	Pool m_bulletPool;
	int const NUM_OF_BULLETS = 12;

	// targets 
	std::vector<Target> &m_target;

	// variable for score text in game
	float m_score = 0; 
	int m_bulletsFired = 0;
	
	// repairs varaibles
	bool m_turretRequiresRepair{ false };
	int m_repairsMade = 30; 

	SpatialParticion grid;
};
