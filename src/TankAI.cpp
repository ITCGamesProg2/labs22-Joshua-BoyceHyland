#include "TankAi.h"

////////////////////////////////////////////////////////////
TankAi::TankAi(sf::Texture const & texture, std::vector<sf::Sprite> & wallSprites)
	: m_currentState(AIState::Patrol_Map
)
	, m_texture(texture)
	, m_wallSprites(wallSprites)
	, m_steering(0, 0)
	, m_visionCone(), 
	m_bullets( 150)
{
	// Initialises the tank base and turret sprites.
	initSprites();
	
	m_startLocation = m_tankBase.getPosition();
	m_currentState = AIState::Patrol_Map;
	
}

////////////////////////////////////////////////////////////
void TankAi::update(std::function<void(float)>& t_decrementVisualHealth, Tank & playerTank, double dt)
{

	m_visionCone.update(m_currentState, m_tankBase.getPosition(), m_tankBase.getRotation());
	m_bullets.update();
	sf::Vector2f vectorToPlayer = seek(playerTank.getPosition());
	sf::Vector2f acceleration;
	bool rightCollision;
	bool leftCollision;

	switch (m_currentState)
	{
	case AIState::Patrol_Map:
		

		if (m_reachedPatrolTarget)
		{
			m_patrolDestination = randomPatrolLocation();
			m_reachedPatrolTarget = false;
		}
		

		m_avoidance = collisionAvoidance();
		m_steering += thor::unitVector(m_patrolDestination - m_tankBase.getPosition());
		m_steering += m_avoidance;
		m_steering = MathUtility::truncate(m_steering, MAX_FORCE);

		acceleration = m_steering / MASS;
		m_velocity = MathUtility::truncate(m_velocity + m_steering, MAX_SPEED);




		if (checkForTargetReached())
		{
			m_reachedPatrolTarget = true;
		}
		if (coneCollisionWithPlayer(playerTank))
		{
			std::cout << "You do be colliding tho" << std::endl;
			m_currentState = AIState::Player_Detected;
			m_statesTransitionTimer.restart();
		}
		
		break;

	case AIState::Player_Detected:



		m_avoidance = collisionAvoidance();
		m_steering += thor::unitVector(vectorToPlayer);
		m_steering += m_avoidance;
		m_steering = MathUtility::truncate(m_steering, MAX_FORCE);

		acceleration = m_steering / MASS;
		m_velocity = MathUtility::truncate(m_velocity + m_steering, MAX_SPEED);
		//m_velocity = MathUtility::truncate(m_velocity + acceleration, MAX_SPEED);

		rightCollision = isColliding(m_aheadRight, m_aheadRight);
		leftCollision = isColliding(m_aheadLeft, m_aheadLeft);



		if (!coneCollisionWithPlayer(playerTank) && m_statesTransitionTimer.getElapsedTime().asSeconds() > 2)
		{
			std::cout << "You do be colliding tho" << std::endl;
			m_currentState = AIState::Patrol_Map;
		}

		/*if ((rightCollision || leftCollision))
		{
			m_steering = thor::unitVector(vectorToPlayer);
			m_steering = MathUtility::truncate(m_steering, MAX_FORCE);
			m_velocity = MathUtility::truncate(m_velocity + m_steering, MAX_SPEED);
			m_currentState = AIState::STRAIGHTEN;
		}*/


		break;
		case AIState::Attack_Player:
			m_avoidance = collisionAvoidance();
			m_steering += thor::unitVector(vectorToPlayer);
			m_steering += m_avoidance;
			m_steering = MathUtility::truncate(m_steering, MAX_FORCE);

			acceleration = m_steering / MASS;
			m_velocity = MathUtility::truncate(m_velocity + m_steering, MAX_SPEED);
			//m_velocity = MathUtility::truncate(m_velocity + acceleration, MAX_SPEED);

			rightCollision = isColliding(m_aheadRight, m_aheadRight);
			leftCollision = isColliding(m_aheadLeft, m_aheadLeft);

			if (!coneCollisionWithPlayer(playerTank) && m_statesTransitionTimer.getElapsedTime().asSeconds() > 2)
			{
				std::cout << "You do be colliding tho" << std::endl;
				m_currentState = AIState::Patrol_Map;
			}

			m_bullets.setStartOfNextAvailableBullet(m_tankBase.getPosition(), m_turret.getRotation());

			/*if ((rightCollision || leftCollision))
			{
				m_steering = thor::unitVector(vectorToPlayer);
				m_steering = MathUtility::truncate(m_steering, MAX_FORCE);
				m_velocity = MathUtility::truncate(m_velocity + m_steering, MAX_SPEED);
				m_currentState = AIState::STRAIGHTEN;
			}*/


			break;

	//case AIState::STRAIGHTEN:
	//	updateHeads();

	//	rightCollision = isColliding(m_aheadRight, m_aheadRight);
	//	leftCollision = isColliding(m_aheadLeft, m_aheadLeft);
	//	m_headOnCollision = isColliding(m_aheadFront, m_halfAheadFront);

	//	//std::cout << "Straighting " << std::endl; 	


	//	//if (!m_headOnCollision ||(!rightCollision && !leftCollision) )   // m_headOnCollision provide decen
	//	//{
	//	//	m_aiBehaviour = AiBehaviour::SEEK_PLAYER;
	//	//}

	//	if (!m_headOnCollision && !rightCollision && !leftCollision)   // m_headOnCollision provide decen
	//	{
	//		//currentState = AIState::SEEK_PLAYER;
	//	}
	//	else if (m_headOnCollision)
	//	{
	//		//currentState = AIState::SEEK_PLAYER;
	//	}

	//	break;
	case AIState::STOP:
		m_velocity = sf::Vector2f(0, 0);
		
		//motion->m_speed = 0;
		break; 
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


	if (thor::length(vectorToPlayer) < MAX_SEE_AHEAD && m_currentState == AIState::Player_Detected)
	{
		m_currentState = AIState::Attack_Player;
	}
	/*se if (currentState == AIState::STRAIGHTEN)
	{

	}*/
	/*else
	{
		currentState = AIState::SEEK_PLAYER;
	}*/

	updateMovement(dt);
	
	for (int i = 0; i < 12; i++)
	{

		if ((CollisionDetector::collision(m_bullets.getBullet(i).getBody(),playerTank.getBase())))
		{
			std::cout << "You hit the Enemy" << std::endl;
			m_bullets.getBullet(i).despawn();
			t_decrementVisualHealth(20);
			playerTank.decrementHealth(1);

		}
		else if(m_bullets.getBullet(i).getBody().getPosition().x < 0)
		{
			m_bullets.getBullet(i).despawn();
		}
		else if (m_bullets.getBullet(i).getBody().getPosition().x > 1440)
		{
			m_bullets.getBullet(i).despawn();
		}
		else if (m_bullets.getBullet(i).getBody().getPosition().y < 0)
		{
			m_bullets.getBullet(i).despawn();
		}
		else if (m_bullets.getBullet(i).getBody().getPosition().y > 1440)
		{
			m_bullets.getBullet(i).despawn();
		}

	}
	
}


sf::Vector2f TankAi::randomPatrolLocation()
{
	int screenWidth = 1440 ;

	int screenHeight =  900 ;

	int randYStartRange = m_startLocation.y - screenHeight / 2;
	int randYEndRange = m_startLocation.y + screenHeight / 2;


	int randX = rand() % screenWidth + 1 ;
	int randY = rand() % randYStartRange + randYEndRange;

	return sf::Vector2f(randX, randY);
}
bool TankAi::checkForTargetReached()
{
	float differenceX = m_patrolDestination.x - m_tankBase.getPosition().x;
	float differenceY = m_patrolDestination.y - m_tankBase.getPosition().y;

	if (differenceX < 10 && differenceY < 10)
	{
		return true;
	
	}
	return false;
}
bool TankAi::coneCollisionWithPlayer(Tank const& playerTank)
{
	int basePoint = 0;
	int left = 1;
	int right = 2;
	int sizeOfTankDistanceDifference = 7500;

	sf::Vector2f leftPoint = m_tankBase.getPosition() + (thor::rotatedVector(m_visionCone.getCurrentPoint(m_currentState, left), m_tankBase.getRotation()));
	sf::Vector2f rightPoint = m_tankBase.getPosition() + (thor::rotatedVector(m_visionCone.getCurrentPoint(m_currentState, right), m_tankBase.getRotation()));
	sf::Vector2f topOfCone = leftPoint;



	/*std::cout << "LeftPoint X: " << ((m_tankBase.getPosition().x - leftPoint.x) * (playerTank.getPosition().y - leftPoint.y) -
		(m_tankBase.getPosition().y - leftPoint.y) * (playerTank.getPosition().x - leftPoint.x)) << " Y: " << leftPoint.y << std::endl;
	std::cout << "RightPoint X: " << rightPoint.x << " Y: " << rightPoint.y << std::endl;*/
	
	
	bool hasPassLeftLine = ( (m_tankBase.getPosition().x - leftPoint.x  ) * (playerTank.getPosition().y - leftPoint.y) -
						     (m_tankBase.getPosition().y - leftPoint.y) * (playerTank.getPosition().x - leftPoint.x)        ) < sizeOfTankDistanceDifference;

	bool hasPassedRightLine = ( (m_tankBase.getPosition().x - rightPoint.x) * (playerTank.getPosition().y - rightPoint.y) -
							    (m_tankBase.getPosition().y - rightPoint.y) * (playerTank.getPosition().x - rightPoint.x)   ) > -sizeOfTankDistanceDifference;
	

	bool isWithinCone = ((rightPoint.x - leftPoint.x) * (playerTank.getPosition().y - leftPoint.y) -
						(rightPoint.y - leftPoint.y) * (playerTank.getPosition().x - leftPoint.x)) > -sizeOfTankDistanceDifference;
	
	
	return hasPassLeftLine && hasPassedRightLine && isWithinCone;
}
////////////////////////////////////////////////////////////
void TankAi::render(sf::RenderWindow & window)
{
	// TODO: Don't draw if off-screen...
	for (sf::CircleShape circle : m_obstacles)
	{
		window.draw(circle);
	}
	//sf::CircleShape ahead(10);
	//ahead.setPosition(m_aheadFront);
	//window.draw(ahead);

	//sf::CircleShape aheadRight(10);
	//aheadRight.setPosition(m_aheadRight);
	//window.draw(aheadRight);

	//sf::CircleShape aheadLeft(10);
	//aheadLeft.setPosition(m_aheadLeft);
	//window.draw(aheadLeft);
	
	m_visionCone.draw(window);
	window.draw(m_tankBase);
	m_bullets.draw(window);
	window.draw(m_turret);
	
}

////////////////////////////////////////////////////////////
void TankAi::init(sf::Vector2f position)
{
	m_tankBase.setPosition(position);
	m_turret.setPosition(position);

	for (sf::Sprite const wallSprite : m_wallSprites)
	{
		sf::CircleShape circle(wallSprite.getTextureRect().width * 1.25f);
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