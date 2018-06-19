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
#include "soapRuleEngineBindingProxy.h"
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
				//std::cout << "Name: " << profiles[i]->Name << std::endl;

				if (profiles[i]->AudioEncoderConfiguration) {
					profiles[i]->AudioEncoderConfiguration->token;
				}

				if (profiles[i]->AudioSourceConfiguration) {
					profiles[i]->AudioSourceConfiguration->SourceToken;
					profiles[i]->AudioSourceConfiguration->token;
				}

				if (profiles[i]->Extension) {
					if (profiles[i]->Extension->AudioDecoderConfiguration) {
						profiles[i]->Extension->AudioDecoderConfiguration->token;
					}
					if (profiles[i]->Extension->AudioOutputConfiguration) {
						profiles[i]->Extension->AudioOutputConfiguration->token;
					}
				}

				if (profiles[i]->MetadataConfiguration) {
					if (profiles[i]->MetadataConfiguration->AnalyticsEngineConfiguration) {
						tt__MetadataConfiguration *metadata = profiles[i]->MetadataConfiguration;
						std::vector<tt__Config*> metaVec = metadata->AnalyticsEngineConfiguration->AnalyticsModule;
						for (int i = 0; i < metaVec.size(); i++) {
							//cout << "Meta: " << metaVec[i]->Name << endl;
						}
					}
				}

				if (profiles[i]->PTZConfiguration) {
					//profiles[i]->PTZConfiguration->token;
					//profiles[i]->PTZConfiguration->NodeToken;
				}

				//This appears to be a portion of what I need
				if (profiles[i]->VideoAnalyticsConfiguration) {
					RuleEngineBindingProxy analytics;
					analytics.soap_endpoint = device.anXaddr.c_str();

					if (profiles[i]->VideoAnalyticsConfiguration->AnalyticsEngineConfiguration) {
						_tan__GetSupportedAnalyticsModules GSAM;
						GSAM.ConfigurationToken = profiles[i]->VideoAnalyticsConfiguration->token;
						_tan__GetSupportedAnalyticsModulesResponse GSAMresp;

						soap_wsse_add_Security(&analytics);
						soap_wsse_add_UsernameTokenDigest(&analytics, "Id", username.c_str(), password.c_str());
						analytics.GetSupportedAnalyticsModules(&GSAM, GSAMresp);
					}
					
					if (profiles[i]->VideoAnalyticsConfiguration->RuleEngineConfiguration) {
						_tan__GetSupportedRules GSR;
						GSR.ConfigurationToken = profiles[i]->VideoAnalyticsConfiguration->token;
						_tan__GetSupportedRulesResponse GSRresp;

						soap_wsse_add_Security(&analytics);
						soap_wsse_add_UsernameTokenDigest(&analytics, "Id", username.c_str(), password.c_str());
						analytics.GetSupportedRules(&GSR, GSRresp);

						std::vector<tt__ConfigDescription*> descriptions = GSRresp.SupportedRules->RuleDescription;
						for (int j = 0; j < descriptions.size(); j++) {
							cout << "Rule Description names: " << descriptions[j]->Name << endl;
							std::vector < _tt__ConfigDescription_Messages> mesgs = descriptions[j]->Messages;
							for (int k = 0; k < mesgs.size(); k++) {
								if (mesgs[k].Source) {
									std::vector<_tt__ItemListDescription_SimpleItemDescription> source = mesgs[k].Source->SimpleItemDescription;
									cout << "source: " << source.size() << endl;
								}
								if (mesgs[k].Key) {
									std::vector<_tt__ItemListDescription_SimpleItemDescription> key = mesgs[k].Key->SimpleItemDescription;
									cout << "key: " << key.size() << endl;
								}
								if (mesgs[k].Data) {
									std::vector<_tt__ItemListDescription_SimpleItemDescription> data = mesgs[k].Data->SimpleItemDescription;
									cout << "data: " << data.size() << endl;
								}
							}
						}
					}
					
					/*std::vector<tt__Config*> aecVec = profiles[i]->VideoAnalyticsConfiguration->AnalyticsEngineConfiguration->AnalyticsModule;
					for (int j = 0; j < aecVec.size(); j++) {
						cout << "Analytics Engine: " << aecVec[j]->Name << endl;
					}

					std::vector<tt__Config*> ruleVec = profiles[i]->VideoAnalyticsConfiguration->RuleEngineConfiguration->Rule;
					for (int k = 0; k < ruleVec.size(); k++) {
						cout << "Rule Engine: " << ruleVec[k]->Name << endl;
					}*/
				}

				if (profiles[i]->VideoEncoderConfiguration){
					//profiles[i]->VideoEncoderConfiguration->token;
				}

				if (profiles[i]->VideoSourceConfiguration){
					//profiles[i]->VideoSourceConfiguration->SourceToken;
					//profiles[i]->VideoSourceConfiguration->token;
				}
			}
		}
	}
	else {
		std::cerr << "This camera does not support profiles" << std::endl;
	}
	PumpDataJson(ValidateCredentials(argc, argv), deviceData, eventData);
	return 0;
}



