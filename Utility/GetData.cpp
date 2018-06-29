#include "device.hpp"
#include "event.hpp"
#include "profiles.hpp"
#include "GetData.hpp"
#include "ParseEventProperties.h"
#include "soapRecordingBindingProxy.h"
#include "Topic.h"
#include "parson.h"

GetData::GetData(std::string user, std::string pass, std::string url) {
	m_username = user;
	m_password = pass;
	m_url = url;

	profile.SetParameters(m_username, m_password, m_url);
	if (profile.GetProfiles()) {
		std::cout << "HERE" << std::endl;
	}

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

	/*std::cout << "Number of topics: " << topics.size() << std::endl;
	for (int i = 0; i < topics.size(); i++) {
		std::cout << "Topic " << i << " is " << topics[i].name << std::endl;
	}*/

	std::cout << "" << std::endl;

	//Goes to correct function to parse
	for (size_t i = 0; i < topics.size(); i++) {
			auto elements = topics[i].elements;
			std::string name = topics[i].name;
			if (elements.size() == 2) {
				ToJsonTopicTwoElements(name, elements, &device);
			}
			else if (elements.size() > 2) {
				ToJsonTopicMoreElements(name, elements, &device);
			}
			else if (elements.size() == 1) {
				ToJsonTopicLessElements(name, elements, &device);
			}
			else {
				std::cerr << "ERROR" << std::endl;
			}
		}

	root_value = json_value_init_object();

	JSON_Object *root_object = json_value_get_object(root_value);

	json_object_set_string(root_object, "manufacturer", Manufacturer.c_str());

		//Adds all motion topics to the Json
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

		//Adds all input trigger topics to the Json
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

void GetData::ToJsonTopicTwoElements(std::string name, std::vector<std::pair<std::string, std::string>> elements, Device* device)
{


	JSON_Value *topic_value = json_value_init_object();
	JSON_Object *topic_object = json_value_get_object(topic_value);
	json_object_set_string(topic_object, "name", name.c_str());

	JSON_Value *source_value = json_value_init_object();
	JSON_Object *source_object = json_value_get_object(source_value);
	JSON_Value *data_value = json_value_init_object();
	JSON_Object *data_object = json_value_get_object(data_value);

	json_object_set_string(source_object, "name", elements[0].first.c_str());
	json_object_set_string(data_object, "name", elements[1].first.c_str());

	DealWithTypes(source_object, elements[0], device);
	DealWithTypes(data_object, elements[1], device);
	
	RecordingBindingProxy record;
	if (device->GCresp.Capabilities->Extension) {
		if (device->GCresp.Capabilities->Extension->Recording) {
			//record.soap_endpoint = device->GCresp.Capabilities->Extension->Recording->XAddr.c_str();
			record.soap_endpoint = device->deXaddr.c_str();
			_trc__GetServiceCapabilities gsc;
			_trc__GetServiceCapabilitiesResponse gscresp;

			record.GetServiceCapabilities(&gsc, gscresp);

			//std::cout << "DynamicRecordings: " << gscresp.Capabilities->DynamicRecordings << std::endl;
		}
	}
	


	json_object_set_value(topic_object, "source", source_value);
	json_object_set_value(topic_object, "data", data_value);

	if (IsMotion(name)) {
		motionV.push_back(std::string(json_serialize_to_string(topic_value)));
	}
	else if (IsInTrig(name)) {
		inputV.push_back(std::string(json_serialize_to_string(topic_value)));
	}
	else {
		//std::cout << "Neither motion or input trigger: " << name << std::endl;
	}
	return;
}

void GetData::ToJsonTopicMoreElements(std::string name, std::vector<std::pair<std::string, std::string>> elements, Device* device)
{
	JSON_Value *topic_value = json_value_init_object();
	JSON_Object *topic_object = json_value_get_object(topic_value);
	if (IsMotion(name)) {
		motionV.push_back(std::string(json_serialize_to_string(topic_value)));
	}
	else if (IsInTrig(name)) {
		inputV.push_back(std::string(json_serialize_to_string(topic_value)));
	}
	else {
		//std::cout << "Neither motion or input trigger: " << name << std::endl;
	}

	return;
}

void GetData::ToJsonTopicLessElements(std::string name, std::vector<std::pair<std::string, std::string>> elements, Device* device)
{
	JSON_Value *topic_value = json_value_init_object();
	JSON_Object *topic_object = json_value_get_object(topic_value);
	json_object_set_string(topic_object, "name", name.c_str());

	JSON_Value *data_value = json_value_init_object();
	JSON_Object *data_object = json_value_get_object(data_value);

	json_object_set_string(data_object, "name", elements[0].first.c_str());

	//data
	if (elements[0].second.find("boolean") != std::string::npos) {
		json_object_set_string(data_object, "on", "true");
		json_object_set_string(data_object, "off", "false");
	}
	else if ((elements[0].second.find("token") != std::string::npos) || (elements[0].second.find("Token") != std::string::npos)) {
		std::string token = FindReferenceToken(elements[0].second, device);
		json_object_set_string(data_object, "token", token.c_str());
	}
	else { //non-boolean data
		json_object_set_string(data_object, "datatype", elements[0].second.c_str());
	}

	json_object_set_value(topic_object, "data", data_value);
	if (IsMotion(name)) {
		if (!std::string(json_serialize_to_string(topic_value)).empty()) {
			motionV.push_back(std::string(json_serialize_to_string(topic_value)));
		}
	}
	else if (IsInTrig(name)) {
		if (!std::string(json_serialize_to_string(topic_value)).empty()) {
			inputV.push_back(std::string(json_serialize_to_string(topic_value)));
		}
	}
	else {
		//std::cout << "Neither motion or input trigger: " << name << std::endl;
	}
	return;
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

std::string GetData::FindReferenceToken(std::string s, Device* device)
{
	if (s.find("AlarmInToken") != std::string::npos) {
		
	}
	else if (s.find("InputToken") != std::string::npos) {
		
	}
	else if (s.find("RelayToken") != std::string::npos) {
		
	}
	else if (s.find("ReferenceToken") != std::string::npos) {//I think this is the video source tokens

	}
	else {
		std::cerr << "Unrecognized token: " << s << std::endl;
	}

	std::string token = s;
	return token;
}

void GetData::DealWithTypes(JSON_Object* json, std::pair<std::string, std::string> pair, Device* device)
{

	if (pair.second.find("boolean") != std::string::npos) {//Figure out how to differentiate the booleans
		json_object_set_string(json, "boolean2", pair.second.c_str());
	}
	else if ((pair.first.find("Token") != std::string::npos) || (pair.second.find("Token") != std::string::npos)) {
		std::string token = FindReferenceToken(pair.second, device);
		json_object_set_string(json, "token2", token.c_str());

		


	}
	else if (pair.second.find("string") != std::string::npos) {
		json_object_set_string(json, "name", pair.first.c_str());
		json_object_set_string(json, "datatype", pair.second.c_str());
	}
	else if (pair.second.find("int") != std::string::npos) {
		json_object_set_string(json, "name", pair.first.c_str());
		json_object_set_string(json, "datatype", pair.second.c_str());
	}
	else {
		json_object_set_string(json, "name", pair.first.c_str());
		json_object_set_string(json, "datatype", pair.second.c_str());
	}
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
