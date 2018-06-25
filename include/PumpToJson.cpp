#include <sstream>
#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>
#include <string>
#include "split.h"
#include "PumpToJson.hpp"
#include "parson.h"
#include "soapMediaBindingProxy.h"
#include "soapRuleEngineBindingProxy.h"

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

			json_object_set_string(motion_object, "name", st.c_str());
			
			if (topics[i].elements.size() == 2) {

				JSON_Value *source_value = json_value_init_object();
				JSON_Object *source_object = json_value_get_object(source_value);

				JSON_Value *data_value = json_value_init_object();
				JSON_Object *data_object = json_value_get_object(data_value);

				json_object_set_string(source_object, "name", topics[i].elements[0].first.c_str());
				json_object_set_string(data_object, "name", topics[i].elements[1].first.c_str());


				//Taking into account that different cameras use different boolean types
				if (topics[i].elements[1].second.find("boolean")) {
					json_object_set_string(data_object, "on", "true");
					json_object_set_string(data_object, "off", "false");
				}
				else { //non-boolean data
					json_object_set_string(data_object, "Not", "Boolean");
					json_object_set_string(data_object, "Fix", "Later");
				}

				//Need to deal with datatypes for the source

				json_object_set_value(motion_object, "source", source_value);
				json_object_set_value(motion_object, "data", data_value);
			}
			else {
				//Dealing with more than the regular number of inputs
				std::cout << "TEST 1 " << std::endl;
				for (int q = 0; q < topics[i].elements.size(); q++) {
					std::cout << "TEST 2 " << std::endl;

					if (topics[i].elements[q].first.find("VideoAnalyticsConfigurationToken")) {
						MediaBindingProxy mediaProxy;

						mediaProxy.soap_endpoint = cam.url.c_str();

						soap_wsse_add_Security(&mediaProxy);
						soap_wsse_add_UsernameTokenDigest(&mediaProxy, "Id", cam.username.c_str(), cam.password.c_str());
						_trt__GetProfiles GP;
						_trt__GetProfilesResponse GPresp;
						std::cout << "TEST 3 " << std::endl;
						if (mediaProxy.GetProfiles(&GP, GPresp) == SOAP_OK) {
							//std::cout << "VideoAnalyticsConfigurationToken && Profiles" << std::endl;
							auto profiles = GPresp.Profiles;
					
							std::cout << "TEST 4 " << std::endl;
							for (int k = 0; k < profiles.size(); k++) {

								std::cout << "TEST 5 " << std::endl;
								RuleEngineBindingProxy ruleEngine;

								std::cout << deviceData.capData.caps->Analytics << std::endl;
								std::cout << "TEST 6 " << std::endl;

								if (deviceData.capData.caps->Analytics) {
									std::cout << "TEST IF" << std::endl;
									ruleEngine.soap_endpoint = cam.url.c_str();
								}
								else {
									std::cout << "TEST ELSE" << std::endl;
									ruleEngine.soap_endpoint = deviceData.capData.caps->Analytics->XAddr.c_str();
								}
								

								

								soap_wsse_add_Security(&ruleEngine);
								soap_wsse_add_UsernameTokenDigest(&ruleEngine, "Id", cam.username.c_str(), cam.password.c_str());

								_tan__GetSupportedRules GSR;
								GSR.ConfigurationToken = profiles[k]->VideoAnalyticsConfiguration->token;
								_tan__GetSupportedRulesResponse GSRresp;

								if (ruleEngine.GetSupportedRules(&GSR, GSRresp) == SOAP_OK) {
									std::cout << "TEST 7 " << std::endl;
									if (GSRresp.SupportedRules->RuleDescription.size() > 0) {
										std::cout << "TEST 8 " << std::endl;
										for (int j = 0; j < GSRresp.SupportedRules->RuleDescription.size(); j++) {
											std::cout << "TEST 9 " << std::endl;
											if (GSRresp.SupportedRules->RuleDescription[j]->Messages.size() > 0) {
												for (int g = 0; g < GSRresp.SupportedRules->RuleDescription[j]->Messages.size(); g++) {

													std::cout << GSRresp.SupportedRules->RuleDescription[j]->Messages[g].ParentTopic << std::endl;
													if (GSRresp.SupportedRules->RuleDescription[j]->Messages[g].Source) {
														if (GSRresp.SupportedRules->RuleDescription[j]->Messages[g].Source->SimpleItemDescription.size() > 0) {
															//std::cout << "source: ";
															//std::cout << GSRresp.SupportedRules->RuleDescription[j]->Messages[g].Source->SimpleItemDescription[0].Name << std::endl;
														}
													}
													if (GSRresp.SupportedRules->RuleDescription[j]->Messages[g].Key) {
														if (GSRresp.SupportedRules->RuleDescription[j]->Messages[g].Key->SimpleItemDescription.size() > 0) {
															//std::cout << "key: ";
															//std::cout << GSRresp.SupportedRules->RuleDescription[j]->Messages[g].Key->SimpleItemDescription[0].Name << std::endl;
														}
													}
													if (GSRresp.SupportedRules->RuleDescription[j]->Messages[g].Data) {
														if (GSRresp.SupportedRules->RuleDescription[j]->Messages[g].Data->SimpleItemDescription.size() > 0) {
															//std::cout << "data: ";
															//std::cout << GSRresp.SupportedRules->RuleDescription[j]->Messages[g].Data->SimpleItemDescription[0].Name << std::endl;
														}
													}
													//std::cout << "" << endl;
												}
											}
										}
									}
								}
							}
						}
						else {
							//std::cout << "VideoAnalyticsConfigurationToken && No Profiles" << std::endl;
						}
						//std::cout << "" << std::endl;
					}
				}
				//std::cout << "" << std::endl;
			}

			motionV.push_back(string(json_serialize_to_string(motion_value)));
			//std::cout << "" << std::endl;
		}
		//else if (st.find("Device") != -1) { //Input Trigger
		//	JSON_Value *input_value = json_value_init_object();
		//	JSON_Object *input_object = json_value_get_object(input_value);

		//	JSON_Value *data_value = json_value_init_object();
		//	JSON_Object *data_object = json_value_get_object(data_value);

		//	if (topics[i].elements.size() > 1) {
		//		json_object_set_string(data_object, "name", topics[i].elements[1].first.c_str());
		//		for (int q = 0; q < topics[i].elements.size(); q++) {
		//			std::cout << "Element " << q << " first " << topics[i].elements[q].first << std::endl;
		//			std::cout << "Element " << q << " second " << topics[i].elements[q].second << std::endl;
		//		}
		//		std::cout << "" << std::endl;
		//	}
		//	else {
		//		std::cerr << "Issue at: " << __LINE__ << " with " << st << std::endl;
		//		for (int q = 0; q < topics[i].elements.size(); q++) {
		//			std::cout << "Element " << q << " first " << topics[i].elements[q].first << std::endl;
		//			std::cout << "Element " << q << " second " << topics[i].elements[q].second << std::endl;
		//		}
		//		std::cout << "" << std::endl;
		//	}

		//	//Not yet sure how to get the values that go along with this

		//	json_object_set_string(input_object, "name", st.c_str());
		//	json_object_set_value(input_object, "data", data_value);

		//	inputV.push_back(string(json_serialize_to_string(input_value)));
		//	std::cout << "" << std::endl;
		//}
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