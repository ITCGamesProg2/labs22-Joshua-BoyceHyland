#include "HUD.h"

////////////////////////////////////////////////////////////
HUD::HUD(sf::Font& hudFont)
    : m_textFont(hudFont)
{
    m_gameStateText.setFont(hudFont);
    m_gameStateText.setCharacterSize(30);
    m_gameStateText.setFillColor(sf::Color::Blue);
    m_gameStateText.setPosition(sf::Vector2f(600, 5));
    m_gameStateText.setString("Game Running");

    //Setting up our hud properties 
    m_hudOutline.setSize(sf::Vector2f(1440.0f, 40.0f));
    m_hudOutline.setFillColor(sf::Color(0, 0, 0, 38));
    m_hudOutline.setOutlineThickness(-.5f);
    m_hudOutline.setOutlineColor(sf::Color(0, 0, 0, 100));
    m_hudOutline.setPosition(0, 0);

    m_currentGameHud.setFont(hudFont);
    m_currentGameHud.setCharacterSize(60);
    m_currentGameHud.setPosition({ 650, 400 });


    
    m_currentGameHudOutline.setOutlineThickness(-.5f);
    
    m_currentGameHudOutline.setPosition({m_currentGameHud.getPosition().x - 38,m_currentGameHud.getPosition() .y - 70});
}

////////////////////////////////////////////////////////////
void HUD::update(GameState const& gameState)
{

    m_gameStateText.setString("GameState: " + m_gameStateStrings[gameState]);

    
    
   switch (gameState)
    {
   case GameState::Menu:
       m_currentGameHud.setString("");
       m_currentGameHudOutline.setSize(sf::Vector2f(0, 0.0f));
        break;
    case GameState::TargetPractice:
        break;
    case GameState::GameStats:
        break;
    case GameState::Scoreboard:
        break;
    case GameState::EnemyGamePlay:
        m_currentGameHud.setString("");
        m_currentGameHudOutline.setSize(sf::Vector2f(0, 0.0f));
        break;
    case GameState::EnemyGamePlayLose:
        
        m_currentGameHud.setString("You Lose");
        m_currentGameHud.setFillColor(sf::Color::Red);
        m_currentGameHudOutline.setSize(sf::Vector2f(220, 220.0f));
        m_currentGameHudOutline.setFillColor(sf::Color(30, 0, 0, 38));
        m_currentGameHudOutline.setOutlineColor(sf::Color(101, 0, 0, 100));
        break;
    case GameState::EnemyGamePlayWin:
        m_currentGameHud.setString("You Won");
        m_currentGameHud.setFillColor(sf::Color::Green);
        m_currentGameHudOutline.setSize(sf::Vector2f(220, 220.0f));
        m_currentGameHudOutline.setFillColor(sf::Color(0, 30, 0, 38));
        m_currentGameHudOutline.setOutlineColor(sf::Color(0, 101, 0, 100));
        
        break;

    default:
        break;
    }
}

void HUD::render(sf::RenderWindow& window)
{
    window.draw(m_hudOutline);
    window.draw(m_gameStateText); 
    window.draw(m_currentGameHudOutline);
    window.draw(m_currentGameHud);

}

void HUD::updatePositions(sf::Vector2f t_pos)
{
    m_hudOutline.setPosition( t_pos );
    m_gameStateText.setPosition({ m_gameStateText.getPosition().x,t_pos.y});
}

