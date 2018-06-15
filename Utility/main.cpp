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

Device device;
Event eventDevice;

int main(int argc, char* argv[]) {

	Camera cam = ValidateCredentials(argc, argv);

	if (cam.error.boo == true) {
		std::cerr << cam.error.eString << std::endl;
		return 0;
	} else if (cam.path.boo == false) {
		std::cout << cam.path.fString << std::endl;
	}
	std::string username = cam.username;
	std::string password = cam.password;
	std::string url = cam.url;

	device.setCredentials(username, password, url);

	if (device.SyncCamTime() != 0) {
		std::cerr << "The camera and local system times could not be synched" << endl << "" << std::endl;
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
		
		std::cout << "size: " << profiles.size() << std::endl;
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
				std::cout << "Name: " << profiles[i]->Name << std::endl;
				if (profiles[i]->AudioEncoderConfiguration){
					std::cout << "AudioEncoderConfiguration: " << profiles[i]->AudioEncoderConfiguration->token << std::endl;
				}
				else {
					std::cout << "No AudioEncoderConfiguration" << std::endl;
				}

				if (profiles[i]->AudioSourceConfiguration){
					std::cout << "AudioSourceConfiguration: " << profiles[i]->AudioSourceConfiguration->Name << std::endl;
				}
				else {
					std::cout << "No AudioSourceConfiguration" << std::endl;
				}

				if (profiles[i]->Extension){
					if (profiles[i]->Extension->AudioDecoderConfiguration) {
						std::cout << "Extension->AudioDecoderConfiguration: " << profiles[i]->Extension->AudioDecoderConfiguration->Name << std::endl;
					}
					if (profiles[i]->Extension->AudioOutputConfiguration) {
						std::cout << "Extension->AudioOutputConfiguration: " << profiles[i]->Extension->AudioOutputConfiguration->Name << std::endl;
					}
				}
				else {
					std::cout << "No Extension" << std::endl;
				}

				if (profiles[i]->MetadataConfiguration) {
					std::cout << "MetadataConfiguration: " << profiles[i]->MetadataConfiguration->Name << std::endl;
					tt__MetadataConfiguration *metadata = profiles[i]->MetadataConfiguration;
				}
				else {
					std::cout << "No MetaDataConfiguration" << std::endl;
				}

				if (profiles[i]->PTZConfiguration){
					std::cout << "PTZConfiguration: " << profiles[i]->PTZConfiguration->Name << std::endl;
				}
				else {
					std::cout << "No PTZConfiguration" << std::endl;
				}

				if (profiles[i]->VideoAnalyticsConfiguration){
					std::cout << "VideoAnalyticsConfiguration: " << profiles[i]->VideoAnalyticsConfiguration->Name << std::endl;
				}
				else {
					std::cout << "No VideoAnalyticsConfiguration" << std::endl;
				}

				if (profiles[i]->VideoEncoderConfiguration){
					std::cout << "VideoEncoderConfiguration: " << profiles[i]->VideoEncoderConfiguration->Name << std::endl;
				}
				else {
					std::cout << "No VideoEncoderConfiguration" << std::endl;
				}

				if (profiles[i]->VideoSourceConfiguration){
					std::cout << "VideoSourceConfiguration: " << profiles[i]->VideoSourceConfiguration->Name << std::endl;
				}
				else {
					std::cout << "No VideoSourceConfiguration" << std::endl;
				}
				std::cout << "" << endl;
			}
		}
	}
	else {
		std::cerr << "This camera does not support profiles" << std::endl;
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



