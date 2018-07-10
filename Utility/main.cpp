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


	std::vector<tt__RelayOutput*> relay_outputs;
	if (deviceIO.GetRelayOutputs() == SOAP_OK) {
		if (deviceIO.relay_outputs.size() > 0) {
			relay_outputs = deviceIO.relay_outputs;
		}
	}
	if (data.device.GetRelayOutputs() == SOAP_OK) {
		if (data.device.GROresp.RelayOutputs.size() > 0) {
			relay_outputs = data.device.GROresp.RelayOutputs;
		}
	}

	
	int result = deviceIO.GetDigitalInputs();
	if (result == 200) {
		
		FILE* tFile;
		tFile = tmpfile();
		
		size_t bytes_in;
		size_t bytes_out;

		
		if (soap_register_plugin(deviceIO.GDIresp.soap, logging)) {
			soap_print_fault(deviceIO.GDIresp.soap, stderr); // failed to register
		}
			
		soap_set_logging_inbound(deviceIO.GDIresp.soap, tFile);
		
		deviceIO.GetDigitalInputs();

		soap_set_logging_inbound(deviceIO.GDIresp.soap, NULL); // disable logging
		soap_get_logging_stats(deviceIO.GDIresp.soap, &bytes_out, &bytes_in);
		
		char *contents;
		long file_size;
		
		fseek(tFile, 0, SEEK_END);
		file_size = ftell(tFile);
		rewind(tFile);
		contents = (char*)malloc((file_size + 1) * (sizeof(char)));
		fread(contents, sizeof(char), file_size, tFile);
		contents[file_size] = 0;
		fclose(tFile);

		//Need to parse contents
		std::cout << contents << std::endl;

		//std::cout << bytes_in << std::endl;
		//std::cout << bytes_out << std::endl;
	}
	else if (result != SOAP_OK) {
		std::cerr << "Fail GetDigitalInputs: " << deviceIO.GetDigitalInputs() << std::endl;
	}
	else {
		std::cout << "Sucsess DigitalInputs\n";
	}

	data.AddDeviceIO(deviceIO.digital_inputs, relay_outputs);

	data.DataToJson();
	std::string name = data.returnManu();

	if (creds.fileBoo) {//Opens filestream with an existing file or creates new file
		SaveToFile(creds.fString, name, data.returnRoot());
	}
	else {
		std::cout << data.returnStream().str();
	}
}


