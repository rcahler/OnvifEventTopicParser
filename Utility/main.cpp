#include <stdlib.h>
#include <stdbool.h>
#include "soapStub.h"
#include "ValidateCred.hpp"
#include "GetData.hpp"
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
		std::cerr << "Fail GetRelayOutputs DeviceIO" << std::endl;
		if (data.device.GetRelayOutputs() != SOAP_OK) {
			std::cerr << "Fail GetRelayOutputs Device" << std::endl;
		}
	}
	if (deviceIO.GetDigitalInputs() != SOAP_OK) {
		std::cerr << "Fail GetDigitalInputs" << std::endl;
	}
	
	data.AddDeviceIO(deviceIO.digital_inputs, deviceIO.relay_outputs);

	data.DataToJson();
	std::string name = data.returnManu();

	if (creds.fileBoo) {//Opens filestream with an existing file or creates new file
		SaveToFile(creds.fString, name, data.returnRoot());
	}
	else {
		std::cout << data.returnStream().str();
	}
}


