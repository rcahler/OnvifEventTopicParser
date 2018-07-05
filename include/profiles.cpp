#include "profiles.hpp"
#include "wsseapi.h"

Profiles::Profiles() {}

int Profiles::GetProfiles() {
	_trt__GetProfiles GP;

	soap_wsse_add_Security(&media);
	soap_wsse_add_UsernameTokenDigest(&media, "Id", m_username.c_str(), m_password.c_str());

	int i = media.GetProfiles(&GP, GPresp);

	if (i) {
		return i;
	}

	profiles = GPresp.Profiles;

	_trt__GetProfile GP1;
	GP1.ProfileToken = profiles[0]->token;
	media.GetProfile(&GP1, GPresp1);
}

void Profiles::SetParameters(std::string user, std::string pass, std::string url)
{
	m_username = user;
	m_password = pass;
	m_url = url;
	media.soap_endpoint = m_url.c_str();
	media.userid = m_username.c_str();
	media.passwd = m_password.c_str();
}

void Profiles::VideoSource()
{
	std::cout << GPresp1.Profile->VideoSourceConfiguration->Name << std::endl;
}
