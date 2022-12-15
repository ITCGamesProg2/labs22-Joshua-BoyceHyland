#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class Target
{
public:
	/// <summary>
	/// sets up the texture, assigns position amd the offset of the target
	/// </summary>
	/// <param name="t_texture">texture loaded for target in game.cpp</param>
	/// <param name="t_position"> position loaded from yaml file in game.cpp</param>
	/// <param name="t_float"> offset loaded from yaml file in gamecpp</param>
	Target(sf::Texture const& t_texture, sf::Vector2f t_position, float t_float);

	/// <summary>
	/// takes in reerence to window and draws the target sprite
	/// </summary>
	/// <param name="t_window">reference to game window</param>
	void draw(sf::RenderWindow& t_window);

	/// <summary>
	/// sets alive to false, sets beenShot to true so the target doesn't respawn
	/// checks how much time the target had remaing and assigns it to the static variable leftover to be used by the next target  
	/// </summary>
	void despawn();

	/// <summary>
	/// sets alive to true
	/// generates a offset (if any )to be applied to the position, sets position
	/// adds on the additional time if there was anyleft over from the last target
	/// also rests blinking incase the character timed out and was blinking when it despawned, without this variable would keep blinking
	/// </summary>
	void respawn();

	/// <summary>
	/// updates the position  of the target to the passed parameter 
	/// </summary>
	/// <param name="t_position"> passed in posiition</param>
	void setPosition(sf::Vector2f t_position);

	/// <summary>
	/// sets the offset 
	/// </summary>
	/// <param name="t_offSet">passed through float to be set as offset</param>
	/// <returns></returns>
	void setOffSet(float t_offSet);

	/// <summary>
	/// primarly used in tank for hit detection 
	/// </summary>
	/// <returns>classes sprite</returns>
	sf::Sprite getBody();

	/// <summary>
	/// checks whether character is alive
	/// primary use for respawn and to check whether it can be shot
	/// </summary>
	/// <returns> alive variable</returns>
	bool isAlive();

	/// <summary>
	/// checks whether character has already been shot
	/// used to determine if it should be respawned
	/// if has been shot will not to avoid targets popping up in same place frequently 
	/// </summary>
	/// <returns> beenShot bool </returns>
	bool beenShot();

	/// <summary>
	/// manages the targets timers
	/// whether it should begin to blink 
	/// whether it has reached it time to be despawned
	/// </summary>
	void updateTimer();

	

private:

	sf::Sprite m_target;
	sf::Texture m_texture;

	sf::Vector2f m_position;
	float m_offSet{ 0 };

	// bool managing whether it should be drawn or respawned
	bool alive {false};
	bool m_beenShot{ false };

	// tagrets variables for management of it screen time 
	sf::Clock m_timedLife;
	sf::Time  m_lifeSpan{ sf::seconds(5) };
	sf::Time m_blinkingTime{ sf::seconds(2) };
	bool m_blinkingState{ false };
	static int m_leftOverTime;
};