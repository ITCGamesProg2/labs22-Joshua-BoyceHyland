#include "Game.h"
#include <iostream>

// Our target FPS
static double const FPS{ 60.0f };
////////////////////////////////////////////////////////////
Game::Game()
	: m_window(sf::VideoMode(ScreenSize::s_width, ScreenSize::s_height, 32), "SFML Playground", sf::Style::Default), 
	  m_tank(m_tankTexture,m_wallSprites,m_targets), 
	 m_aiTank(m_tankTexture, m_wallSprites),
	//m_aiTank2(m_tankTexture, m_wallSprites),
	 m_hud(m_font)//, 
	//grid(ScreenSize::s_width, ScreenSize::s_height)
{
	init();
	camerSetUp();
	m_aiTank.init(m_level.m_aiTank.m_position);
	//m_aiTank2.init({ScreenSize::s_width/2, 1500});

	// Point at TankAI::applyDamage()...this function expects 1 argument(damage amount), but that argument
		//  will be supplied later when we call the function inside Projectile::udpate()
		// So we use what is called a placeholder argument and this will be substituted later with the damage amount
	using std::placeholders::_1;
	// The parameters of bind are the function to be called, followed by the address of the target instance, 
	//  followed by the placeholder argument.
	m_funcApplyDamge = std::bind(&TankAi::applyDamage, &m_aiTank, _1);
	m_funcDecrementHudFuel = std::bind(&HUD::decrementFuelVisual, &m_hud, _1);
	m_funcDecrementHudHealth = std::bind(&HUD::decrementHealthVisual, &m_hud, _1);

}

////////////////////////////////////////////////////////////
void Game::init()
{
	int currentLevel = 1;

	// generates a exception if level loading fails.
	try
	{
		LevelLoader::load(currentLevel, m_level);
	}
	catch (std::exception& e)
	{
		std::cout << "Level Loading failure." << std::endl;
		std::cout << e.what() << std::endl;
		throw e;
	}
	// Really only necessary is our target FPS is greater than 60.
	m_window.setVerticalSyncEnabled(true);

	if (!m_arialFont.loadFromFile("BebasNeue.otf"))
	{
		std::cout << "Error loading font file";
	}

	if (!m_bgTexture.loadFromFile("./resources/images/Background.jpg"))
	{
		std::cout << "Can not load background" << std::endl;
	}
	m_bgTexture.setRepeated(true);
	m_bgSpritee.setTexture(m_bgTexture);
	m_bgSpritee.setTextureRect(sf::IntRect(0, 0, ScreenSize::s_width, 4000));

	// Load the player tank
	if (!m_tankTexture.loadFromFile("./resources/images/SpriteSheet.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}
	if (!m_targetTexture.loadFromFile("resources/images/target2.png"))
	{
		std::cout << "Could not load the texture for the target \n";
	}


	int randSpawn = rand() % m_level.m_tank.m_tankPositions.size();
	m_tank.setPosition(m_level.m_tank.m_tankPositions[0]);



	if (!m_spriteSheetTexture.loadFromFile("./resources/images/SpriteSheet.png"))
	{
		std::string errorMsg("Error loading texture");
		throw std::exception(errorMsg.c_str());
	}

	if (!m_font.loadFromFile("./resources/fonts/Merc.ttf"))
	{
		std::string fontErrorMsg("Error loading font");
		throw std::exception(fontErrorMsg.c_str());
	}

	float y = 600;
	for (int i = 0; i < 3; i++)
	{
		int randNumX = rand() % ScreenSize::s_width + 1 ;
		m_fuel[i].setPosition({ static_cast<float>(randNumX), y }); 
		y += 600; 
	}
	
	setUpText();
	setUpReturnPoint(); 
	generateWalls(); 
	generateTargets(); 
#ifdef TEST_FPS
	x_updateFPS.setFont(m_arialFont);
	x_updateFPS.setPosition(20, 300);
	x_updateFPS.setCharacterSize(24);
	x_updateFPS.setFillColor(sf::Color::White);
	x_drawFPS.setFont(m_arialFont);
	x_drawFPS.setPosition(20, 350);
	x_drawFPS.setCharacterSize(24);
	x_drawFPS.setFillColor(sf::Color::White);
#endif
}

////////////////////////////////////////////////////////////
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	
	sf::Time timePerFrame = sf::seconds(1.0f / FPS); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame.asMilliseconds()); //60 fps
			
#ifdef TEST_FPS
			x_secondTime += timePerFrame;
			x_updateFrameCount++;
			if (x_secondTime.asSeconds() > 1)
			{
				std::string updatesPS = "UPS " + std::to_string(x_updateFrameCount - 1);
				x_updateFPS.setString(updatesPS);
				std::string drawsPS = "DPS " + std::to_string(x_drawFrameCount);
				x_drawFPS.setString(drawsPS);
				x_updateFrameCount = 0;
				x_drawFrameCount = 0;
				x_secondTime = sf::Time::Zero;
			}
#endif
		}
		render(); // as many as possible
