#include "event.hpp"

Event::Event() {}

void Event::setCredentials(std::string user, std::string pass, std::string url)
{
	m_username = user;
	m_password = pass;
	m_url = url;
	PullPoint.soap_endpoint = m_url.c_str();
}

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
	PullPoint.GetEventProperties(&GEV, GEVresp);

	return SOAP_OK;
}

EventData Event::ReturnEventData() {
	EventData data;
	data.resp = GEVresp;
	return data;
}