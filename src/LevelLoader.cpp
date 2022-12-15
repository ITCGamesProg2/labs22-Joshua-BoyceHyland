#include "LevelLoader.h"

YAML::Node LevelLoader::yamlFile; 
/// The various operator >> overloads below are non-member functions used to extract
///  the game data from the YAML data structure.

/// <summary>
/// @brief Extracts the obstacle type, position and rotation values.
/// 
/// </summary>
/// <param name="t_obstacleNode">A YAML node</param>
/// <param name="t_obstacle">A simple struct to store the obstacle data</param>
////////////////////////////////////////////////////////////
void operator >> (const YAML::Node& t_obstacleNode, ObstacleData& t_obstacle)
{
	t_obstacle.m_type = t_obstacleNode["type"].as<std::string>();
	t_obstacle.m_position.x = t_obstacleNode["position"]["x"].as<float>();
	t_obstacle.m_position.y = t_obstacleNode["position"]["y"].as<float>();
	t_obstacle.m_rotation = t_obstacleNode["rotation"].as<double>();
}

/// <summary>
/// @brief Extracts the filename for the game background texture.
/// 
/// </summary>
/// <param name="t_backgroundNode">A YAML node</param>
/// <param name="t_background">A simple struct to store background related data</param>
////////////////////////////////////////////////////////////
void operator >> (const YAML::Node& t_backgroundNode, BackgroundData& t_background)
{
	t_background.m_fileName = t_backgroundNode["file"].as<std::string>();
}

/// <summary>
/// @brief Extracts the initial screen position for the player tank.
/// 
/// </summary>
/// <param name="t_tankNode">A YAML node</param>
/// <param name="t_tank">A simple struct to store data related to the player tank</param>
////////////////////////////////////////////////////////////
void operator >> (const YAML::Node& t_tankNode, sf::Vector2f & t_tankPos)
{
	
	t_tankPos.x = t_tankNode["position"]["x"].as<float>();
	t_tankPos.y = t_tankNode["position"]["y"].as<float>();
	
}

void operator>>(const YAML::Node& t_targetNode, TargetData& t_targetPos)
{
	t_targetPos.m_offset = t_targetNode["position"]["offSet"].as<float>(); 
	t_targetPos.m_position.x = t_targetNode["position"]["x"].as<float>(); 
	t_targetPos.m_position.y = t_targetNode["position"]["y"].as<float>();
}

void operator>>(const YAML::Node& t_baseNode, UserData& t_user)
{

	t_user.m_highAccuracy = t_baseNode["bestAccuracy"].as<float>();
	t_user.m_highScore = t_baseNode["highScore"].as<int>();
	t_user.m_userName = t_baseNode["userName"].as<std::string>();
	
}

/// <summary>
/// @brief Top level function that extracts various game data from the YAML data stucture.
/// 
/// Invokes other functions to extract the background, tank and obstacle data.
//   Because there are multiple obstacles, obstacle data are stored in a vector.
/// </summary>
/// <param name="t_levelNode">A YAML node</param>
/// <param name="t_level">A simple struct to store level data for the game</param>
////////////////////////////////////////////////////////////
void operator >> (const YAML::Node& t_levelNode, LevelData& t_level)
{
	t_levelNode["background"] >> t_level.m_background;
	t_levelNode["user"] >> t_level.m_currentUser;

	t_levelNode["firstPlace"] >> t_level.m_scoreBoard[0]; 
	t_level.m_scoreBoard[0].m_place = "firstPlace";

	t_levelNode["secondPlace"] >> t_level.m_scoreBoard[1];
	t_level.m_scoreBoard[1].m_place = "secondPlace";

	t_levelNode["thirdPlace"] >> t_level.m_scoreBoard[2];
	t_level.m_scoreBoard[0].m_place = "thirdPlace";

	const YAML::Node& tanksNode = t_levelNode["tank"].as<YAML::Node>();
	for (unsigned i = 0; i < tanksNode.size(); ++i)
	{
		sf::Vector2f position;

		tanksNode[i] >> position;
		t_level.m_tank.m_tankPositions.push_back(position);	
	}

	const YAML::Node& obstaclesNode = t_levelNode["obstacles"].as<YAML::Node>();
	for (unsigned i = 0; i < obstaclesNode.size(); ++i)
	{
		ObstacleData obstacle;
		obstaclesNode[i] >> obstacle;
		t_level.m_obstacles.push_back(obstacle);
	}

	const YAML::Node& targetNodeTarget = t_levelNode["target"].as<YAML::Node>();
	for (unsigned i = 0; i < targetNodeTarget.size(); ++i)
	{
		TargetData position;
		targetNodeTarget[i] >> position;
		t_level.m_targetData.push_back(position);
	}
	
	

	

	

}

////////////////////////////////////////////////////////////
void LevelLoader::load(int t_levelNr, LevelData& t_level)
{
	/*std::string filename = "./resources/levels/level" + std::to_string(t_levelNr) + ".yaml";*/
	std::string filename = "./resources/levels/test.yaml"; 
	try
	{
		YAML::Node baseNode = YAML::LoadFile(filename);
		if (baseNode.IsNull())
		{
			std::string message("File: " + filename + " not found");
			throw std::exception(message.c_str());
		}
		baseNode >> t_level;
	}
	catch (YAML::ParserException& e)
	{
		std::string message(e.what());
		message = "YAML Parser Error: " + message;
		throw std::exception(message.c_str());
	}
	catch (std::exception& e)
	{
		std::string message(e.what());
		message = "Unexpected Error: " + message;
		throw std::exception(message.c_str());
	}
}

YAML::Node LevelLoader::getNode()
{
	yamlFile = YAML::LoadFile("./resources/levels/test.yaml");
	return yamlFile;
}


 


