#pragma once

#include "Tank.h"
#include "CollisionDetector.h"


class BackPack
{
public:
	BackPack();
	void draw(sf::RenderWindow& t_window); ;
	bool collisionCheck(sf::Sprite t_playerTankBase);
private:
	sf::Sprite m_body;
	sf::Texture m_texture;
};