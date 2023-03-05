#include "TankAi.h"

////////////////////////////////////////////////////////////
TankAi::TankAi(sf::Texture const & texture, std::vector<sf::Sprite> & wallSprites)
	: m_aiBehaviour(AiBehaviour::SEEK_PLAYER)
	, m_texture(texture)
	, m_wallSprites(wallSprites)
	, m_steering(0, 0)
	, visionCone()
{
	// Initialises the tank base and turret sprites.
	initSprites();
	
	

	currentState = AIState::Patrol_Map;
	
}

////////////////////////////////////////////////////////////
void TankAi::update(Tank const & playerTank, double dt)
{

	visionCone.update(currentState, m_tankBase.getPosition(), m_tankBase.getRotation());
	sf::Vector2f vectorToPlayer = seek(playerTank.getPosition());
	sf::Vector2f acceleration;
	bool rightCollision;
	bool leftCollision;
	switch (m_aiBehaviour)
	{
	case AiBehaviour::SEEK_PLAYER:



		m_avoidance = collisionAvoidance();
		m_steering += thor::unitVector(vectorToPlayer);
		m_steering += m_avoidance;
		m_steering = MathUtility::truncate(m_steering, MAX_FORCE);

		acceleration = m_steering / MASS;
		m_velocity = MathUtility::truncate(m_velocity + m_steering, MAX_SPEED);
		//m_velocity = MathUtility::truncate(m_velocity + acceleration, MAX_SPEED);

		rightCollision = isColliding(m_aheadRight, m_aheadRight);
		leftCollision = isColliding(m_aheadLeft, m_aheadLeft);

		//std::cout << "seeking " << std::endl;



		if ((rightCollision || leftCollision))
		{
			m_steering = thor::unitVector(vectorToPlayer);
			m_steering = MathUtility::truncate(m_steering, MAX_FORCE);
			m_velocity = MathUtility::truncate(m_velocity + m_steering, MAX_SPEED);
			m_aiBehaviour = AiBehaviour::STRAIGHTEN;
		}

		break;

	case AiBehaviour::STRAIGHTEN:
		updateHeads();

		rightCollision = isColliding(m_aheadRight, m_aheadRight);
		leftCollision = isColliding(m_aheadLeft, m_aheadLeft);
		m_headOnCollision = isColliding(m_aheadFront, m_halfAheadFront);

		//std::cout << "Straighting " << std::endl; 	


		//if (!m_headOnCollision ||(!rightCollision && !leftCollision) )   // m_headOnCollision provide decen
		//{
		//	m_aiBehaviour = AiBehaviour::SEEK_PLAYER;
		//}

		if (!m_headOnCollision && !rightCollision && !leftCollision)   // m_headOnCollision provide decen
		{
			m_aiBehaviour = AiBehaviour::SEEK_PLAYER;
		}
		else if (m_headOnCollision)
		{
			m_aiBehaviour = AiBehaviour::SEEK_PLAYER;
		}

		break;
	case AiBehaviour::STOP:
		m_velocity = sf::Vector2f(0, 0);
		//motion->m_speed = 0;
	default:
		break;
	}
	

	// Now we need to convert our velocity vector into a rotation angle between 0 and 359 degrees.
	// The m_velocity vector works like this: vector(1,0) is 0 degrees, while vector(0, 1) is 90 degrees.
	// So for example, 223 degrees would be a clockwise offset from 0 degrees (i.e. along x axis).
	// Note: we add 180 degrees below to convert the final angle into a range 0 to 359 instead of -PI to +PI
	auto dest = atan2(-1 * m_velocity.y, -1 * m_velocity.x) / thor::Pi * 180 + 180;

	auto currentRotation = m_rotation;

	// Find the shortest way to rotate towards the player (clockwise or anti-clockwise)
	if (std::round(currentRotation - dest) == 0.0)
	{
		m_steering.x = 0;
		m_steering.y = 0;
	}

	else if ((static_cast<int>(std::round(dest - currentRotation + 360))) % 360 < 180)
	{
		// rotate clockwise
		m_rotation = static_cast<int>((m_rotation) + 1) % 360;
	}
	else
	{
		// rotate anti-clockwise
		m_rotation = static_cast<int>((m_rotation) - 1) % 360;
	}


	if (thor::length(vectorToPlayer) < MAX_SEE_AHEAD)
	{
		m_aiBehaviour = AiBehaviour::STOP;
	}
	else if (m_aiBehaviour == AiBehaviour::STRAIGHTEN)
	{

	}
	else
	{
		m_aiBehaviour = AiBehaviour::SEEK_PLAYER;
	}

	updateMovement(dt);
	manageCone();
}

