#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Math.hpp>
/// <summary>
/// @brief A simple tank controller.
/// 
/// This class will manage all tank movement and rotations.
/// </summary>
class Tank
{
public:	
	Tank(sf::Texture const & texture, sf::Vector2f const pos);
	void update(double dt);
	void render(sf::RenderWindow & window);
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
	
	
private:
	void initSprites(sf::Vector2f pos);

	// tank variables 
	sf::Sprite m_tankBase;
	double m_speed{ 0.0 };// The tank speed.
	double m_tankRotation{ 0.0 };// The current rotation as applied to tank base.
	sf::Vector2f m_position{ 200.0, 200.0 }; 

	// turret variables 
	sf::Sprite m_turret;
	double m_turretRotation{ 0.0 }; 
	bool centering = false; 

	sf::Texture const & m_texture;
	
	double const DEG_TO_RAD = thor::Pi / 180.0f;
	double const MAX_FORWARD_SPEED = 100; 
	double const MAX_REVERSE_SPEED = -100; 
};
