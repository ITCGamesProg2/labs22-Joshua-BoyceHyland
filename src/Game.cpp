#include "Game.h"
#include <iostream>

// Our target FPS
static double const FPS{ 60.0f };
////////////////////////////////////////////////////////////
Game::Game()
	: m_window(sf::VideoMode(ScreenSize::s_width, ScreenSize::s_height, 32), "SFML Playground", sf::Style::Default), m_tank(m_texture, sf::Vector2f(0,0))
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
	if (!m_texture.loadFromFile("./resources/images/SpriteSheet.png"))
	{
		std::string s("Error loading texture");  
		throw std::exception(s.c_str()); 
	}
	m_tank.setPosition(m_level.m_tank.m_position); 
	
	/*if (!m_tankTexture.loadFromFile("images/E-100.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}
	m_tankPosition = m_level.m_tank.m_position;
	m_tankSprite.setPosition(m_tankPosition);
	m_tankSprite.setTexture(m_tankTexture);
	m_tankSprite.setOrigin(m_tankTexture.getSize().x / 2.0, m_tankTexture.getSize().y / 2.0);*/

	if (!m_spriteSheetTexture.loadFromFile("./resources/images/SpriteSheet.png"))
	{
		std::string errorMsg("Error loading texture"); 
		throw std::exception(errorMsg.c_str()); 
	}

	generateWalls(); 
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_tank.increaseSpeed();
 
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_tank.decreaseSpeed();
		
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_tank.increaseRotation();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_tank.decreaseRotation();
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

////////////////////////////////////////////////////////////
void Game::update(double dt)
{
	m_tank.update(dt);
}

////////////////////////////////////////////////////////////
void Game::render()
{
	m_window.clear(sf::Color(0, 0, 0, 0));
#ifdef TEST_FPS
	m_window.draw(x_updateFPS);
	m_window.draw(x_drawFPS);
#endif

	// Render your sprites here....
	m_window.draw(m_bgSpritee);
	m_tank.render(m_window); 

	// draws all sprites in the vector
	for (sf::Sprite sprite : m_wallSprites)
	{
		m_window.draw(sprite);
	}
	
	
	

	m_window.display();
}





