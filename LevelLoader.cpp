#include "LevelLoader.h"



void operator >> (const YAML::Node& t_tankNode, TankData& t_tank)
{
	t_tank.m_x = t_tankNode["position"]["x"].as<float>();
	t_tank.m_y = t_tankNode["position"]["y"].as<float>();
	t_tank.m_j = t_tankNode["scale"]["x"].as<float>();
	t_tank.m_k = t_tankNode["scale"]["y"].as<float>();
	t_tank.m_roatation = t_tankNode["rotation"].as<float>();
	
}

//void operator>>(const YAML::Node& t_enemyNode, EnemyData t_enemy)
//{
//	t_enemy.m_type = t_enemyNode["type"].as<std::string>();
//	t_enemy.m_health = t_enemyNode["health"].as<int>();
//	t_enemy.m_y = t_enemyNode["position"]["x"].as<float>();
//	t_enemy.m_y = t_enemyNode["position"]["y"].as<float>();
//}

void operator >> (const YAML::Node& t_levelNode, LevelData& t_level)
{
	// proccessinf tank data 
	//t_levelNode["tank"] >> t_level.m_tank;
	
	const YAML::Node& enemiesNode = t_levelNode["tank"].as<YAML::Node>();
	for (unsigned i = 0; i < enemiesNode.size(); ++i)
	{
		TankData tank;
		enemiesNode[i] >> tank;
		t_level.m_tank.push_back(tank);
	}
}

void LevelLoader::load(int t_levelNr, LevelData& t_level)
{
   std::string filename = "./levelData/level" + std::to_string(t_levelNr) + ".yaml";
   int x = 0;
   try
   {
	   YAML::Node baseNode = YAML::LoadFile(filename);
	   if (baseNode.IsNull())
	   {
		   std::string message("File: " + filename + " not found");
		   throw std::exception(message.c_str());
	   }
	   // Call overloaded operator >>, passing baseNode as arg1, t_level as arg2
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
