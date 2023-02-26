#pragma once
#include <SFML/Graphics.hpp>
#include "GameStates.h"

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
};