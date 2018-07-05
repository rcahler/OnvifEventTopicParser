#include <stdlib.h>
#include <stdbool.h>
#include "soapStub.h"
#include "ValidateCred.hpp"
#include "GetData.hpp"
#include "main.h"
#include "SaveToFile.hpp"
#include "device.hpp"
#include "DeviceIO.hpp"
#include "logging.h"
#include <cstdio>

int main(int argc, char* argv[]) {

	ValidateCredentials creds(argc, argv);
	
	if (creds.errorBoo == true) {
		std::cerr << creds.eString << std::endl;
		return 0;
	} else if (creds.fileBoo == false) {
		std::cout << creds.fString << std::endl;
	}
	std::string username = creds.username;
	std::string password = creds.password;
	std::string url = creds.url;

	GetData data(username, password, url);
	DeviceIO deviceIO;

	
	deviceIO.SetParameters(username, password, data.io_url);
		
	if (deviceIO.GetRelayOutputs() != SOAP_OK) {
		std::cerr << "Fail GetRelayOutputs" << std::endl;
	}
	if (deviceIO.GetDigitalInputs() != SOAP_OK) {
		std::cerr << "Fail GetDigitalInputs" << std::endl;
	}

	deviceIO.GROresp.RelayOutputs.size();
	deviceIO.GDIresp.DigitalInputs.size();
	

	std::string name = data.returnManu();

	//Opens filestream with an existing file or creates new file
	if (creds.fileBoo) {
		SaveToFile(creds.fString, name, data.returnRoot());
	}
	else {
		std::cout << data.returnStream().str();
	}
}


