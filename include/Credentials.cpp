#define MAX_URL_LENGTH 128
#include "Credentials.hpp"
#include "soapDeviceBindingProxy.h"
#include "soapStub.h"
#include <fstream>
#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>
#include <string>
#include "split.h"

//TODO
//Check if username and password are valid with the supplied ip address
Credentials::Credentials(int argc, char* argv[]) {

	username = NULL;
	password = NULL;
	ip = NULL;
	filepath = NULL;

	
	for (size_t i = 1; i < argc; ++i) {
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
		else if (strcmp(argv[i], "-v") == 0) {
			verbose = true;
		}
	}
}

int Credentials::ValidateCreds()
{
	//std::cout << username << std::endl;
	//std::cout << password << std::endl;
	//std::cout << ip << std::endl;

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
	strcat_s(xAddr, MAX_URL_LENGTH, "/onvif/device_service");

	url = xAddr;
	
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
	std::cerr << "" << std::endl;
	std::cerr << "Usage:" << std::endl;
	std::cerr << "----------" << std::endl;
	std::cerr << "	The purpose of this utility is to get the event topics from an Onvif camera. In order to do this the camera needs to be connected onto the same network as the computer the Utility is being run on and the user needs access to the username and ip address of the camera." << std::endl;
	std::cerr << "" << std::endl;

	return;
}
