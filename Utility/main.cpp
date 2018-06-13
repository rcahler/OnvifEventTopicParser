#include "stdafx.h"
#include <stdlib.h>
#include <stdbool.h>

#include <fstream>

#include "soapStub.h"
#include "wsseapi.h"
#include "main.h"
#include "device.hpp"
#include "event.hpp"
#include "soapRuleEngineBindingProxy.h"
#include "ValidateCred.hpp"
#include "PumpDataToFile.hpp"
#include "PumpToJson.hpp"

using namespace std;

Device device;
Event eventDevice;

int main(int argc, char* argv[]) {

	Camera cam = ValidateCredentials(argc, argv);

	if (cam.error.boo == true) {
		cerr << cam.error.eString << endl;
		return 0;
	} else if (cam.path.boo == false) {
		cout << cam.path.fString << endl;
	}
	string username = cam.username;
	string password = cam.password;
	string url = cam.url;

	device.setCredentials(username, password, url);

	if (device.SyncCamTime() != 0) {
		cerr << "The camera and local system times could not be synched" << endl << "" << endl;
	}

	if (device.GetCapabilities() != 0) {
		cerr << "Device Capabilities could not be gotten, the camera could not be connected too" << endl;
		return 0;
	}

	device.GetDeviceInformation();
	device.GetRelayOutputs();
	device.GetDiscoveryMode();
	device.GetNetworkInterfaces();
	DeviceData deviceData = device.ReturnDeviceData();

	eventDevice.setCredentials(username, password, device.evXaddr);

	eventDevice.GetEventProperties();
	EventData eventData = eventDevice.ReturnEventData();

	//device.anXaddr;

	//Attempted to pass cam as a parameter but got error C2280: 'Camera::Camera(const Camera &)': attempting to reference a deleted function //Something to do with lacking a constructor maybe
	//Figure out this error at a later date, more important to get everything working first and foremost

	//PumpDataRaw(ValidateCredentials(argc, argv), deviceData, eventData);
	PumpDataJson(ValidateCredentials(argc, argv), deviceData, eventData);

	cout << "" << endl;

	return 0;
}