#ifdef TEST_FPS
		x_drawFrameCount++;
#endif
	}
}

////////////////////////////////////////////////////////////
void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
		processGameEvents(event);
	}
}

////////////////////////////////////////////////////////////
void Game::processGameEvents(sf::Event& event)
{
	// check if the event is a a mouse button release
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		m_window.close();
	}

	if (m_currentGameState == Menu)
	{
		enterUserInfo(event); 
	}

	
	
}

void Game::generateWalls()
{
	sf::IntRect wallRect(2, 129, 33, 23);
	
	//creates the walls
	// auto means compiler with deduce corrrect type for the variable obstacle, this case being obstacle data
	for (auto const &obstacle : m_level.m_obstacles)
	{
		sf::Sprite sprite; 
		sprite.setTexture(m_spriteSheetTexture);
		sprite.setTextureRect(wallRect);
		sprite.setOrigin(wallRect.width / 2.0, wallRect.height / 2.0);
		sprite.setPosition(obstacle.m_position);
		sprite.setRotation(obstacle.m_rotation);
		m_wallSprites.push_back(sprite); 
	}
}

void Game::generateTargets()
{
	for (auto const& targetData : m_level.m_targetData)
	{
		Target target(m_targetTexture, targetData.m_position, targetData.m_offset);
		m_targets.push_back(target);
	}
}

void Game::timerUpdate()
{
	sf::Time elapsedTime = m_clock.getElapsedTime(); // gets time passed
	int displayedTime = m_timer.asSeconds() - elapsedTime.asSeconds(); // truncates to int for display 
	m_timerText.setString("Timer: " + std::to_string(displayedTime));

	// game over criteria 
	if (displayedTime < 1)
	{
		m_currentGameState = UpdateYAML;;
	}
}
	

void Game::setUpText()
{


	m_userInput.setFont(m_font);
	m_userInput.setFillColor(sf::Color::White);
	m_userInput.setCharacterSize(80u);
	m_userInput.setPosition(100, (ScreenSize::s_height / 2) - 100);
	m_userInput.setString("Please enter username: ");

	m_timerText.setFont(m_font);
	m_timerText.setFillColor(sf::Color::White);
	m_timerText.setCharacterSize(80u);
	m_timerText.setPosition(20, 0);

	m_scoreText.setFont(m_font);
	m_scoreText.setFillColor(sf::Color::White);
	m_scoreText.setCharacterSize(80u);
	m_scoreText.setPosition(20, 80);

	m_repairNotification.setFont(m_font);
	m_repairNotification.setFillColor(sf::Color::Red);
	m_repairNotification.setCharacterSize(40u);
	m_repairNotification.setPosition(ScreenSize::s_width / 2 - 250, 40);
	m_repairNotification.setString("TURRET REPAIR REQUIRED \n     Smash: R");

	m_personalStats.setFont(m_font);
	m_personalStats.setFillColor(sf::Color::White);
	m_personalStats.setCharacterSize(80u);
	m_personalStats.setPosition(ScreenSize::s_width/2 - 300, (ScreenSize::s_height/2) -100);

	m_gameResult.setFont(m_font);
	m_gameResult.setFillColor(sf::Color::White);
	m_gameResult.setCharacterSize(90u);
	m_gameResult.setPosition( 430 , 50);

	m_scoreBoard.setFont(m_font);
	m_scoreBoard.setFillColor(sf::Color::White); 
	m_scoreBoard.setCharacterSize(50u); 
	m_scoreBoard.setPosition(ScreenSize::s_width / 2 - 200, (ScreenSize::s_height / 2) - 100);

}

