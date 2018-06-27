#define MAX_URL_LENGTH 128
#include "ValidateCred.hpp"
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
ValidateCredentials::ValidateCredentials(int argc, char* argv[]) {

	//Checks number of input parameters
	if (argc == 4) {
		fileBoo = false;
		eString = "No file path has been chosen, all of the cameras details will be printed to the command line.";
	}
	else if (argc == 5) {
		fileBoo = true;
		fString = argv[4];
	}
	else {
		errorBoo = true;
		eString = "Wrong number of input parameters\nUsage: username password camera ip address [File path of outputfile]";
		return;
	}

	
	std::string ipAddr(argv[3]);

	//Checks ipAddress
	std::vector<std::string> ipVec = split(ipAddr, '.');
	if (ipVec.size() != 4) {
		errorBoo = true;
		eString = "Malformed IP address";
		return;
	}
	for (int i = 0; i < 4; i++) {
		if (ipVec[i].find_first_not_of("[0123456789]") != std::string::npos) {
			errorBoo = true;
			eString = "Malformed IP address";
			return;
		}
		int j = stoi(ipVec[i]);
		if (j > 255 || j < 0) {
			errorBoo = true;
			eString = "Malformed IP address";
			return;
		}
	}

	char xAddr[MAX_URL_LENGTH] = { 0 };
	strcat_s(xAddr, MAX_URL_LENGTH, "http://");
	strcat_s(xAddr, MAX_URL_LENGTH, ipAddr.c_str());
	strcat_s(xAddr, MAX_URL_LENGTH, "/onvif/device_service");

	url = xAddr;
	username = argv[1];
	password = argv[2];

	return;
}
