#include "event.hpp"
#include "wsseapi.h"

Event::Event() {}
int Event::GetEventProperties()
{
	soap_wsse_add_Security(&PullPoint);
	soap_wsse_add_UsernameTokenDigest(&PullPoint, "Id", m_username.c_str(), m_password.c_str());

	_tev__CreatePullPointSubscription CPPS;
	_tev__CreatePullPointSubscriptionResponse CPPSresp;
	PullPoint.CreatePullPointSubscription(&CPPS, CPPSresp);

	soap_wsse_add_Security(&PullPoint);
	soap_wsse_add_UsernameTokenDigest(&PullPoint, "Id", m_username.c_str(), m_password.c_str());

	_tev__GetEventProperties GEV;
	int result = PullPoint.GetEventProperties(&GEV, GEVresp);

	if (result != SOAP_OK) {
		return result;
	}
	
	topics = GEVresp.wstop__TopicSet->__any;

	return result;
}

void Event::SetParameters(std::string user, std::string pass, std::string url)
{
	m_username = user;
	m_password = pass;
	m_url = url;
	PullPoint.soap_endpoint = m_url.c_str();
}