void Game::chechForTargetRespawn()
{
	bool aTargetIsActive = false; 
	int targetsAvailable = m_targets.size() - m_tank.getScore(); 

	// checks if any targets are on the screen
	for (Target& target : m_targets)
	{
		if (target.isAlive())
		{
			aTargetIsActive = true;
			break;
		}
	}

	// activate when there isnt a single target active on the screen
	if (!aTargetIsActive)
	{
		int randSpawn = rand() % m_targets.size();
		if (!m_targets[randSpawn].beenShot())
		{
			m_targets[randSpawn].respawn();
		}
	}
	
	// all target have been destroyed
	if (targetsAvailable == 0)
	{
		m_currentGameState = UpdateYAML;
	}
}

void Game::scoreUpdate()
{
	m_scoreText.setString("Score: " + std::to_string(m_tank.getScore()));
}

void Game::manageTargetTimers()
{
	for (Target &target : m_targets)
	{
		target.updateTimer();
	}
}

void Game::gameSummary()
{
	
	int targetsShot = m_tank.getScore(); 
	int hitAccuracy = m_tank.getAccuracy() * 100; // truncated int for percentage
	int highestAccuracy = m_level.m_currentUser.m_highAccuracy * 100; 

	if (targetsShot == m_targets.size())
	{
		m_gameResult.setString("            WINNER! \n You shot all the targets");
	}
	else
	{
		m_gameResult.setString("            LOSER! \n There was still " + std::to_string((m_targets.size()) - targetsShot) + " left");
	}

	m_personalStats.setString("High Score: " + std::to_string(m_level.m_currentUser.m_highScore)
		+"\nGame Score: " + std::to_string(m_tank.getScore()) 
		+ "\nHit Accuracy: " + std::to_string(hitAccuracy) + "%" 
		+ "\n Your Best Accuracy: " + std::to_string(highestAccuracy) + "%" 
		+"\n \n  Press S to view scoreboard");
	

}

void Game::updateYAML()
{
	bool isBetter = false; 

	m_baseNode["user"]["userName"] = m_level.m_currentUser.m_userName;

	// chechs if it should write to the file or not based on whther the scores are higher
	if (m_tank.getScore() > m_level.m_currentUser.m_highScore)
	{
		m_level.m_currentUser.m_highScore = m_tank.getScore();
		m_baseNode["user"]["highScore"] = m_level.m_currentUser.m_highScore;
	
	}
	else if (m_tank.getAccuracy() > m_level.m_currentUser.m_highAccuracy)
	{
		m_level.m_currentUser.m_highAccuracy = m_tank.getAccuracy();
		m_baseNode["user"]["bestAccuracy"] = m_level.m_currentUser.m_highAccuracy;
		
	}

	addPlayerToLeaderBoard();
	sortScoreboard();
	 



	
	// if true write new vairables to the file 
	
	std::string fileName = "./resources/levels/test.yaml";
	std::ofstream fout(fileName);
	YAMLWriter::writeYamlOrderedMaps(fout, m_baseNode);
	fout.close(); // close the file 
	
	m_currentGameState = GameStats; 
	
}

void Game::enterUserInfo(sf::Event& event)
{
	sf::String currentKey;
	
	if (event.type == sf::Event::TextEntered)
	{
		currentKey = event.text.unicode;
	}

	// size safe guards so if the user back spaces too many times it doenst go out of bounds of the sting array
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))&&(m_userName.size()!=0))
	{
		m_userName.pop_back(); 
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
	    m_level.m_currentUser.m_userName = m_userName;
		m_currentGameState = EnemyGamePlay; 
	}

	m_userName =  m_userName + currentKey; 
	m_userInput.setString("Please enter username: " + m_userName);
}

void Game::switchToScoreboardCheck()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_currentGameState = Scoreboard;
	}
	
}

void Game::updateScoreBoard()
{
	m_scoreBoard.setString("1. Username: " + m_level.m_scoreBoard[0].m_userName + "\n High Score: " + std::to_string(m_level.m_scoreBoard[0].m_highScore) + "\n Best Accuracy: " + std::to_string(static_cast<int>(m_level.m_scoreBoard[0].m_highAccuracy * 100)) + "%" +
		"\n2. Username: " + m_level.m_scoreBoard[1].m_userName + "\n High Score: " + std::to_string(m_level.m_scoreBoard[1].m_highScore) + "\n Best Accuracy: " + std::to_string(static_cast<int>(m_level.m_scoreBoard[1].m_highAccuracy*100)) + "%" +
		"\n3. Username: " + m_level.m_scoreBoard[2].m_userName + "\n High Score: " + std::to_string(m_level.m_scoreBoard[2].m_highScore) + "\n Best Accuracy: " + std::to_string(static_cast<int>(m_level.m_scoreBoard[2].m_highAccuracy * 100)) + "%");
}

