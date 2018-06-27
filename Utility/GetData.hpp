#pragma once
#include <string>
#include "parson.h"
#include <sstream>




class GetData {
public: //Constructor
	GetData(std::string user, std::string pass, std::string url);
private://Internal functions
	void ToJsonTopicTwoElements(std::string, std::vector<std::pair<std::string, std::string>>);
	void ToJsonTopicMoreElements(std::string, std::vector<std::pair<std::string, std::string>>);
	void ToJsonTopicLessElements(std::string, std::vector<std::pair<std::string, std::string>>);
	bool IsMotion(std::string);
	bool IsInTrig(std::string);
public://Returns
	std::stringstream& returnStream();
	std::string returnManu();
public:
	std::string m_username;
	std::string m_password;
	std::string m_url;
	std::vector<std::string> motionV;
	std::vector<std::string> inputV;
	std::stringstream stream;
	std::string Manufacturer;
};
