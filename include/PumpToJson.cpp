#include <sstream>
#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>
#include <string>
#include "split.h"
#include "PumpToJson.hpp"
#include "parson.h"

using namespace std;

class Topic {
public:
	string name;
	vector<pair<string, string>> elements;
};

//All motion events contain the words motion, video, or ruleengine
// "Device/Trigger/DigitalInput/", "UserAlarm/GeneralAlarm/", "Device/IO/Port/", "Device/Trigger/Relay/"
void PumpDataJson(Camera cam, DeviceData deviceData, EventData eventData) {

	stringstream stream;
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
	vector<soap_dom_element> dom = eventData.resp.wstop__TopicSet->__any;
	vector<vector<string>> EventProperties = ParseEventProperties(dom);
	vector<Topic> topics;

	//Move functionality into ParseEventPropertiesv 
	for (int i = 0; i < EventProperties.size(); i++) {

		Topic topic;
		topic.name = EventProperties[i][0];

		vector<pair<string, string>> elements;

		for (int k = 1; k < EventProperties[i].size(); k += 2) {
			pair<string, string> pair = make_pair(EventProperties[i][k], EventProperties[i][k + 1]);
			elements.push_back(pair);
		}
		topic.elements = elements;

		topics.push_back(topic);

	}


	string Manu = deviceData.devInfo.response.Manufacturer;
	string name = "TEST";

	JSON_Value *root_value = json_value_init_object();
	JSON_Object *root_object = json_value_get_object(root_value);
	char *serialized_string = NULL;
	json_object_set_string(root_object, "manufacturer", Manu.c_str());
	JSON_Value *motion_value = json_value_init_object();
	JSON_Object *motion_object = json_value_get_object(motion_value);
	json_object_set_string(motion_object, "name", name.c_str());

	string s = "[" + string(json_serialize_to_string(motion_value)) + ']'; //Automate this with an unknown number of parameters, seperate function probablt

	for (int i = 0; i < topics.size(); i++) {
		//cout << "name: " << topics[i].name << endl;

		string st = topics[i].name;

		//There doesn't really seem to be a better way of doing this, still need to deal with options
		if (st.find("Video") != -1 || st.find("RuleEngine") != -1) {//motion
			cout << "Motion" << endl;
			json_object_dotset_value(root_object, "motion.Topic", json_parse_string(s.c_str()));
		}
		else if (st.find("Device") != -1) { //Input Trigger
			cout << "Input Trigger" << endl;
			json_object_dotset_value(root_object, "input trigger.Topic", json_parse_string(s.c_str()));
		}
		else if (st.find("/IVA/") != -1) {//Analytics
			cout << "Analytics" << endl;
			json_object_dotset_value(root_object, "analytics.Topic", json_parse_string(s.c_str()));
		}
	}

	serialized_string = json_serialize_to_string_pretty(root_value);
	puts(serialized_string);
	json_free_serialized_string(serialized_string);
	json_value_free(root_value);

}