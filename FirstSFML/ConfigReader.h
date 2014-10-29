#ifndef CONFIG_READER_H
#define CONFIG_READER_H
#include "rapidxml.hpp"

#include <string>
#include <map>
class ConfigReader {

	public:
		ConfigReader();
	public:
		rapidxml::xml_document<> doc;

		std::string getRootName();

		const std::string filepath;
		std::string xmlStr;

};


#endif