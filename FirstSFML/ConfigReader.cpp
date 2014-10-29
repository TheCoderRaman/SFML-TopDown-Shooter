#include "ConfigReader.h"

#include <sstream>
#include <fstream>

ConfigReader::ConfigReader()
	: filepath("Media/Config/config.xml")
	, xmlStr()
{
	std::ifstream file(filepath);
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	xmlStr = buffer.str();

	doc.parse<rapidxml::parse_no_data_nodes>(&xmlStr[0]);
}

std::string ConfigReader::getRootName() {
	return doc.first_node()->name();
}
