#include "profiles.hpp"
#include "wsseapi.h"

Profiles::Profiles() {}

int Profiles::GetProfiles() {
	_trt__GetProfiles GPS;
	soap_wsse_add_Security(&media);
	soap_wsse_add_UsernameTokenDigest(&media, "Id", m_username.c_str(), m_password.c_str());
	return media.GetProfiles(&GPS, GPSresp);
}

void Profiles::SetParameters(std::string user, std::string pass, std::string url)
{
	m_username = user;
	m_password = pass;
	m_url = url;
	media.soap_endpoint = m_url.c_str();
	GottenProfiles = false;
}

std::string Profiles::VideoSource()
{
	if (!GottenProfiles) {
		if (this->GetProfiles() != SOAP_OK) {
			std::cerr << "Profiles could not be gotten" << std::endl;
			return "FAIL";
		}
		GottenProfiles = true;
	}

	return "VideoSource";
	//return GPresp.Profile->VideoSourceConfiguration->token;
}

std::string Profiles::VideoAnalytics()
{
	if (!GottenProfiles) {
		if (this->GetProfiles() != SOAP_OK) {
			std::cerr << "Profiles could not be gotten" << std::endl;
			return "FAIL";
		}
		GottenProfiles = true;
	}

	std::vector<tt__Profile*> profiles = GPSresp.Profiles;

	for (size_t i = 0; i < profiles.size(); i++) {
		if (*profiles[i]->MetadataConfiguration->Analytics) {
			_trt__GetCompatibleVideoAnalyticsConfigurations GCVACS;
			GCVACS.ProfileToken = profiles[i]->token;
			_trt__GetCompatibleVideoAnalyticsConfigurationsResponse GCVACSresp;
			soap_wsse_add_Security(&media);
			soap_wsse_add_UsernameTokenDigest(&media, "Id", m_username.c_str(), m_password.c_str());

			if (media.GetCompatibleVideoAnalyticsConfigurations(&GCVACS, GCVACSresp) != SOAP_OK) {
				//std::cerr << "VideoAnalyticsConfigurations could not be gotten" << std::endl;
				std::cerr << "FAIL" << std::endl;
			}
			else {
				std::cout << "GetCompatibleVideoAnalyticsConfigurations suceed" << std::endl;
			}
		}
		else {
			std::cout << "Profile does not support analytics\n";
		}
	}

	

	return "VideoAnalytics";
	//return GPresp.Profile->VideoAnalyticsConfiguration->token;
}
