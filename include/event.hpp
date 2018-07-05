#include <string>
#include "soapStub.h"
#include "soapPullPointSubscriptionBindingProxy.h"

#ifndef _Event_
#define _Event_

class Event {

public: //Constructor
	Event();
public: //Functions
	int GetEventProperties();
	void SetParameters(std::string user, std::string pass, std::string url);
private: //Variables
	std::string m_username;
	std::string m_password;
	std::string m_url;
	PullPointSubscriptionBindingProxy PullPoint;
	_tev__GetEventPropertiesResponse GEVresp;
public: //Variables to be accessed from outside
	std::vector<soap_dom_element> topics;
};

#endif
