#pragma once

#include <sstream>
#include <fstream>
#include <iostream>
#include "yaml-cpp\yaml.h"

struct TankData
{
	//posiotion
	float m_x;
	float m_y;
	//scale
	float m_j;
	float m_k;
	float m_roatation; 
};
struct LevelData
{
   //TankData m_tank;
   
   std::vector<TankData>m_tank; 
};

class LevelLoader
{
public:
   static void load(int t_levelNr, LevelData& t_level);
};
