#include "YAMLWrite.h"

// main function that emits a yaml node to an ouptu stream
void YAMLWriter::writeOrderedMaps(std::ostream& out, const YAML::Node& node)
{
	YAML::Emitter emitter;
	writeNode(node, emitter);
	out << emitter.c_str() << std::endl;
}


// recursive helper function thatdoes all the work 
void YAMLWriter::writeNode(const YAML::Node& node, YAML::Emitter& emmitter)
{
	switch (node.Type())
	{
	case YAML::NodeType::Sequence:
	{
		emmitter << YAML::BeginSeq;
		for (size_t i = 0; i < node.size(); i++)
		{
			writeNode(node[i], emmitter);
		}
		emmitter << YAML::EndSeq;
		break;
	}
	case YAML::NodeType::Map:
	{
		emmitter << YAML::BeginMap; 
		// first collect all the keys
		std::vector<std::string> keys(node.size());
		int key_it = 0; 
		for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
		{
			keys[key_it++] = it->first.as<std::string>(); 
		}

		// then sort them 
		std::sort(keys.begin(), keys.end());

		// then emit all the entries in sorted order
		for (size_t i = 0; i < keys.size(); i++) 
		{
			emmitter << YAML::Key; 
			emmitter << keys[i]; 
			emmitter << YAML::Value; 
			writeNode(node[keys[i]], emmitter); 
		}
		emmitter << YAML::EndMap; 
		break; 
	}
	default:
		emmitter << node; 
		break; 
	}
}
