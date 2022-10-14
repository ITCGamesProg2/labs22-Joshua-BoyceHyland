#pragma once
#include <SFML/Graphics.hpp>

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
	
private:
	void initSprites(sf::Vector2f pos);

	sf::Sprite m_tankBase;
	sf::Sprite m_turret;
	sf::Texture const & m_texture;
		
};
