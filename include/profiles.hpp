#pragma once
#include <string>
#include "soapMediaBindingProxy.h"

class Profiles {
public:
	Profiles();
public:
	int GetProfiles();
	void SetParameters(std::string user, std::string pass, std::string url);
	void VideoSource();
public:
	_trt__GetProfilesResponse GPresp; //GetProfiles
	_trt__GetProfileResponse GPresp1; //GetProfile
private:
	std::string m_username;
	std::string m_password;
	std::string m_url;
	MediaBindingProxy media;
	std::vector<tt__Profile*> profiles;
};