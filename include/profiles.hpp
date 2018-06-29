#pragma once
#include <string>
#include "soapMedia2BindingProxy.h"

class Profiles {

public:
	Profiles(std::string user, std::string pass, std::string url);
public:
	int GetProfiles();
private:
	std::string m_username;
	std::string m_password;
	std::string m_url;
	Media2BindingProxy media;
	_ns8__GetProfilesResponse GPresp;
};