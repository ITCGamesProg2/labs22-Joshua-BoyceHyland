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




    initialiseFuelIcon();
    initialiseHealthIcon();
    initialiseHudBackground();
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
    window.draw(m_hudBackground);
    window.draw(m_hudOutline);
    window.draw(m_gameStateText); 
    window.draw(m_currentGameHudOutline);
    window.draw(m_currentGameHud);
    window.draw(fuelShape);
    window.draw(fuelBar);
    window.draw(fuelIndicator);
    window.draw(m_healthbar); 

}

void HUD::updatePositions(sf::Vector2f t_currentScreenCenter)
{


    std::cout << "f: " << m_healthbar.getPosition().y << std::endl; 

    m_hudOutline.setPosition({0, t_currentScreenCenter.y - screenHeight / 2 });
    m_gameStateText.setPosition({ m_gameStateText.getPosition().x,t_currentScreenCenter.y - screenHeight/2});
    fuelBar.setPosition({ fuelBar.getPosition().x, t_currentScreenCenter.y + 300 + 25 });
    fuelIndicator.setPosition({ fuelIndicator.getPosition().x, t_currentScreenCenter.y + 389 + 25});
    fuelShape.setPosition({ fuelShape.getPosition().x, t_currentScreenCenter.y + 300  + 25});


    m_healthbar.setPosition({ m_healthbar.getPosition().x, t_currentScreenCenter.y + 390 });
}

void HUD::initialiseFuelIcon()
{

    if (!FuelBarTexture.loadFromFile("resources/images/fuelBar.png"))
    {
        std::cout << "could not load fuelBar" << std::endl;
    }

    if (!fuelIndicatorTexture.loadFromFile("resources/images/indicator.png"))
    {
        std::cout << "could not load fuelBar" << std::endl;
    }

    fuelBar.setTexture(FuelBarTexture);
    fuelBar.setPosition({ 0 ,750 + 25 });
    fuelBar.setScale({ 0.75, 0.75 });
   
    fuelIndicator.setTexture(fuelIndicatorTexture);
    fuelIndicator.setPosition({ 96,750 + 89 + 25 });
    fuelIndicator.setRotation(15);
    fuelIndicator.setOrigin({ 129, 119 });
    fuelIndicator.setScale({ 0.75, 0.75 });



    fuelShape.setPosition({ 20,750 + 25});
    fuelShape.setOutlineColor(sf::Color::Red);
    fuelShape.setOutlineThickness(2);
    fuelShape.setSize({ 157,100 });
}

void HUD::initialiseHealthIcon()
{
    if (!m_healtTexture.loadFromFile("resources/images/healthbar.png"))
    {
        std::cout << "Could not load health bar" << std::endl; 
    }

    m_healthbar.setTexture(m_healtTexture);
    m_healthbar.setPosition({ 200, 840 });
    m_healthbar.setScale({ 0.15,0.15 });
  //  m_healthIndicator.setSize
}

void HUD::initialiseHudBackground()
{
    m_hudBackground.setPointCount(4); 
    m_hudBackground.setPoint(0, { 0, 750 }); 
    m_hudBackground.setPoint(1, {200,750 });
    m_hudBackground.setPoint(2, { 200, 900 });

    m_hudBackground.setPoint(3, { 0, 1000 });


    m_hudBackground.setFillColor(sf::Color(75, 83, 32));
}

void HUD::decrementFuelVisual(int t_decrementaion)
{
    fuelIndicator.setRotation(fuelIndicator.getRotation() - 0.05);
}

