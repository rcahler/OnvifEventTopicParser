#pragma once
#include <string>
#include "soapMedia2BindingProxy.h"

class Profiles {

public:
	Profiles();
public:
	int GetProfiles();
	void SetParameters(std::string user, std::string pass, std::string url);
public:
	_ns8__GetProfilesResponse GPresp;
private:
	std::string m_username;
	std::string m_password;
	std::string m_url;
	Media2BindingProxy media;
};