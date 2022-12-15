#pragma once
#include <SFML/Graphics.hpp>
#include "Thor/Vectors/VectorAlgebra2D.hpp"
#include <iostream>
class Bullet {

public:

	Bullet();

	/// <summary>
	/// sets the starting position and rotation of the sprite while the bullet has not been already shot
	/// </summary>
	/// <param name="t_playerPosition"> the </param>
	/// <param name="t_playerRotation"> current rotation of the tank</param>
	///	<returns>whether the bullet was able to be shot</returns>
	bool canSetStart(sf::Vector2f t_playerPosition, float t_playerRotation);
	
	/// <summary>
	/// adds the speed to the current position of the bullet if it has been shot
	/// </summary>
	void move();

	/// <summary>
	/// draws the sprites with a reference to the window 
	/// </summary>
	/// <param name="t_window">referenced game window</param>
	void draw(sf::RenderWindow& t_window);

	/// <summary>
	/// sets the bullet to not shot and moves it off screen
	/// </summary>
	void despawn();

	/// <summary>
	/// return the sprite of the bullet
	/// </summary>
	/// <returns>bullet sprite</returns>
	sf::Sprite getBody() const;
	

private:
	sf::Sprite bulletBody;
	sf::Texture bulletTexture;
	sf::Vector2f m_speed{ 20.0f, 20.0f };
	sf::Vector2f m_position;
	sf::Vector2f m_bodySize{ 10.0f,10.0f };
	bool beenShot = false; // needles get rid of it 
	double const DEG_TO_RAD = thor::Pi / 180.0f;

};