void Game::sortScoreboard()
{

	UserData tempContainer; 
	for (int  i = 2; i >0; i--)
	{
		
		// is 3 greater than 2
		// is 2 greater than 1
		if ((m_level.m_scoreBoard[i].m_highScore > m_level.m_scoreBoard[i - 1].m_highScore)&&(i!=0))
		{
			tempContainer = m_level.m_scoreBoard[i - 1]; 
			m_level.m_scoreBoard[i - 1] = m_level.m_scoreBoard[i]; 
			m_level.m_scoreBoard[i] = tempContainer; 
		}
		
		
	}

	for (int i = 0; i < 3; i++)
	{
		m_baseNode[m_level.m_scoreBoard[i].m_place]["bestAccuracy"] = m_level.m_scoreBoard[i].m_highAccuracy;
		m_baseNode[m_level.m_scoreBoard[i].m_place]["highScore"] = m_level.m_scoreBoard[i].m_highScore;
		m_baseNode[m_level.m_scoreBoard[i].m_place]["userName"] = m_level.m_scoreBoard[i].m_userName;
	}
}

void Game::addPlayerToLeaderBoard()
{
	// checks if the player got a better score than the last player
	if (m_tank.getScore() > m_level.m_scoreBoard[2].m_highScore)
	{
		m_level.m_scoreBoard[2].m_highAccuracy = m_tank.getAccuracy();
		m_level.m_scoreBoard[2].m_highScore = m_tank.getScore();
		m_level.m_scoreBoard[2].m_userName = m_level.m_currentUser.m_userName;

		/*m_baseNode[m_level.m_scoreBoard[2].m_place]["bestAccuracy"] = m_level.m_scoreBoard[2].m_highAccuracy;
		m_baseNode[m_level.m_scoreBoard[2].m_place]["highScore"] = m_level.m_scoreBoard[2].m_highScore;
		m_baseNode[m_level.m_scoreBoard[2].m_place]["userName"] = m_level.m_scoreBoard[2].m_userName;*/

	}
}

void Game::camerSetUp()
{
	sf::FloatRect defaultMenuView{ 0,0, ScreenSize::s_width, ScreenSize::s_height };
	m_camera.reset(sf::FloatRect(defaultMenuView));
	m_window.setView(m_camera);
}

void Game::updateCamera()
{


	switch (m_currentGameState)
	{
		case EnemyGamePlay:
			
			float mapLength = 3000;

			bool tankAtTopOfScreen = m_tank.getPosition().y < (ScreenSize::s_height / 2);
			bool tankAtBottomOfScreen = m_tank.getPosition().y > (mapLength - ScreenSize::s_height / 2);

			if (!tankAtTopOfScreen && !tankAtBottomOfScreen )
			{
				m_camera.setCenter({ ScreenSize::s_width / 2,m_tank.getPosition().y });
			}
			// check if it should set the limit to the top of the screen
			else if(m_tank.getPosition().y < ScreenSize::s_height)
			{
				m_camera.reset(sf::FloatRect(sf::FloatRect { 0,0, ScreenSize::s_width, ScreenSize::s_height }));
			}
			// or the bottom of the screen
			else
			{
				m_camera.reset(sf::FloatRect(sf::FloatRect{ 0,mapLength - ScreenSize::s_height, ScreenSize::s_width, ScreenSize::s_height }));
			}
			
			std::cout << "Top: " << m_camera.getViewport().top << std::endl; 
			m_hud.updatePositions({ 0, m_camera.getCenter().y}); // updates hud to top of screen as it moves
			m_window.setView(m_camera);

			break;


		}
}

void Game::setUpReturnPoint()
{
	sf::Color red = sf::Color::Red;
	red.a = 50;
	returnPoint.setRadius(100);
	returnPoint.setFillColor(red);
	returnPoint.setPosition({ ScreenSize::s_width / 2 - 50, 100 });
}