void TankAi::manageCone()
{

	
}
////////////////////////////////////////////////////////////
void TankAi::render(sf::RenderWindow & window)
{
	// TODO: Don't draw if off-screen...
	//for (sf::CircleShape circle : m_obstacles)
	//{
	//	window.draw(circle);
	//}
	//sf::CircleShape ahead(10);
	//ahead.setPosition(m_aheadFront);
	//window.draw(ahead);

	//sf::CircleShape aheadRight(10);
	//aheadRight.setPosition(m_aheadRight);
	//window.draw(aheadRight);

	//sf::CircleShape aheadLeft(10);
	//aheadLeft.setPosition(m_aheadLeft);
	//window.draw(aheadLeft);
	
	visionCone.draw(window);
	window.draw(m_tankBase);
	window.draw(m_turret);
	
	
}

////////////////////////////////////////////////////////////
void TankAi::init(sf::Vector2f position)
{
	m_tankBase.setPosition(position);
	m_turret.setPosition(position);

	for (sf::Sprite const wallSprite : m_wallSprites)
	{
		sf::CircleShape circle(wallSprite.getTextureRect().width * 1.0f);
		circle.setOrigin(circle.getRadius(), circle.getRadius());
		circle.setPosition(wallSprite.getPosition());
		m_obstacles.push_back(circle);
	}
}

