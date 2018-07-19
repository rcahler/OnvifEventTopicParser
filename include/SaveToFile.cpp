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

	//TODO
	/*
	Run checks not just if the manufacturer is in the json, but if each specific topic is aswell
	*/

	size_t index = 0;
	for (size_t i = 0; i < json_array_get_count(onvif_array); i++) {
		JSON_Value* manu_value = json_array_get_value(onvif_array, i); //JSON of one individual manufacturer
		JSON_Object* manu_object = json_value_get_object(manu_value);
		json_object_get_value(manu_object, "manufacturer"); 
		std::string manu_json(json_serialize_to_string(json_object_get_value(manu_object, "manufacturer")));
		manu_json = manu_json.substr(1, manu_json.size() - 2);
		if (!manu_json.compare(manu)) {
			boo = true;
			index = i;
		}	
	}

	if (boo) {
		std::cout << "manufacturer: " << manu << " is contained in the JSON, individual topics will be added" << std::endl;
		JSON_Value* element_value = json_array_get_value(onvif_array, index);
		JSON_Object* element_obj = json_value_get_object(element_value);

		
		JSON_Array* motion_array = json_object_dotget_array(element_obj, "motion.topic");
		JSON_Array* input_array = json_object_dotget_array(element_obj, "input trigger.topic");

		//Iterate through motion topics in json
		for (size_t i = 0; i < json_array_get_count(motion_array); i++) {
			JSON_Value* json_topic = json_array_get_value(motion_array, i);
			JSON_Object* json_topic_object = json_value_get_object(json_topic);

			std::cout << (json_topic_object, "name") << std::endl;
			/*char* string = json_serialize_to_string_pretty(json_topic);
			std::cout << string << std::endl;*/
		}

		//Iterate through input topics in json
		for (size_t i = 0; i < json_array_get_count(input_array); i++) {
			JSON_Value* json_topic = json_array_get_value(input_array, i);
		}


		JSON_Value* m_test = json_value_init_object();
		JSON_Object* m_test_o = json_value_get_object(m_test);

		JSON_Value* i_test = json_value_init_object();
		JSON_Object* i_test_o = json_value_get_object(i_test);

		json_object_set_string(m_test_o, "m_test", "m_test"); //Is added
		json_object_set_string(i_test_o, "i_test", "i_test"); //Is added

		//Works appending values to file, does not change anything else
		json_array_append_value(motion_array, m_test);
		json_array_append_value(input_array, i_test);

		//
		char* string = json_serialize_to_string_pretty(root_value);

		removeChar(string, '\\');

		std::ofstream finalfile;
		finalfile.open(filename);
		finalfile << string;
		finalfile.close();
		json_free_serialized_string(string);
		//

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
		json_free_serialized_string(string);

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
	std::tm tm;
	localtime_s(&tm, &time);

	std::stringstream s;
	s << tm.tm_year + 1900 << "-" << tm.tm_mon + 1 << "-" << tm.tm_mday;

	json_object_set_string(root_object, "version", "1.0.000"); //Check what version this should be
	json_object_set_string(root_object, "modified-date", s.str().c_str());
	json_object_set_string(root_object, "owner", "onvifnvc");

	//Figure out if anything needs to be done for globals
	json_object_dotset_value(root_object, "onvifnvc.items", json_parse_string("[]"));

	return root;
}
