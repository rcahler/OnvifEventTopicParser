#pragma once
#include <string>
#include "soapMediaBindingProxy.h"

class Profiles {
public:
	Profiles();
public:
	void SetParameters(std::string user, std::string pass, std::string url);
	std::string VideoSource();
	std::string VideoAnalytics();
private:
	int GetProfiles();
	std::string m_username;
	std::string m_password;
	std::string m_url;
	bool GottenProfiles;
	MediaBindingProxy media;
	std::vector<tt__Profile*> profiles;
public:
	_trt__GetProfilesResponse GPSresp; //GetProfiles
	_trt__GetVideoAnalyticsConfigurationsResponse GVACSresp;
	_trt__GetAudioOutputsResponse GAOresp;

};