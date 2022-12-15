#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include "yaml-cpp\yaml.h"

/// <summary>
/// @brief A struct to represent Obstacle data in the level.
/// 
/// </summary>
struct ObstacleData
{
	std::string m_type;
	sf::Vector2f m_position;
	double m_rotation;
};

/// <summary>
/// @brief A struct to store Background texture information.
/// 
/// </summary>
struct BackgroundData
{
	std::string m_fileName;
};

/// <summary>
/// @brief A struct to represent tank data in the level.
/// 
/// </summary>
struct TankData
{
	std::vector<sf::Vector2f> m_tankPositions;
};

struct TargetData
{
	float m_offset; 
	sf::Vector2f m_position; 
};

struct UserData
{
	int m_highScore; 
	float m_highAccuracy; 
	std::string m_userName;  
};

struct ScoreBoardData
{
	int m_highScore;
	float m_highAccuracy;
	std::string m_userName;
	char place = 'a';
	void operator = (UserData user)
	{
		m_highScore = user.m_highScore;
		m_highAccuracy = user.m_highAccuracy;
		m_userName = user.m_userName;
	};

};
/// <summary>
/// @brief A struct representing all the Level Data.
/// 
/// Obstacles may be repeated and are therefore stored in a container.
/// </summary>
struct LevelData
{
	BackgroundData m_background;
	TankData m_tank;
	UserData m_currentUser; 

	std::vector<ScoreBoardData> m_scoreboard; 
	std::vector<ObstacleData> m_obstacles;
	std::vector<TargetData> m_targetData; 
};

/// <summary>
/// @brief A class to manage level loading.
/// 
/// This class will manage level loading using YAML.
/// </summary>
class LevelLoader
{
public:

	/// <summary>
	/// @brief No-op default constructor.
	/// 
	/// </summary>
	LevelLoader() = default;

	/// <summary>
	/// @brief Loads and parses the yaml level file.
	/// The level file is identified by a number and is assumed to have
	/// the following format: "level" followed by number followed by .yaml extension
	/// E.g. "level1.yaml"
	/// The level information is stored in the specified LevelData object.
	/// If the filename is not found or the file data is invalid, an exception
	/// is thrown.
	/// </summary>
	/// <param name="t_levelNr">The level number</param>
	/// <param name="t_level">A reference to the LevelData object</param>
	static void load(int t_levelNr, LevelData& t_level);

	static YAML::Node getNode();
	static YAML::Node yamlFile;
};