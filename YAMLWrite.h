#pragma once

#include "yaml-cpp/yaml.h"
#include <string>
#include <algorithm> 
#include <iostream>
#include <fstream>

class YAMLWriter
{
public: 
	static void writeOrderedMaps(std::ostream& out, const YAML::Node& node);

private:
	static void writeNode(const YAML::Node& node, YAML::Emitter& emmitter); 
};