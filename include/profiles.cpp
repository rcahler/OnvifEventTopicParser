#include "profiles.hpp"

Profiles::Profiles() {}

int Profiles::GetProfiles() {
	_ns8__GetProfiles GP;

	return media.GetProfiles(&GP, GPresp);
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
