#pragma once
#include <string>
#include "parson.h"
#include <sstream>
#include "device.hpp"

class GetData {
public: //Constructor
	GetData(std::string user, std::string pass, std::string url);
private://Internal functions
	void ToJsonTopicTwoElements(std::string, std::vector<std::pair<std::string, std::string>>, Device*);
	void ToJsonTopicMoreElements(std::string, std::vector<std::pair<std::string, std::string>>, Device*);
	void ToJsonTopicLessElements(std::string, std::vector<std::pair<std::string, std::string>>, Device*);
	bool IsMotion(std::string);
	bool IsInTrig(std::string);
	std::string FindReferenceToken(std::string, Device*);
	void DealWithTypes(JSON_Object*, std::pair<std::string, std::string>, Device*);
public://Returns
	std::stringstream& returnStream();
	std::string returnManu();
	JSON_Value* returnRoot();
private:
	std::string m_username;
	std::string m_password;
	std::string m_url;
	std::vector<std::string> motionV;
	std::vector<std::string> inputV;
	std::stringstream stream;
	std::string Manufacturer;
	JSON_Value * root_value;
};
