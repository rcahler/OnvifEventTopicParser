#pragma once
#include <string>
#include "parson.h"
#include <sstream>
#include "device.hpp"
#include "profiles.hpp"
#include "event.hpp"

class GetData {
public: //Constructor
	GetData(std::string user, std::string pass, std::string url, bool);
private://Internal functions
	void ToJsonTopicTwoElements(std::string, std::vector<std::pair<std::string, std::string>>);
	void ToJsonTopicMoreElements(std::string, std::vector<std::pair<std::string, std::string>>);
	void ToJsonTopicOneElement(std::string, std::vector<std::pair<std::string, std::string>>);
	bool IsMotion(std::string);
	bool IsInTrig(std::string);
	std::string FindReferenceToken(JSON_Object*, std::pair<std::string, std::string>);
	JSON_Value* DealWithTypes(std::pair<std::string, std::string>);
public:
	void DataToJson();
	void AddDeviceIO(std::vector<tt__DigitalInput*>, std::vector<tt__RelayOutput*>);
	void AddDeviceIO(std::vector<std::string>, std::vector<tt__RelayOutput*>);
	std::stringstream& returnStream();
	std::string returnManu();
	JSON_Value* returnRoot();
	bool connected;
private:
	std::string m_username;
	std::string m_password;
	std::string m_url;
	bool m_verbose;
	
	std::string event_url;
	std::string media_url;
	std::vector<std::string> motionV;
	std::vector<std::string> inputV;
	std::stringstream stream;
	std::string Manufacturer;
	JSON_Value * root_value;
	Event event;
	std::vector<tt__DigitalInput*> digital_inputs_soap;
	bool diSoap = false;
	std::vector<std::string> digital_inputs_string;
	bool diString = false;
	std::vector<tt__RelayOutput*> relay_outputs;
public:
	std::string io_url;
	Device device;
};
