#include <stdlib.h>
#include <stdbool.h>
#include "soapStub.h"
#include "Credentials.hpp"
#include "GetData.hpp"
#include "SaveToFile.hpp"
#include "device.hpp"
#include "DeviceIO.hpp"
#include "logging.h"
#include <cstdio>
#include "ParseDigitalInputsXML.hpp"

int main(int argc, char* argv[]) {

	Credentials creds(argc, argv);

	if (creds.ValidateCreds()) {
		printCredErrors(creds.ValidateCreds());
		return 0;
	}

	std::string username = creds.username;
	std::string password = creds.password;
	std::string url = std::string(creds.url);

	bool verbose = creds.verbose;

	GetData data(username, password, url, verbose);

	DeviceIO deviceIO;
	deviceIO.SetParameters(username, password, data.io_url);


	std::vector<tt__RelayOutput*> relay_outputs;

	//Device IO calls are being made in Main() because the procedurally generated destructors made by Gsoap are not working
	//Ties calls to runtime of the program instead
	int deviceio_relay_result = deviceIO.GetRelayOutputs();
	if (deviceio_relay_result == SOAP_OK) {	
		if (deviceIO.relay_outputs.size() > 0) {
			relay_outputs = deviceIO.relay_outputs;
		}
	}

	int device_relay_result = data.device.GetRelayOutputs();
	if (device_relay_result == SOAP_OK) {
		if (data.device.GROresp.RelayOutputs.size() > 0) {
			relay_outputs = data.device.GROresp.RelayOutputs;
		}
	}
	
	int digital_inputs_result = deviceIO.GetDigitalInputs();
	if (verbose) {
		std::cerr << "GetDigitalInputs error code " << digital_inputs_result << std::endl;
	}
	std::vector<std::string> DigitalInputs;
	
	if (digital_inputs_result == 200) {//Some cameras use a non-standard namespace which cannot be parsed by the procedurally generated gsoap functions, this is that return

		//Uses gsoaps logging features to write the xml of the entire call to a temp file
		FILE* tFile;
		errno_t file_error = tmpfile_s(&tFile);
		if ((file_error) && (verbose)) {
			std::cerr << file_error << std::endl;
		}
		
		if (soap_register_plugin(deviceIO.GDIresp.soap, logging)) {
			soap_print_fault(deviceIO.GDIresp.soap, stderr); // failed to register
		}
			
		soap_set_logging_inbound(deviceIO.GDIresp.soap, tFile);
		
		deviceIO.GetDigitalInputs();

		soap_set_logging_inbound(deviceIO.GDIresp.soap, NULL); // disable logging
		
		char *contents;
		long file_size;
		
		fseek(tFile, 0, SEEK_END);
		file_size = ftell(tFile);
		rewind(tFile);
		contents = (char*)malloc((file_size + 1) * (sizeof(char)));
		fread(contents, sizeof(char), file_size, tFile);
		contents[file_size] = 0;
		fclose(tFile);

		DigitalInputs = ParseDigitalInputsXML(contents);
		free(contents);
	}
	else if (digital_inputs_result == SOAP_OK) {
		data.AddDeviceIO(deviceIO.digital_inputs_soap, relay_outputs);
	}
	else {//Non standard error on this call
		if (verbose) {
			std::cerr << "This is a non-standard GetDigitalInputsResponse" << std::endl;
		}
	}

	if (DigitalInputs.size()) {
		data.AddDeviceIO(DigitalInputs, relay_outputs);
	}

	data.DataToJson();
	std::string name = data.returnManu();
	
	if ((creds.filepath)&&(data.connected)) {//Opens filestream with an existing file or creates new file
		SaveToFile(creds.filepath, name, data.returnRoot());
	}
	else if (data.connected) {
		std::cout << data.returnStream().str() << std::endl;
	}
}