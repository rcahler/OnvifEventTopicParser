#define MAX_URL_LENGTH 128
#include "Credentials.hpp"

#include <fstream>
#include <iostream>
#include <list>
#include <iterator>
#include <string>
#include <algorithm>

#include "soapDeviceBindingProxy.h"
#include "soapStub.h"

#include "split.h"

//The credentials class handles the users inputs, and makes it usefull for the rest of the program.
//It also handles checking to make sure things like the ip address are correctly formatted.
Credentials::Credentials(int argc, char* argv[]) {

	username = NULL;
	password = NULL;
	ip = NULL;
	filepath = NULL;
	port = NULL;
	
	for (size_t i = 1; i < argc; ++i) {

		bool old_way = false;
		if (strcmp(argv[i], "-u") == 0) { //username
			if ((i + 1) <= argc) {
				username = argv[i + 1];
			}
		}
		else if (strcmp(argv[i], "-p") == 0) { //password
			if ((i + 1) <= argc) {
				password = argv[i + 1];
			}
		}
		else if (strcmp(argv[i], "-ip") == 0) { //ip address
			if ((i + 1) <= argc) {
				ip = argv[i + 1];
			}
		}
		else if (strcmp(argv[i], "-f") == 0) { //filepath
			if ((i + 1) <= argc) {
				filepath = argv[i + 1];
			}
		}
		else if (strcmp(argv[i], "-h") == 0) { //help
			help = true;
		}
		else if (strcmp(argv[i], "-v") == 0) { //verbosity
			verbose = true;
		} 
		else if (strcmp(argv[i], "-port") == 0) { //port number
			if ((i + 1) <= argc) {
				port = argv[i + 1];
			}
		}
		
		//old_way allows the user to enter data with the following format
		//Utility.exe username password ip_address filepath
		//This is just so I don't have to re-write my batch files, delete later
		if (strcmp(argv[argc - 1], "-o") == 0) {
			old_way = true;
		}

		if (old_way) {
			username = argv[1];
			password = argv[2];
			ip = argv[3];
			if ((argv[4]) && (argc >= 5)) {
				filepath = argv[4];
			}
		}
	}

	//Not all cams require a password, but the utility was written assuming they do
	//Adds placeholder
	if (password == NULL) {
		password = new char[8];
		strcpy_s(password, 8, "NO_PASS");
	}
}

//Error code numbers explained in printCredErrors(int)
int Credentials::ValidateCreds()
{
	if (help) {
		printHelp();
	}


	//Error code 1
	if ((!username) || (!ip)) {
		return 1;
	}

	//Checks ipAddress
	std::vector<std::string> ipVec = split(std::string(ip), '.');
	if (ipVec.size() != 4) {
		return 2;
	}
	for (int i = 0; i < 4; i++) {
		if (ipVec[i].find_first_not_of("[0123456789]") != std::string::npos) {
			return 2;
		}
		int j = stoi(ipVec[i]);
		if (j > 255 || j < 0) {
			return 2;
		}
	}
	char xAddr[MAX_URL_LENGTH] = { 0 };
	strcat_s(xAddr, MAX_URL_LENGTH, "http://");
	strcat_s(xAddr, MAX_URL_LENGTH, ip);
	if (port) {
		std::string p = ":" + std::string(port);
		strcat_s(xAddr, MAX_URL_LENGTH, p.c_str());
	}
	strcat_s(xAddr, MAX_URL_LENGTH, "/onvif/device_service");

	url = xAddr;

	//Checks valid json file
	if (filepath) {
		if (strlen(filepath) <= 5) {
			return 3;
		}
		char filetype[6];
		int j = 0;
		for (size_t i = strlen(filepath) - 5; i < strlen(filepath); ++i) {
			filetype[j] = filepath[i];
			++j;
		}
		filetype[5] = '\0'; //Null terminator
		if (!strcmp(filetype, ".json") == 0) {return 3;}
	}
	return 0;
}

void printCredErrors(int error)
{
	std::string usage;
	switch (error) {
	case 1:
		std::cerr << "A username and an ip address must be supplied to communicate with all cameras" << std::endl;
		break;
	case 2:
		std::cerr << "Malformed IPv4 address" << std::endl;
		break;
	case 3:
		std::cerr << "If pumping the data to a file, list a valid filepath" << std::endl;
	}
	return;
}

void printHelp() {
	std::cerr << "*Help*" << std::endl;
	std::cerr << "----------" << std::endl;
	std::cerr << "Commands:" << std::endl;
	std::cerr << "	-h: Optional, Brings up all avaliable commands and usage information" << std::endl;
	std::cerr << "	-v: Optional, Verbose output" << std::endl;
	std::cerr << "	-u: Req, Username of the camera whose topics are being gotten" << std::endl;
	std::cerr << "	-p: Optional, Password of the camera whose topics are being gotten, requiered on most cameras but not all" << std::endl;
	std::cerr << "	-ip: Req, IP address of the camera whose topics are being gotten" << std::endl;
	std::cerr << "	-f: Optional, Filepath to the json file (Whether it exists or needs to be created) where the data will be added. If no file is given, the data will be sent to the command line" << std::endl;
	std::cerr << "	-o: Testing, Place at the end of the input parameters uses the built in order of [Utility.exe username password ip /filepath/]" << std::endl;
	std::cerr << "	-port: Optional, allows the user to specify a non-standard port number" << std::endl;
	std::cerr << "" << std::endl;
	std::cerr << "Usage:" << std::endl;
	std::cerr << "----------" << std::endl;
	std::cerr << "	The purpose of this utility is to get the event topics from an Onvif camera. In order to do this the camera needs to be connected onto the same network as the computer the Utility is being run on and the user needs access to the username and ip address of the camera." << std::endl;
	std::cerr << "" << std::endl;

	return;
}
