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
    initialiseObjectiveIcon();
   
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
    window.draw(m_healthBarBlackout);
    window.draw(m_healthIndicator);
    window.draw(m_healthbar); 

    if (m_objectiveAquired)
    {
        window.draw(m_backPack);
        // window.draw(m_objectiveText);
    }
    
   

}

void HUD::updatePositions(sf::Vector2f t_currentScreenCenter)
{


    std::cout << "f: " << m_currentGameHud.getPosition().y << std::endl;

    m_hudOutline.setPosition({ 0, t_currentScreenCenter.y - screenHeight / 2 });
    m_gameStateText.setPosition({ m_gameStateText.getPosition().x,t_currentScreenCenter.y - screenHeight / 2 });
    fuelBar.setPosition({ fuelBar.getPosition().x, t_currentScreenCenter.y + 300 + 25 });
    fuelIndicator.setPosition({ fuelIndicator.getPosition().x, t_currentScreenCenter.y + 389 + 25 });
    fuelShape.setPosition({ fuelShape.getPosition().x, t_currentScreenCenter.y + 300 + 25 });


    m_hudBackground.setPosition({ m_hudBackground.getPosition().x, t_currentScreenCenter.y + 300 });

    m_healthbar.setPosition({ m_healthbar.getPosition().x, t_currentScreenCenter.y + 390 });
    m_healthIndicator.setPosition({ m_healthIndicator.getPosition().x ,t_currentScreenCenter.y + 403 });
    m_healthBarBlackout.setPosition(m_healthIndicator.getPosition());

    m_backPack.setPosition({ m_backPack.getPosition().x, t_currentScreenCenter.y + 270 });

    m_currentGameHudOutline.setPosition({ m_currentGameHudOutline.getPosition().x, t_currentScreenCenter.y - 100 });
    m_currentGameHud.setPosition({ m_currentGameHud.getPosition().x,  t_currentScreenCenter.y -100  });
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
    m_healthIndicator.setSize({ 102, 12 });
    m_healthIndicator.setPosition({ 220, 853 });
    m_healthIndicator.setFillColor(sf::Color::Red);

    m_healthBarBlackout.setFillColor(sf::Color::Black); 
    m_healthBarBlackout.setPosition(m_healthIndicator.getPosition()); 
    m_healthBarBlackout.setSize(m_healthIndicator.getSize());
}

void HUD::initialiseHudBackground()
{
    m_hudBackground.setPointCount(6); 
    m_hudBackground.setPoint(0, { 0, 0 }); 
    m_hudBackground.setPoint(1, {200,0 });
    m_hudBackground.setPoint(2, { 200, 80 });
    m_hudBackground.setPoint(3, { 335, 77 });
    m_hudBackground.setPoint(4, { 335, 150 });

    m_hudBackground.setPoint(5, { 0, 150 });


    m_hudBackground.setFillColor(sf::Color(75, 83, 32));
    m_hudBackground.setOutlineThickness(2); 
    m_hudBackground.setOutlineColor(sf::Color::Black);


    m_hudBackground.setPosition({ 0, 750 });
}

void HUD::initialiseObjectiveIcon()
{
    if (!m_backPackTexture.loadFromFile("resources/images/backPack.png"))
    {
        std::cout << "Could load backpack icon" << std::endl;
    }

    m_backPack.setTexture(m_backPackTexture);
    m_backPack.setScale({ 0.75, 0.75 });
    m_backPack.setPosition({ 1270, 720 });

    m_objectiveText.setFont(m_textFont); 
    m_objectiveText.setString("Objective Acquired: "); 
    m_objectiveText.setPosition({ 1200, 680 });
    m_objectiveText.setFillColor(sf::Color::Black); 
    m_objectiveText.setCharacterSize(20);

    
}

void HUD::decrementFuelVisual(float t_decrementaion)
{
    fuelIndicator.setRotation(fuelIndicator.getRotation() - t_decrementaion);
}

void HUD::decrementHealthVisual(float t_decrementaion)
{
    if (m_healthIndicator.getSize().x > 0)
    {
        m_healthIndicator.setSize({ m_healthIndicator.getSize().x - t_decrementaion, m_healthIndicator.getSize().y });
    }
    
}

void HUD::refilFuel()
{
    fuelIndicator.setRotation(15);
}

void HUD::refilHealth()
{
    m_healthIndicator.setSize({ 102, 12 });
}

void HUD::acquiredObjective()
{
    m_objectiveAquired = true; 
}

bool HUD::playerHasObjective()
{
    return m_objectiveAquired;
}

