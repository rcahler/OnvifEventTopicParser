#include "profiles.hpp"

Profiles::Profiles(std::string user, std::string pass, std::string url)
{
	m_username = user;
	m_password = pass;
	m_url = url;
	media.soap_endpoint = m_url.c_str();
	media.userid = m_username.c_str();
	media.passwd = m_password.c_str();
}

int Profiles::GetProfiles() {
	_ns8__GetProfiles GP;
	
	int i = media.GetProfiles(&GP, GPresp);
	if (!i) {
		std::cerr << "Profile call failed\n";
		return i;
	}

	std::cout << "Profile call sucess\n";
	return i;
}
