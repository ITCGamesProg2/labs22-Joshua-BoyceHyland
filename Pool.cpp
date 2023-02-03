#include "Pool.h"

void Pool::draw(sf::RenderWindow& t_window)
{
	for (int i = 0; i < NUM_OF_BULLETS; i++)
	{
		bullets[i].draw(t_window);
	}
}

void Pool::update()
{
	for (int i = 0; i < NUM_OF_BULLETS; i++)
	{
		bullets[i].move();
	}

	if (!m_readyToShoot)
	{
		m_tillCanBeShot--;
		if (m_tillCanBeShot < 0)
		{
			m_readyToShoot = true;
		}
	}
	

}

bool Pool::setStartOfNextAvailableBullet(sf::Vector2f t_tankPosition, double t_turretRotation)
{
	if (m_readyToShoot)
	{
		for (int i = 0; i < NUM_OF_BULLETS; i++)
		{
			if (bullets[i].canSetStart(t_tankPosition, t_turretRotation))
			{
				m_tillCanBeShot = 40;
				m_readyToShoot = false;

				return true;

			}
		}
	}

	return false;
}

Bullet& Pool::getBullet(int t_i)
{
	return bullets[t_i];
}



