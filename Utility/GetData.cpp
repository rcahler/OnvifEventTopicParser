#include "device.hpp"
#include "event.hpp"
#include "GetData.hpp"
#include "ParseEventProperties.h"
#include "Topic.h"
#include "parson.h"

GetData::GetData(std::string user, std::string pass, std::string url) {
	m_username = user;
	m_password = pass;
	m_url = url;

	Device device(m_username, m_password, m_url);

	if (device.SyncCamTime() != SOAP_OK) {
		//This does not actually mean that the camera cannot be conntacted too, as certain Onvif cameras do not allow you to sync time
		std::cerr << "The camera and local system times could not be synched" << std::endl;
	}

	if (device.GetCapabilities() != SOAP_OK) {
		std::cerr << "Device Capabilities could not be gotten, the camera could not be connected too" << std::endl;
		return;
	}
	
	if (device.GetDeviceInformation() != SOAP_OK) {
		std::cerr << "Device Information could not be gotten" << std::endl;
		return;
	}

	std::string event_url = device.evXaddr;

	Event event(m_username, m_password, event_url);

	if (event.GetEventProperties() != SOAP_OK) {
		std::cerr << "Event Properties could not be gotten" << std::endl;
		return;
	}

	Manufacturer = device.Manufacturer;
	std::string FirmwareVersion = device.FirmwareVersion;
	std::string HardwareId = device.HardwareId;
	std::string Model = device.Model;
	std::string SerialNumber = device.SerialNumber;

	std::vector<soap_dom_element> domVector = event.topics; 

	std::vector<Topic> topics = ParseEventProperties(domVector);

	if (topics.size() < 1) {
		std::cerr << "This camera does not support topics" << std::endl;
		return;
	}
	
	std::cout << "" << std::endl;

	//Goes to correct function to parse
	for (size_t i = 0; i < topics.size(); i++) {
		auto elements = topics[i].elements;
		std::string name = topics[i].name;
		if (elements.size() == 2) {
			ToJsonTopicTwoElements(name, elements);
		}
		else if (elements.size() > 2) {
			ToJsonTopicMoreElements(name, elements);
		}
		else {
			ToJsonTopicLessElements(name, elements);
		}
	}

	root_value = json_value_init_object();
	JSON_Object *root_object = json_value_get_object(root_value);
	
	json_object_set_string(root_object, "manufacturer", Manufacturer.c_str());
	
	if (motionV.size()) {
		std::string mString;
		for (size_t i = 0; i < motionV.size(); i++) {
			if (i == motionV.size() - 1) {
				mString += motionV[i];
			}
			else {
				mString += motionV[i] + ",";
			}
		}
		mString = '[' + mString + ']';
		json_object_dotset_value(root_object, "motion.topic", json_parse_string(mString.c_str()));
	}
	
	if (inputV.size()) {
		std::string iString;
		for (size_t i = 0; i < inputV.size(); i++) {
			if (i == inputV.size() - 1) {
				iString += inputV[i];
			}
			else {
				iString += inputV[i] + ",";
			}
		}
		iString = '[' + iString + ']';
		json_object_dotset_value(root_object, "input trigger.topic", json_parse_string(iString.c_str()));
	}
}

void GetData::ToJsonTopicTwoElements(std::string name, std::vector<std::pair<std::string, std::string>> elements)
{
	if (IsMotion(name)) {
		JSON_Value *motion_value = json_value_init_object();
		JSON_Object *motion_object = json_value_get_object(motion_value);
		json_object_set_string(motion_object, "name", name.c_str());

		JSON_Value *source_value = json_value_init_object();
		JSON_Object *source_object = json_value_get_object(source_value);
		JSON_Value *data_value = json_value_init_object();
		JSON_Object *data_object = json_value_get_object(data_value);

		json_object_set_string(source_object, "name", elements[0].first.c_str());
		json_object_set_string(data_object, "name", elements[1].first.c_str());

		//source
		if (elements[0].second.find("boolean")) {
			json_object_set_string(source_object, "on", "true");
			json_object_set_string(source_object, "off", "false");
		}
		else { //non-boolean data
			json_object_set_string(source_object, "Not", "Boolean");
			json_object_set_string(source_object, "Fix", "Later");
		}


		//data
		if (elements[1].second.find("boolean")) {
			json_object_set_string(data_object, "on", "true");
			json_object_set_string(data_object, "off", "false");
		}
		else { //non-boolean data
			json_object_set_string(data_object, "Not", "Boolean");
			json_object_set_string(data_object, "Fix", "Later");
		}


		json_object_set_value(motion_object, "source", source_value);
		json_object_set_value(motion_object, "data", data_value);

		motionV.push_back(std::string(json_serialize_to_string(motion_value)));
	}
	else if (IsInTrig(name)) {
		JSON_Value *input_value = json_value_init_object();
		JSON_Object *input_object = json_value_get_object(input_value);
		json_object_set_string(input_object, "name", name.c_str());

		//The source values are more likely to be arrays of data, while the data should be the same format
		JSON_Value *source_value = json_value_init_object();
		JSON_Object *source_object = json_value_get_object(source_value);
		JSON_Value *data_value = json_value_init_object();
		JSON_Object *data_object = json_value_get_object(data_value);

		json_object_set_string(source_object, "name", elements[0].first.c_str());
		json_object_set_string(data_object, "name", elements[1].first.c_str());


		json_object_set_string(source_object, "data type", elements[0].second.c_str());


		//data
		if (elements[1].second.find("boolean")) {
			json_object_set_string(data_object, "on", "true");
			json_object_set_string(data_object, "off", "false");
		}
		else { //non-boolean data
			json_object_set_string(data_object, "Not", "Boolean");
			json_object_set_string(data_object, "Fix", "Later");
		}

		json_object_set_value(input_object, "source", source_value);
		json_object_set_value(input_object, "data", data_value);

		inputV.push_back(std::string(json_serialize_to_string(input_value)));
	}
	else {
		//std::cout << "" << std::endl << "Neither motion or input trigger: " << name << std::endl;
	}
}

