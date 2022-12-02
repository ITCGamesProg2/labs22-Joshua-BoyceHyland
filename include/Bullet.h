#pragma once
#include <SFML/Graphics.hpp>
#include "Thor/Vectors/VectorAlgebra2D.hpp"
#include <iostream>
class Bullet {

public:

	Bullet();
	bool setStart(sf::Vector2f t_playerPosition, float t_playerRotation);
	void move();
	void draw(sf::RenderWindow& t_window);
	sf::Sprite getBody() const;
	bool checkCollisionAgainst(sf::RectangleShape t_enemy);
	bool checkCollisionAgainst2(sf::Sprite t_Enemy);

private:
	sf::Sprite bulletBody;
	sf::Texture bulletTexture;
	sf::Vector2f m_speed{ 20.0f, 20.0f };
	sf::Vector2f m_position;
	sf::Vector2f m_bodySize{ 10.0f,10.0f };
	bool beenShot = false; // needles get rid of it 
	bool m_readyToBeShot = false;

};

