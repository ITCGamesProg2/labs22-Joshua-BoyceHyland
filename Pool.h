#pragma once
#include "Bullet.h"
class Pool
{
public:
	Pool(int t_delayBetweenBullets = 20) :m_tillCanBeShot(t_delayBetweenBullets) {};
	void draw(sf::RenderWindow& t_window);
	void update();
	bool setStartOfNextAvailableBullet(sf::Vector2f t_tankPosition, double t_turretRotation);
	Bullet &getBullet(int t_i);
private:


	int const NUM_OF_BULLETS = 12;
	Bullet bullets[12];
	bool m_readyToShoot{ true };
	int m_tillCanBeShot = 20;
};