void GetData::ToJsonTopicMoreElements(std::string name, std::vector<std::pair<std::string, std::string>> elements)
{
	if (IsMotion(name)) {
		/*std::cout << name << std::endl;
		for (int i = 0; i < elements.size(); i++) {
			std::cout << "name:" << elements[i].first << " type:" << elements[i].second << std::endl;
		}
		std::cout << "" << std::endl;*/
	}
	else if (IsInTrig(name)) {
		/*std::cout << name << std::endl;
		for (int i = 0; i < elements.size(); i++) {
			std::cout << "name:" << elements[i].first << " type:" << elements[i].second << std::endl;
		}
		std::cout << "" << std::endl;*/
	}
	else {
		/*std::cout << name << std::endl;
		for (int i = 0; i < elements.size(); i++) {
			std::cout << "name:" << elements[i].first << " type:" << elements[i].second << std::endl;
		}
		std::cout << "" << std::endl;*/
	}

}

void GetData::ToJsonTopicLessElements(std::string name, std::vector<std::pair<std::string, std::string>> elements)
{
	if (IsMotion(name)) {
		JSON_Value *motion_value = json_value_init_object();
		JSON_Object *motion_object = json_value_get_object(motion_value);
		json_object_set_string(motion_object, "name", name.c_str());

		JSON_Value *data_value = json_value_init_object();
		JSON_Object *data_object = json_value_get_object(data_value);

		json_object_set_string(data_object, "name", elements[0].first.c_str());

		//data
		if (elements[0].second.find("boolean")) {
			json_object_set_string(data_object, "on", "true");
			json_object_set_string(data_object, "off", "false");
		}
		else { //non-boolean data
			json_object_set_string(data_object, "Not", "Boolean");
			json_object_set_string(data_object, "Fix", "Later");
		}

		json_object_set_value(motion_object, "data", data_value);

		motionV.push_back(std::string(json_serialize_to_string(motion_value)));
	}
	else if (IsInTrig(name)) {
		JSON_Value *input_value = json_value_init_object();
		JSON_Object *input_object = json_value_get_object(input_value);
		json_object_set_string(input_object, "name", name.c_str());

		//The source values are more likely to be arrays of data, while the data should be the same format
		JSON_Value *data_value = json_value_init_object();
		JSON_Object *data_object = json_value_get_object(data_value);
		json_object_set_string(data_object, "name", elements[0].first.c_str());

		//data
		if (elements[0].second.find("boolean")) {
			json_object_set_string(data_object, "on", "true");
			json_object_set_string(data_object, "off", "false");
		}
		else { //non-boolean data
			json_object_set_string(data_object, "Not", "Boolean");
			json_object_set_string(data_object, "Fix", "Later");
		}

		json_object_set_value(input_object, "data", data_value);
		inputV.push_back(std::string(json_serialize_to_string(input_value)));
	}
	else {
		/*std::cout << "ToJsonTopicLessElements: " << "Else" << std::endl;
		std::cout << name << std::endl;
		std::cout << elements[0].first << std::endl;
		std::cout << elements[0].second << std::endl;
		std::cout << "" << std::endl;*/
	}
}

bool GetData::IsMotion(std::string s)
{
	bool boo;
	if (s.find("Video") != -1 || s.find("RuleEngine") != -1) {
		boo = true;
	}
	else {
		boo = false;
	}
	return boo;
}

bool GetData::IsInTrig(std::string s)
{
	bool boo;
	if (s.find("Device") != -1 || s.find("UserAlarm") != -1) {
		boo = true;
	}
	else {
		boo = false;
	}
	return boo;
}

std::stringstream& GetData::returnStream()
{
	char *serialized_string = NULL;
	serialized_string = json_serialize_to_string_pretty(root_value);
	stream << serialized_string;
	json_free_serialized_string(serialized_string);
	return stream;
}

std::string GetData::returnManu()
{
	return Manufacturer;
}

JSON_Value * GetData::returnRoot()
{
	return root_value;
}