bool TankAi::collidesWithPlayer(Tank const& playerTank) const
{
	// checks if the AI tank has collided with the player tank
	if (CollisionDetector::collision(m_turret, playerTank.getTurret()) ||
		CollisionDetector::collision(m_tankBase, playerTank.getBase()))
	{
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////
sf::Vector2f TankAi::seek(sf::Vector2f playerPosition) const
{
	return playerPosition - m_tankBase.getPosition();
}

////////////////////////////////////////////////////////////
sf::Vector2f TankAi::collisionAvoidance()
{
	updateHeads();

	const sf::CircleShape mostThreatening = findMostThreateningObstacle();
	
	sf::Vector2f avoidance = { 0, 0 };
	
	
	if ((mostThreatening.getRadius() != 0.0))
	{
		avoidance.x = m_aheadFront.x - mostThreatening.getPosition().x;
		avoidance.y = m_aheadFront.y - mostThreatening.getPosition().y;
		avoidance = thor::unitVector(avoidance);
		avoidance *= MAX_AVOID_FORCE;
		m_headOnCollision = true;
	}
	else
	{
		m_headOnCollision = false;
		avoidance *= 0.0f;
	}


	return avoidance;
}

void TankAi::updateHeads()
{
	auto headingRadians = thor::toRadian(m_rotation);
	float headDifferences = 1.57079633;

	sf::Vector2f headingVectorFront(std::cos(headingRadians) * MAX_SEE_AHEAD, std::sin(headingRadians) * MAX_SEE_AHEAD);
	m_aheadFront = m_tankBase.getPosition() + headingVectorFront;
	m_halfAheadFront = m_tankBase.getPosition() + (headingVectorFront * 0.5f);

	sf::Vector2f headingVectorRight(std::cos(headingRadians + headDifferences) * MAX_SEE_AHEAD, std::sin(headingRadians + headDifferences) * MAX_SEE_AHEAD);
	m_aheadRight = m_tankBase.getPosition() + headingVectorRight;
	m_halfAheadRight = m_tankBase.getPosition() + (headingVectorRight * 0.5f);

	sf::Vector2f headingVectorLeft(std::cos(headingRadians - headDifferences) * MAX_SEE_AHEAD, std::sin(headingRadians - headDifferences) * MAX_SEE_AHEAD);
	m_aheadLeft = m_tankBase.getPosition() + headingVectorLeft;
	m_halfAheadLeft = m_tankBase.getPosition() - (headingVectorLeft * 0.5f);
}

////////////////////////////////////////////////////////////
const sf::CircleShape TankAi::findMostThreateningObstacle()
{
	sf::CircleShape mostThreatening(0);

	for (int i = 0; i < m_obstacles.size(); i++)
	{

		float distanceBetweenCurrent = MathUtility::distance(m_tankBase.getPosition(), m_obstacles[i].getPosition());

		// only checks to assign if the obstacle is close to it  or if buy chanch has gone over it
		if (distanceBetweenCurrent > m_obstacles[i].getRadius() * 0.95 || distanceBetweenCurrent < m_obstacles[i].getRadius() * 1.05)
		{
			bool headIntersectsCircle = MathUtility::lineIntersectsCircle(m_aheadFront, m_halfAheadFront, m_obstacles[i]);
			bool radiusisZero = mostThreatening.getRadius() == 0;
			bool isCloserThanPreviousObstacle = distanceBetweenCurrent < (MathUtility::distance(m_tankBase.getPosition(), mostThreatening.getPosition()));

			if (headIntersectsCircle && (radiusisZero || isCloserThanPreviousObstacle))
			{
				mostThreatening = m_obstacles[i];
			}
		}

	}

	//if(right)
	
	return mostThreatening;
}

const bool TankAi::isColliding(sf::Vector2f t_ahead, sf::Vector2f t_halfAhead)
{
	for (int i = 0; i < m_obstacles.size(); i++)
	{
		float distanceBetweenCurrent = MathUtility::distance(m_tankBase.getPosition(), m_obstacles[i].getPosition());
		// only checks to assign if the obstacle is close to it  or if buy chanch has gone over it
		if (distanceBetweenCurrent > m_obstacles[i].getRadius() * 0.95 || distanceBetweenCurrent < m_obstacles[i].getRadius() * 1.05)
		{
			bool headIntersectsCircle = MathUtility::lineIntersectsCircle(t_ahead, t_halfAhead, m_obstacles[i]);
			
			if (headIntersectsCircle )
			{
				{
					return true;
				}
			}
		}

	}

	return false;
}

void TankAi::applyDamage(int t_damageAmount)
{
	health -= t_damageAmount;
}

sf::Sprite TankAi::getTankBase()
{
	return m_tankBase;
}

void TankAi::reset()
{
	health = 5;
}

bool TankAi::IsAlive()
{
	return health>=0;
}

////////////////////////////////////////////////////////////
void TankAi::initSprites()
{
	// Initialise the tank base
	m_tankBase.setTexture(m_texture);
	sf::IntRect baseRect(103, 43, 79, 43);
	m_tankBase.setTextureRect(baseRect);
	m_tankBase.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);

	// Initialise the turret
	m_turret.setTexture(m_texture);
	sf::IntRect turretRect(122, 1, 83, 31);
	m_turret.setTextureRect(turretRect);
	m_turret.setOrigin(turretRect.width / 3.0, turretRect.height / 2.0);
}


////////////////////////////////////////////////////////////
void TankAi::updateMovement(double dt)
{
	double speed = thor::length(m_velocity);
	sf::Vector2f newPos(m_tankBase.getPosition().x + std::cos(MathUtility::DEG_TO_RAD  * m_rotation) * speed * (dt / 1000),
						m_tankBase.getPosition().y + std::sin(MathUtility::DEG_TO_RAD  * m_rotation) * speed * (dt / 1000));
	m_tankBase.setPosition(newPos.x, newPos.y);
	m_tankBase.setRotation(m_rotation);
	m_turret.setPosition(m_tankBase.getPosition());
	m_turret.setRotation(m_rotation);
}