////////////////////////////////////////////////////////////
void Game::update(double dt)
{
	
	switch (m_currentGameState)
	{
		case Menu:
			break; 

		case TargetPractice:
			//m_tank.update(dt, m_funcApplyDamge,m_funcDecrementHudFuel m_aiTank.getTankBase());
			timerUpdate();
			scoreUpdate();
			manageTargetTimers();
			chechForTargetRespawn();
			m_tank.update(dt, m_funcApplyDamge, m_funcDecrementHudFuel, m_aiTank.getTankBase());
			break;

		case UpdateYAML:
			updateYAML();
			break; 

		case GameStats: 
			gameSummary();
			switchToScoreboardCheck(); 
			break; 

		case Scoreboard:
			updateScoreBoard();
			break; 

		case EnemyGamePlay:
			m_tank.update(dt, m_funcApplyDamge,m_funcDecrementHudFuel, m_aiTank.getTankBase());
			m_aiTank.update(m_funcDecrementHudHealth,m_tank, dt);
		//	m_aiTank2.update(m_funcDecrementHudHealth, m_tank, dt);

			if (!m_tank.isAlive())
			{
				m_currentGameState = EnemyGamePlayLose;
				m_clock.restart();
			}

			if (m_repairKit.collisionCheck(m_tank.getBase()))
			{
				m_tank.repairTank();
				m_hud.refilFuel(); 
				m_hud.refilHealth(); 
			}

			for (int i = 0; i < 3; i++)
			{
				if (m_fuel[i].collisionCheck(m_tank.getBase()))
				{
					m_tank.fillFuel();
					m_hud.refilFuel();
				}
			}

			if (m_backpack.collisionCheck(m_tank.getBase()))
			{
				m_hud.acquiredObjective();
			}
			if ((m_hud.playerHasObjective()) && (m_tank.getBase().getGlobalBounds().intersects(returnPoint.getGlobalBounds())))
			{
				m_currentGameState = EnemyGamePlayLose;
				m_clock.restart();
			}
			break;

		case EnemyGamePlayLose:
			if (m_clock.getElapsedTime().asSeconds() > 5)
			{
				m_currentGameState = Menu;
				m_aiTank.reset();
				//m_aiTank2.reset();
				int randSpawn = rand() % m_level.m_tank.m_tankPositions.size();
				m_tank.setPosition(m_level.m_tank.m_tankPositions[randSpawn]);
			}
			break;

		case EnemyGamePlayWin:
			if (m_clock.getElapsedTime().asSeconds() > 5)
			{
				m_currentGameState = Menu;
				m_aiTank.reset();
			//	m_aiTank2.reset();
				int randSpawn = rand() % m_level.m_tank.m_tankPositions.size();
				m_tank.setPosition(m_level.m_tank.m_tankPositions[randSpawn]);
			}
			break; 


	}

	m_hud.update(m_currentGameState);
	updateCamera();
}

////////////////////////////////////////////////////////////
void Game::render()
{
	m_window.clear(sf::Color(0, 0, 0, 0));
#ifdef TEST_FPS
	m_window.draw(x_updateFPS);
	m_window.draw(x_drawFPS);
#endif

	m_window.draw(m_bgSpritee);

	//grid.draw(m_window);
	if (m_currentGameState == Menu)
	{
		m_window.draw(m_userInput);
	}

	if (m_currentGameState == TargetPractice)
	{
		// Render your sprites here....
		
		m_tank.render(m_window);
		for (Target target : m_targets)
		{
			target.draw(m_window);
		}
		// draws all sprites in the vector
		for (sf::Sprite sprite : m_wallSprites)
		{
			m_window.draw(sprite);
		}
		
		m_window.draw(m_timerText);
		m_window.draw(m_scoreText);
		if (m_tank.needsRepair())
		{
			m_window.draw(m_repairNotification);
		}
	}

	if (m_currentGameState == EnemyGamePlay)
	{
		m_window.draw(returnPoint);
		m_repairKit.draw(m_window);
		for (int i = 0; i < 3; i++)
		{
			m_fuel[i].draw(m_window);
		}
		
		m_backpack.draw(m_window);
		m_tank.render(m_window);

		m_aiTank.render(m_window);
		//m_aiTank2.render(m_window); 
		for (sf::Sprite sprite : m_wallSprites)
		{
			m_window.draw(sprite);
		}
		
		m_hud.render(m_window);

	}

	if (m_currentGameState == EnemyGamePlayLose)
	{
		m_hud.render(m_window);
	}

	if (m_currentGameState == EnemyGamePlayWin)
	{
		m_hud.render(m_window);
	}
	if (m_currentGameState == GameStats)
	{
		m_window.draw(m_gameResult);
		m_window.draw(m_personalStats);
	}

	if (m_currentGameState == Scoreboard)
	{
		m_window.draw(m_scoreBoard); 
	}
	
	//m_window.draw(m_gameStateText);
	
	
	
	m_window.display();
}





