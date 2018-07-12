#include "SaveToFile.hpp"
#include "removeChar.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdint>
#include <windows.h>
#include <AtlBase.h>
#include <atlconv.h>
#include <ctime>
#include <split.h>
#include "parson.h"
#include <string>
#include <cstdio>
#include <cerrno>

void SaveToFile(std::string filename, std::string manu, JSON_Value* element) {
	
	//Allows for filepathes which add the json file into a nonexistant folder, even one nested in another newly created folder
	std::vector<std::string> strings = split(filename, '/');
	std::wstring base = ToWstring(strings[0]);
	for (size_t i = 1; i < strings.size() - 1; i++) {
		base +=  L'/' + ToWstring(strings[i]);
		if (GetFileAttributes(base.c_str()) == INVALID_FILE_ATTRIBUTES) {
			//creates File path if it does not already exist
			CreateDirectory(base.c_str(), NULL);
		}
	}
	
	std::fstream file;
	file.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);
	if (!file){
		file.open(filename, std::fstream::in | std::fstream::out | std::fstream::trunc);
	}//New file is created

	file.close();

	JSON_Value *root_value;
	root_value = json_parse_file(filename.c_str());

	if (json_parse_file(filename.c_str())) {
		root_value = json_parse_file(filename.c_str());
	}
	else {
		root_value = CreateJsonHeader();
	}

	JSON_Object *object = json_value_get_object(root_value);

	JSON_Value* onvif_value = json_object_get_value(object, "onvifnvc");
	JSON_Object* onvif_object = json_value_get_object(onvif_value);

	JSON_Array* onvif_array = json_object_get_array(onvif_object, "items");

	
	bool boo = false;
	JSON_Value* existing_topic_value;

	//TODO
	/*
	Run checks not just if the manufacturer is in the json, but if each specific topic is aswell
	*/

	for (size_t i = 0; i < json_array_get_count(onvif_array); i++) {
		JSON_Value* manu_value = json_array_get_value(onvif_array, i); //JSON of one individual manufacturer
		JSON_Object* manu_object = json_value_get_object(manu_value);
		json_object_get_value(manu_object, "manufacturer");
		std::string manu_json(json_serialize_to_string(json_object_get_value(manu_object, "manufacturer")));
		manu_json = manu_json.substr(1, manu_json.size() - 2);
		if (!manu_json.compare(manu)) {
			boo = true;
			existing_topic_value = manu_value;
		}
	}

	if (boo) {
		std::cout << "manufacturer: " << manu << " is contained in the JSON, individual topics will be added" << std::endl;

		JSON_Object* existing_topic_object = json_value_get_object(existing_topic_value);

		JSON_Value* element_value = element;
		JSON_Object* element_object = json_value_get_object(element_value);

		//JSON values from the camera
		JSON_Value* element_motion_topic_value = json_object_get_value(element_object, "motion");
		JSON_Array* element_motion_array = json_object_get_array(json_value_get_object(element_motion_topic_value), "topic");

		//JSON values from the json file
		JSON_Value* motion_topic_value = json_object_get_value(existing_topic_object, "motion");
		JSON_Array* motion_array = json_object_get_array(json_value_get_object(motion_topic_value), "topic");

		for (size_t i = 0; i < json_array_get_count(element_motion_array); ++i) {
			JSON_Value* value_i = json_array_get_value(element_motion_array, i);
			std::string string_i = std::string(json_serialize_to_string(json_object_get_value(json_value_get_object(value_i), "name")));
			bool boo = false;
			for (size_t j = 0; j < json_array_get_count(motion_array); ++j) {
				JSON_Value* value_j = json_array_get_value(motion_array, j);
				std::string string_j = std::string(json_serialize_to_string(json_object_get_value(json_value_get_object(value_j), "name")));
				if (string_i.compare(string_j) == 0) {//Element exists within the json
					boo = true;
				}
			}
			if (boo == false) {//Does not currently add the element in
				//std::cout << json_serialize_to_string_pretty(value_i) << std::endl;
				json_array_append_value(motion_array, value_i);
			}
		}


		JSON_Value* element_input_topic_value = json_object_get_value(element_object, "input trigger");
		JSON_Array* element_input_array = json_object_get_array(json_value_get_object(element_input_topic_value), "topic");

		JSON_Value* input_topic_value = json_object_get_value(existing_topic_object, "input trigger");
		JSON_Array* input_array = json_object_get_array(json_value_get_object(input_topic_value), "topic");

		for (size_t i = 0; i < json_array_get_count(element_input_array); ++i) {
			bool boo = false;
			JSON_Value* value_i = json_array_get_value(element_input_array, i);
			std::string string_i = std::string(json_serialize_to_string(json_object_get_value(json_value_get_object(value_i), "name")));
			for (size_t j = 0; j < json_array_get_count(input_array); ++j) {
				JSON_Value* value_j = json_array_get_value(input_array, j);
				std::string string_j = std::string(json_serialize_to_string(json_object_get_value(json_value_get_object(value_j), "name")));
				if (string_i.compare(string_j) == 0) {//Element exists within the json
					boo = true;
				}
			}
			if (boo == false) {//Does not yet work, accessing through index in original array??
				//std::cout << json_serialize_to_string_pretty(value_i) << std::endl;
				json_array_append_value(input_array, value_i);
			}
		}

		std::cout << json_serialize_to_string_pretty(root_value) << std::endl;


		return;
	}
	else {
		std::cout << "manufacturer: " << manu << " is not contained in the JSON" << std::endl;

		json_array_append_value(onvif_array, element);

		char* string = json_serialize_to_string_pretty(root_value);

		removeChar(string, '\\');

		std::ofstream finalfile;
		finalfile.open(filename);
		finalfile << string;
		finalfile.close();

		return;
	}
}

std::wstring ToWstring(std::string s)
{
	CA2W ca2w(s.c_str());
	std::wstring w = ca2w.m_psz;
	return w;
}

JSON_Value* CreateJsonHeader() {
	JSON_Value* root = json_value_init_object();
	JSON_Object *root_object = json_value_get_object(root);

	std::time_t time = std::time(NULL);
	std::tm* tm = std::localtime(&time);

	std::stringstream s;
	s << tm->tm_year + 1900 << "-" << tm->tm_mon + 1 << "-" << tm->tm_mday; 

	json_object_set_string(root_object, "version", "1.0.000"); //Check what version this should be
	json_object_set_string(root_object, "modified-date", s.str().c_str());
	json_object_set_string(root_object, "owner", "onvifnvc");

	//Figure out if anything needs to be done for globals
	json_object_dotset_value(root_object, "onvifnvc.items", json_parse_string("[]"));

	return root;
}