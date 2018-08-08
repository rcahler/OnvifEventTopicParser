#include "SaveToFile.hpp"

//#include <cstdint>
#include <ctime>
//#include <cstdio>
#include <cerrno>
//Windows specific header files//
#include <windows.h>
#include <AtlBase.h>
#include <atlconv.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "split.h"
#include "parson.h"
#include "removeChar.h"

void SaveToFile(std::string filename, std::string manu, JSON_Value* element) {
	
	//Allows for filepathes which add the json file into a nonexistant folder, even one nested in another newly created folder
	//Was forced to use windows api calls to be able to do this
	std::vector<std::string> strings = split(filename, '/');
	std::wstring base = ToWstring(strings[0]);
	for (size_t i = 1; i < strings.size() - 1; i++) {
		base +=  L'/' + ToWstring(strings[i]);
		if (GetFileAttributes(base.c_str()) == INVALID_FILE_ATTRIBUTES) {
			//creates File path if it does not already exist
			CreateDirectory(base.c_str(), NULL);
		}
	}
	
	//Creates a file if one does not already exist
	std::fstream file;
	file.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);
	if (!file){
		file.open(filename, std::fstream::in | std::fstream::out | std::fstream::trunc);
	}//New file is created
	file.close();
	//

	JSON_Value *root_value;

	//Parson handles the file io here
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

	//Each element of this array is the json data on a specific manufacturer
	JSON_Array* onvif_array = json_object_get_array(onvif_object, "items");

	bool manu_is_in_json = false;


	size_t index = 0;
	for (size_t i = 0; i < json_array_get_count(onvif_array); i++) {
		JSON_Value* manu_value = json_array_get_value(onvif_array, i); //JSON of one individual manufacturer
		JSON_Object* manu_object = json_value_get_object(manu_value);
		json_object_get_value(manu_object, "manufacturer"); 
		std::string specific_manu_json(json_serialize_to_string(json_object_get_value(manu_object, "manufacturer")));
		specific_manu_json = specific_manu_json.substr(1, specific_manu_json.size() - 2);
		if (!specific_manu_json.compare(manu)) {
			manu_is_in_json = true;
			index = i; //Index is for if specific topics need to be added later
		}	
	}

	if (manu_is_in_json) {

		//For indeces of cam topics not in JSON
		std::vector<int> motionVec;
		std::vector<int> inputVec;

		std::cout << "manufacturer: " << manu << " is contained in the JSON, individual topics will be added" << std::endl;
		JSON_Value* from_json_value = json_array_get_value(onvif_array, index);
		JSON_Object* from_json_obj = json_value_get_object(from_json_value);

		JSON_Value* from_cam_value = element;
		JSON_Object* from_cam_obj = json_value_get_object(from_cam_value);
		
		JSON_Array* json_motion_array = json_object_dotget_array(from_json_obj, "motion.topic");
		JSON_Array* cam_motion_array = json_object_dotget_array(from_cam_obj, "motion.topic");
		//Loops through motion topics from the camera
		for (size_t i = 0; i < json_array_get_count(cam_motion_array); ++i) {
			JSON_Value* cam_topic = json_array_get_value(cam_motion_array, i);
			JSON_Object* cam_topic_object = json_value_get_object(cam_topic);

			const char* cam_name = json_object_get_string(cam_topic_object, "name");

			bool in_json = false;

			//Loop through topics in the json, checking each element against the current camera topic
			for (size_t j = 0; j < json_array_get_count(json_motion_array); ++j) {

				JSON_Value* json_topic = json_array_get_value(json_motion_array, j);
				JSON_Object* json_topic_object = json_value_get_object(json_topic);

				//Incorectly formatted json will cause this to crash
				const char* json_name = json_object_get_string(json_topic_object, "name");
				
				if (strcmp(json_name, cam_name) == 0) {
					in_json = true;
				}
			}


			if (!in_json) { //Correctly recognizes
				motionVec.push_back(i);
			}
		}
		for (size_t i = 0; i < motionVec.size(); ++i) {
			int index = motionVec[i];

			//For whatever reason the call does not work if not outputing to a stream
			JSON_Value* value = json_array_get_value(cam_motion_array, index);
			std::stringstream st;
			st << "Trying to append value to motion array: " << json_array_append_value(json_motion_array, value) << std::endl;
			std::cout << st.str() << std::endl;
		}

		JSON_Array* json_input_array = json_object_dotget_array(from_json_obj, "input trigger.topic");
		JSON_Array* cam_input_array = json_object_dotget_array(from_cam_obj, "input trigger.topic");
		//Loop through input topics from the camera
		for (size_t i = 0; i < json_array_get_count(cam_input_array); i++) {
			JSON_Value* cam_topic = json_array_get_value(cam_input_array, i);
			JSON_Object* cam_topic_object = json_value_get_object(cam_topic);

			const char* cam_name = json_object_get_string(cam_topic_object, "name");
			bool in_json = false;

			//Loop through topics in the json, checking each element against the current camera topic
			for (size_t j = 0; j < json_array_get_count(json_input_array); j++) {
				JSON_Value* json_topic = json_array_get_value(json_input_array, j);
				JSON_Object* json_topic_object = json_value_get_object(json_topic);
				//Incorectly formatted json will cause this to crash

				const char* json_name = json_object_get_string(json_topic_object, "name");

				if (strcmp(json_name, cam_name) == 0) {
					in_json = true;
				}
			}

			if (!in_json) { //Correctly recognizes
				inputVec.push_back(i);
			}
		}
		for (size_t i = 0; i < inputVec.size(); ++i) {
			int index = inputVec[i];

			JSON_Value* value = json_array_get_value(cam_input_array, index);

			//For whatever reason the call does not work if not outputing to a stream
			std::stringstream st;
			st << "Trying to append value to input trigger array: " << json_array_append_value(json_input_array,value) << std::endl;
			std::cout << st.str() << std::endl;
		}

		char* string = json_serialize_to_string_pretty(root_value);

		removeChar(string, '\\');

		std::ofstream finalfile;
		finalfile.open(filename);
		finalfile << string;
		finalfile.close();
		json_free_serialized_string(string);

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
