#pragma once
#include <SFML/Graphics.hpp>
#include "GameStates.h"
#include <iostream>

/// <summary>
/// @brief A basic HUD implementation.
/// 
/// Shows current game state only.
/// </summary>

class HUD
{
public:
    /// <summary>
    /// @brief Default constructor that stores a font for the HUD and initialises the general HUD appearance.
    /// </summary>
    HUD(sf::Font& hudFont);

    /// <summary>
    /// @brief Checks the current game state and sets the appropriate status text on the HUD.
    /// </summary>
    /// <param name="gameState">The current game state</param>
    void update(GameState const& gameState);

    /// <summary>
    /// @brief Draws the HUD outline and text.
    ///
    /// </summary>
    /// <param name="window">The SFML Render window</param>
    void render(sf::RenderWindow& window);

    /// <summary>
    /// updates position of relevant hud text
    /// </summary>
    void updatePositions(sf::Vector2f t_pos);

    /// <summary>
    /// sets up the fuel icon and box
    /// </summary>
    void initialiseFuelIcon();

    /// <summary>
    /// sets up the health icon
    /// </summary>
    void initialiseHealthIcon();

    /// <summary>
    /// setups the points for the huf
    /// </summary>
    void initialiseHudBackground();

    /// <summary>
    /// moves the indicator for fuel gage
    /// </summary>
    /// <param name="t_decrementaion">abiut by which it is rotate</param>
    void decrementFuelVisual(float t_decrementaion);

    /// <summary>
    /// moves the health bar down on health bar
    /// </summary>
    /// <param name="t_decrementaion">the amount which it moves down by </param>
    void decrementHealthVisual(float t_decrementaion); 
private:
    // The font for this HUD.
    sf::Font m_textFont;

    // A container for the current HUD text.
    sf::Text m_gameStateText;

    sf::Text m_currentGameHud;
    // A simple background shape for the HUD.
    sf::RectangleShape m_hudOutline;

    sf::RectangleShape m_currentGameHudOutline;

    std::string m_gameStateStrings[8] = {
    "Menu",
    "TargetPractice",
    "UpdateYAML",
    "GameStats",
    "Scoreboard" ,
    /*player vs ai states*/
    "EnemyGamePlay",
    "EnemyGamePlayLose",
    "EnemyGamePlayWin"
    };

    float screenWidth = 1440;
    float screenHeight = 900;


    // fuel visuals
    sf::Sprite fuelBar;
    sf::Sprite fuelIndicator;
    sf::Texture FuelBarTexture;
    sf::Texture fuelIndicatorTexture;
    sf::RectangleShape fuelShape;

    // healthVisuals
    sf::RectangleShape m_healthIndicator; 
    sf::RectangleShape m_healthBarBlackout;
    sf::Sprite m_healthbar; 
    sf::Texture m_healtTexture;
    
  


    // backGround of hud ui
    sf::ConvexShape m_hudBackground;

    sf::View m_minimap;
};