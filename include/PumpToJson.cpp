#include <sstream>
#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>
#include <string>
#include "split.h"
#include "PumpToJson.hpp"
#include "parson.h"

class Topic {
public:
	std::string name;
	std::vector<std::pair<std::string, std::string>> elements;
};

//All motion events contain the words motion, video, or ruleengine
// "Device/Trigger/DigitalInput/", "UserAlarm/GeneralAlarm/", "Device/IO/Port/", "Device/Trigger/Relay/"
void PumpDataJson(Camera cam, DeviceData deviceData, EventData eventData) {

	std::stringstream stream;
	stream << boolalpha;

	//For whether or not the camera has these topics
	bool motion = false; //Figure out how to set these
	bool input_trigger = false;
	bool analytics = false;
	bool options = false;

	if (deviceData.capData.caps->Extension != NULL) {

		tt__AnalyticsDeviceCapabilities* AnalyticsDeviceCapabilities = deviceData.capData.caps->Extension->AnalyticsDevice; //Extension
		if (AnalyticsDeviceCapabilities != NULL) {
			soap_dom_attribute dom = AnalyticsDeviceCapabilities->__anyAttribute;
			for (soap_dom_attribute::iterator itr = dom.att_begin(); itr != dom.att_end(); ++itr) {
				stream << itr.name << ": " << itr.nstr << endl;
			}
		}
	}
	//GetEventProperties
	std::vector<soap_dom_element> dom = eventData.resp.wstop__TopicSet->__any;
	std::vector<std::vector<std::string>> EventProperties = ParseEventProperties(dom);
	std::vector<Topic> topics;

	//Move functionality into ParseEventProperties

	for (int i = 0; i < EventProperties.size(); i++) {

		Topic topic;
		topic.name = EventProperties[i][0];

		std::vector<std::pair<std::string, std::string>> elements;

		for (int k = 1; k < EventProperties[i].size(); k += 2) {
			std::pair<std::string, std::string> pair = make_pair(EventProperties[i][k], EventProperties[i][k + 1]);
			elements.push_back(pair);
		}
		topic.elements = elements;

		topics.push_back(topic);

	}


	std::string Manu = deviceData.devInfo.response.Manufacturer;
	std::string name = "TEST";

	JSON_Value *root_value = json_value_init_object();
	JSON_Object *root_object = json_value_get_object(root_value);
	char *serialized_string = NULL;
	json_object_set_string(root_object, "manufacturer", Manu.c_str());

	std::vector<std::string> motionV;
	std::vector<std::string> inputV;
	std::vector<std::string> analyticsV;
	std::vector<std::string> optionsV;

	for (int i = 0; i < topics.size(); i++) {

		std::string st = topics[i].name;

		if (st.find("Video") != -1 || st.find("RuleEngine") != -1) {//motion
			JSON_Value *motion_value = json_value_init_object();
			JSON_Object *motion_object = json_value_get_object(motion_value);

			JSON_Value *data_value = json_value_init_object();
			JSON_Object *data_object = json_value_get_object(data_value);
			
			if (topics[i].elements.size() > 1) {
				json_object_set_string(data_object, "name", topics[i].elements[1].first.c_str());
			}
			else {
				std::cerr << "Issue at: " << __LINE__ << " with " << st << std::endl;
			}
			
			//Not yet sure how to get the values that go along with this
		
			json_object_set_string(motion_object, "name", st.c_str());
			json_object_set_value(motion_object, "data", data_value);

			motionV.push_back(string(json_serialize_to_string(motion_value)));
		}
		else if (st.find("Device") != -1) { //Input Trigger
			JSON_Value *input_value = json_value_init_object();
			JSON_Object *input_object = json_value_get_object(input_value);

			JSON_Value *data_value = json_value_init_object();
			JSON_Object *data_object = json_value_get_object(data_value);

			if (topics[i].elements.size() > 1) {
				json_object_set_string(data_object, "name", topics[i].elements[1].first.c_str());
			}
			else {
				std::cerr << "Issue at: " << __LINE__ << " with " << st << std::endl;
			}

			//Not yet sure how to get the values that go along with this

			json_object_set_string(input_object, "name", st.c_str());
			json_object_set_value(input_object, "data", data_value);

			inputV.push_back(string(json_serialize_to_string(input_value)));
		}
		else if (st.find("/IVA/") != -1) {//Analytics
			//cout << "Analytics" << endl;
			//json_object_dotset_value(root_object, "analytics.Topic", json_parse_string(s.c_str()));
		}
	}


	std::string mString;
	for (int i = 0; i < motionV.size(); i++) {
		if (i == motionV.size() - 1) {
			mString += motionV[i];
		}
		else {
			mString += motionV[i] + ",";
		}
	}

	std::string iString;
	for (int i = 0; i < inputV.size(); i++) {
		if (i == inputV.size() - 1) {
			iString += inputV[i];
		}
		else {
			iString += inputV[i] + ",";
		}
	}

	mString = '[' + mString + ']';
	iString = '[' + iString + ']';

	json_object_dotset_value(root_object, "motion.topic", json_parse_string(mString.c_str()));
	json_object_dotset_value(root_object, "input trigger.topic", json_parse_string(iString.c_str()));

	serialized_string = json_serialize_to_string_pretty(root_value);
	puts(serialized_string);
	json_free_serialized_string(serialized_string);
	json_value_free(root_value);

}