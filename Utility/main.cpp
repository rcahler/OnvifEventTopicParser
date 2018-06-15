#include "stdafx.h"
#include <stdlib.h>
#include <stdbool.h>

#include <fstream>

#include "soapStub.h"
#include "wsseapi.h"
#include "main.h"
#include "device.hpp"
#include "event.hpp"
#include "soapMediaBindingProxy.h"
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

	//Attempted to pass cam as a parameter but got error C2280: 'Camera::Camera(const Camera &)': attempting to reference a deleted function //Something to do with lacking a constructor maybe
	//Figure out this error at a later date, more important to get everything working first and foremost

	MediaBindingProxy mediaProxy;
	mediaProxy.soap_endpoint = url.c_str();

	soap_wsse_add_Security(&mediaProxy);
	soap_wsse_add_UsernameTokenDigest(&mediaProxy, "Id", username.c_str(), password.c_str());

	_trt__GetProfiles GP;
	_trt__GetProfilesResponse GPresp;
	if (mediaProxy.GetProfiles(&GP, GPresp) == SOAP_OK) {

		vector<tt__Profile*> profiles = GPresp.Profiles;
		if (profiles.size() == 0) {
			_trt__CreateProfile trt__CreateProfile;
			trt__CreateProfile.Name = "TEST";
			_trt__CreateProfileResponse trt__CreateProfileResponse;

			mediaProxy.CreateProfile(&trt__CreateProfile, trt__CreateProfileResponse);
			profiles.push_back(trt__CreateProfileResponse.Profile);
		}
		
		for (int i = 0; i < profiles.size(); i++) {
			//Cleans out previous test profiles made by the software
			if (profiles[i]->Name == "TEST") {
				_trt__DeleteProfile del;
				del.ProfileToken = profiles[i]->token;
				_trt__DeleteProfileResponse delResp;

				soap_wsse_add_Security(&mediaProxy);
				soap_wsse_add_UsernameTokenDigest(&mediaProxy, "Id", username.c_str(), password.c_str());

				mediaProxy.DeleteProfile(&del, delResp);
			}
			else {
				cout << profiles[i]->Name << endl;
				if (profiles[i]->AudioEncoderConfiguration){
					cout << "AudioEncoderConfiguration: " << profiles[i]->AudioEncoderConfiguration->token << endl;
				}
				if (profiles[i]->AudioSourceConfiguration){
					cout << "AudioSourceConfiguration: " << profiles[i]->AudioSourceConfiguration->Name << endl;
				}
				if (profiles[i]->Extension){
					if (profiles[i]->Extension->AudioDecoderConfiguration) {
						cout << "Extension->AudioDecoderConfiguration: " << profiles[i]->Extension->AudioDecoderConfiguration->Name << endl;
					}
					if (profiles[i]->Extension->AudioOutputConfiguration) {
						cout << "Extension->AudioOutputConfiguration: " << profiles[i]->Extension->AudioOutputConfiguration->Name << endl;
					}
				}
				if (profiles[i]->MetadataConfiguration){
					cout << "MetadataConfiguration: " << profiles[i]->MetadataConfiguration->Name << endl;
				}
				if (profiles[i]->PTZConfiguration){
					cout << "PTZConfiguration: " << profiles[i]->PTZConfiguration->Name << endl;
				}
				if (profiles[i]->VideoAnalyticsConfiguration){
					cout << "VideoAnalyticsConfiguration: " << profiles[i]->VideoAnalyticsConfiguration->Name << endl;
				}
				if (profiles[i]->VideoEncoderConfiguration){
					cout << "VideoEncoderConfiguration: " << profiles[i]->VideoEncoderConfiguration->Name << endl;
				}
				if (profiles[i]->VideoSourceConfiguration){
					cout << "VideoSourceConfiguration: " << profiles[i]->VideoSourceConfiguration->Name << endl;
				}
				cout << "" << endl;
			}
		}
	}
	else {
		cerr << "This camera does not support profiles" << endl;
	}

	//PumpDataJson(ValidateCredentials(argc, argv), deviceData, eventData);
	return 0;
}




//
//soap_wsse_add_Security(&mediaProxy);
//soap_wsse_add_UsernameTokenDigest(&mediaProxy, "Id", username.c_str(), password.c_str());
//mediaProxy.CreateProfile(&trt__CreateProfile, trt__CreateProfileResponse);
//
//_trt__GetAudioOutputConfigurationOptions GAOCO;
//_trt__GetAudioOutputConfigurationOptionsResponse GAOCOresp;
//mediaProxy.GetAudioOutputConfigurationOptions(&GAOCO, GAOCOresp);
//
//_trt__GetCompatibleVideoSourceConfigurations GCVSC;
//GCVSC.ProfileToken = trt__CreateProfileResponse.Profile->token;
//_trt__GetCompatibleVideoSourceConfigurationsResponse GCVSCresp;
////ISSUE HERE
//mediaProxy.GetCompatibleVideoSourceConfigurations(&GCVSC, GCVSCresp);
//
//cout << "HERE" << endl;
//_trt__GetMetadataConfigurations GMC;
//_trt__GetMetadataConfigurationsResponse GMCresp;
//mediaProxy.GetMetadataConfigurations(&GMC, GMCresp);
//
//
//soap_wsse_add_Security(&mediaProxy);
//soap_wsse_add_UsernameTokenDigest(&mediaProxy, "Id", username.c_str(), password.c_str());
//
//_trt__GetVideoAnalyticsConfigurations GVAC;
//_trt__GetVideoAnalyticsConfigurationsResponse GVACresp;
//
//
//int i = mediaProxy.GetVideoAnalyticsConfigurations(&GVAC, GVACresp);
//if (i == 0) {
//	if (GVACresp.Configurations.size() == 0) {
//		cerr << "ISSUE" << endl;
//	}
//}
//else {
//	if (GVACresp.Configurations.size() > 0) {
//		cerr << "ISSUE" << endl;
//	}
//}
//cout << "HERE" << endl;



