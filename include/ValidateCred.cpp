#define MAX_URL_LENGTH 128
#include "ValidateCred.hpp"
#include "soapDeviceBindingProxy.h"
#include "soapStub.h"
#include <sstream>
#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>
#include <string>
#include "split.h"

using namespace std;

//TODO
//Check if username and password are valid with the supplied ip address
Camera ValidateCredentials(int argc, char* argv[]) {
	
	Camera cam;

	//Checks number of input parameters
	if (argc == 4) {
		cam.path.boo = false;
		cam.path.fString = "No file path has been chosen, all of the cameras details will be printed to the command line.";
	}
	else if (argc == 5) {
		cam.path.boo = true;
		cam.path.fString = argv[4];
	}
	else {
		cam.error.boo = true;
		cam.error.eString = "Wrong number of input parameters\nUsage: username password camera ip address [File path of outputfile]";
		return cam;
	}

	
	string ipAddr(argv[3]);

	//Checks ipAddress
	vector<string> ipVec = split(ipAddr, '.');
	if (ipVec.size() != 4) {
		cam.error.boo = true;
		cam.error.eString = "Malformed IP address";
		return cam;
	}
	for (int i = 0; i < 4; i++) {
		if (ipVec[i].find_first_not_of("[0123456789]") != string::npos) {
			cam.error.boo = true;
			cam.error.eString = "Malformed IP address";
			return cam;
		}
		int j = stoi(ipVec[i]);
		if (j > 255 || j < 0) {
			cam.error.boo = true;
			cam.error.eString = "Malformed IP address";
			return cam;
		}
	}

	//Opens filestream with an existing file or creates new file
	if (cam.path.boo) {
		cam.path.file.open(cam.path.fString, ios::out | ios::app);
	}

	char xAddr[MAX_URL_LENGTH] = { 0 };
	strcat_s(xAddr, MAX_URL_LENGTH, "http://");
	strcat_s(xAddr, MAX_URL_LENGTH, ipAddr.c_str());
	strcat_s(xAddr, MAX_URL_LENGTH, "/onvif/device_service");

	cam.url = xAddr;
	cam.username = argv[1];
	cam.password = argv[2];
	
	return cam;

}
