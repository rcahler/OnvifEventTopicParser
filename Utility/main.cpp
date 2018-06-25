#include "stdafx.h"
#include <stdlib.h>
#include <stdbool.h>
#include "soapStub.h"
#include "ValidateCred.hpp"
#include "GetData.hpp"


int main(int argc, char* argv[]) {

	cout << std::boolalpha;
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

	GetData(username, password, url);

	

	//Attempted to pass cam as a parameter but got error C2280: 'Camera::Camera(const Camera &)': attempting to reference a deleted function //Something to do with lacking a constructor maybe
	//Figure out this error at a later date, more important to get everything working first and foremost
	
	//PumpDataJson(ValidateCredentials(argc, argv), deviceData, eventData);
	return 0;
}

////Not sure if this is needed yet
//DeviceIOBindingProxy deviceIO;
//deviceIO.soap_endpoint = device.deXaddr.c_str();
//
//soap_wsse_add_Security(&deviceIO);
//soap_wsse_add_UsernameTokenDigest(&deviceIO, "Id", username.c_str(), password.c_str());
//
//_tds__GetRelayOutputs GRO;
//_tds__GetRelayOutputsResponse GROresp;
//deviceIO.GetRelayOutputs(&GRO, GROresp);
//
//soap_wsse_add_Security(&deviceIO);
//soap_wsse_add_UsernameTokenDigest(&deviceIO, "Id", username.c_str(), password.c_str());
//
//_tmd__GetDigitalInputs GDI;
//_tmd__GetDigitalInputsResponse GDIresp;
//deviceIO.GetDigitalInputs(&GDI, GDIresp);
//
//_tmd__GetServiceCapabilities GSC;
//_tmd__GetServiceCapabilitiesResponse GSCresp;
//if (deviceIO.GetServiceCapabilities(&GSC, GSCresp) == SOAP_OK) {
//	if (GSCresp.Capabilities->RelayOutputs) {
//		for (int i = 0; i < GROresp.RelayOutputs.size(); i++) {
//			cout << "Relay Token: " << GROresp.RelayOutputs[i]->token << endl;
//		}
//	}
//
//	if (GSCresp.Capabilities->DigitalInputs) {
//		for (int i = 0; i < GDIresp.DigitalInputs.size(); i++) {
//			cout << "Digital Token: " << GDIresp.DigitalInputs[i]->token << endl;
//		}
//	}
//}
//else {
//	cout << "Invalid Call" << endl;
//}

////I think I can get all of the profile info of of the event properties?
//
//MediaBindingProxy mediaProxy;
//mediaProxy.soap_endpoint = url.c_str();
//
//soap_wsse_add_Security(&mediaProxy);
//soap_wsse_add_UsernameTokenDigest(&mediaProxy, "Id", username.c_str(), password.c_str());
//_trt__GetProfiles GP;
//_trt__GetProfilesResponse GPresp;
//if (mediaProxy.GetProfiles(&GP, GPresp) == SOAP_OK) {
//
//	vector<tt__Profile*> profiles = GPresp.Profiles;
//
//	//Does it really need to go through every profile?
//	if (profiles.size() == 0) {
//		_trt__CreateProfile trt__CreateProfile;
//		trt__CreateProfile.Name = "TEST";
//		_trt__CreateProfileResponse trt__CreateProfileResponse;
//
//		mediaProxy.CreateProfile(&trt__CreateProfile, trt__CreateProfileResponse);
//		profiles.push_back(trt__CreateProfileResponse.Profile);
//	}
//
//	//for (int i = 0; i < profiles.size(); i++) {
//	int i = 0;
//	//Cleans out previous test profiles made by the software
//	if (profiles[i]->Name == "TEST") {
//		_trt__DeleteProfile del;
//		del.ProfileToken = profiles[i]->token;
//		_trt__DeleteProfileResponse delResp;
//
//		soap_wsse_add_Security(&mediaProxy);
//		soap_wsse_add_UsernameTokenDigest(&mediaProxy, "Id", username.c_str(), password.c_str());
//
//		mediaProxy.DeleteProfile(&del, delResp);
//	}
//	else {
//		//std::cout << "Name: " << profiles[i]->Name << std::endl;
//
//		if (profiles[i]->AudioEncoderConfiguration) {
//			profiles[i]->AudioEncoderConfiguration->token;
//		}
//
//		if (profiles[i]->AudioSourceConfiguration) {
//		}
//
//		if (profiles[i]->Extension) {
//			if (profiles[i]->Extension->AudioDecoderConfiguration) {
//				profiles[i]->Extension->AudioDecoderConfiguration->token;
//			}
//			if (profiles[i]->Extension->AudioOutputConfiguration) {
//				profiles[i]->Extension->AudioOutputConfiguration->token;
//			}
//		}
//
//		if (profiles[i]->MetadataConfiguration) {
//			if (profiles[i]->MetadataConfiguration->AnalyticsEngineConfiguration) {
//				tt__MetadataConfiguration *metadata = profiles[i]->MetadataConfiguration;
//				std::vector<tt__Config*> metaVec = metadata->AnalyticsEngineConfiguration->AnalyticsModule;
//				for (int i = 0; i < metaVec.size(); i++) {
//					//cout << "Meta: " << metaVec[i]->Name << endl;
//				}
//			}
//		}
//
//		if (profiles[i]->PTZConfiguration) {
//			//profiles[i]->PTZConfiguration->token;
//			//profiles[i]->PTZConfiguration->NodeToken;
//		}
//
//		//This appears to be a portion of what I need
//		if (profiles[i]->VideoAnalyticsConfiguration) {
//
//		}
//
//		if (profiles[i]->VideoEncoderConfiguration) {
//			//profiles[i]->VideoEncoderConfiguration->token;
//		}
//
//		if (profiles[i]->VideoSourceConfiguration) {
//		}
//	}
//}
//else {
//	std::cerr << "This camera does not support profiles" << std::endl;
//}



