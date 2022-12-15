#include "Game.h"
#include <iostream>

// Our target FPS
static double const FPS{ 60.0f };
////////////////////////////////////////////////////////////
Game::Game()
	: m_window(sf::VideoMode(ScreenSize::s_width, ScreenSize::s_height, 32), "SFML Playground", sf::Style::Default), m_tank(m_tankTexture,m_wallSprites,m_targets)
{
	init();
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
	catch (std::exception & e)
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
	m_bgSpritee.setTexture(m_bgTexture);

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
	m_tank.setPosition(m_level.m_tank.m_tankPositions[randSpawn]);
	
	

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
	setUpText();
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
		m_currentGameState = PersonalStats;
	}
}
	

void Game::setUpText()
{


	m_userInput.setFont(m_font);
	m_userInput.setFillColor(sf::Color::White);
	m_userInput.setCharacterSize(80u);
	m_userInput.setPosition(ScreenSize::s_width / 2 - 200, (ScreenSize::s_height / 2) - 100);
	m_userInput.setString("Name: ");

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



	


}

void Game::chechForTargetRespawn()
{
	bool aTargetIsActive = false; 
	int targetsAvailable = m_targets.size() - m_tank.getScore(); 

	for (Target& target : m_targets)
	{
		//target.respawn();
		if (target.isAlive())
		{
			aTargetIsActive = true;
			break;
		}
	}

	if (!aTargetIsActive)
	{
		
		int randSpawn = rand() % m_targets.size();
		if (!m_targets[randSpawn].beenShot())
		{
			m_targets[randSpawn].respawn();
		}
	}
	
	if (targetsAvailable == 0)
	{
		m_currentGameState = PersonalStats;
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
	int highestAccuracy = m_level.m_scores.m_highAccuracy * 100; 

	if (targetsShot == m_targets.size())
	{
		m_gameResult.setString("            WINNER! \n You shot all the targets");
	}
	else
	{
		m_gameResult.setString("            LOSER! \n There was still " + std::to_string((m_targets.size()) - targetsShot) + " left");
	}

	m_personalStats.setString("High Score: " + std::to_string(m_level.m_scores.m_highScore) 
		+"\nGame Score: " + std::to_string(m_tank.getScore()) 
		+ "\nHit Accuracy: " + std::to_string(hitAccuracy) + "%" 
		+ "\n Your Best Accuracy: " + std::to_string(highestAccuracy) + "%" 
		+"\n \n  Press S to view scoreboard");
	

}

void Game::updateYAML()
{
	bool write = false; 
 

	// chechs if it should write to the file or not based on whther the scores are higher
	if (m_tank.getScore() > m_level.m_scores.m_highScore)
	{
		m_level.m_scores.m_highScore = m_tank.getScore(); 
		m_baseNode["scores"]["highScore"] = m_level.m_scores.m_highScore;
		write = true;
	}
	else if (m_tank.getAccuracy() > m_level.m_scores.m_highAccuracy)
	{
		m_level.m_scores.m_highAccuracy = m_tank.getAccuracy(); 
		m_baseNode["scores"]["bestAccuracy"] = m_level.m_scores.m_highAccuracy;
		write = true;
	}
	
	// if true write new vairables to the file 
	if (write)
	{
		std::string fileName = "./resources/levels/test.yaml";
		std::ofstream fout(fileName);
		YAMLWriter::writeYamlOrderedMaps(fout, m_baseNode);
		fout.close(); // close the file 
	}

	
}

void Game::enterUserInfo(sf::Event& event)
{
	sf::String currentKey;
	m_userName = m_level.m_scores.m_userName;
	if (event.type == sf::Event::TextEntered)
	{
		currentKey = event.text.unicode;
	}

	// size safe guards so if the user back spaces too many times it doenst go out of bounds of the sting array
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))&&(m_userName.size()!=0))
	{
		m_userName.pop_back(); 
	}

	m_userName =  m_userName + currentKey; 
	m_userInput.setString("Name: " + m_userName);
	m_level.m_scores.m_userName = m_userName; 
	std::cout << m_level.m_scores.m_userName << std::endl; 
}

void Game::switchToScoreboardCheck()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_currentGameState = Scoreboard;
	}
	
}

////////////////////////////////////////////////////////////
void Game::update(double dt)
{
	switch (m_currentGameState)
	{
		case Menu:
			break; 

		case Gameplay:
			m_tank.update(dt);
			timerUpdate();
			scoreUpdate();
			manageTargetTimers();
			chechForTargetRespawn();
			break;

		case PersonalStats: 
			updateYAML();
			gameSummary();
			switchToScoreboardCheck(); 
			break; 

		case Scoreboard:


			break; 

	}
	
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


	if (m_currentGameState == Menu)
	{
		m_window.draw(m_userInput);
	}

	if (m_currentGameState == Gameplay)
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

	if (m_currentGameState == PersonalStats)
	{
		m_window.draw(m_gameResult);
		m_window.draw(m_personalStats);
	}

	if (m_currentGameState == Scoreboard)
	{

	}
	
	

	m_window.display();
}





