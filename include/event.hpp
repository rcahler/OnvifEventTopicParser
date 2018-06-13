#include <string>
#include "soapStub.h"
#include "soapPullPointSubscriptionBindingProxy.h"
#include "wsseapi.h"

#ifndef _Event_
#define _Event_

using namespace std;

struct EventData {
	_tev__GetEventPropertiesResponse resp;
};

class Event {

public: //Constructor
	Event();
public: //Functions
	void setCredentials(string user, string pass, string url);
	int GetEventProperties();
	EventData ReturnEventData();
private: //Variables
	string m_username;
	string m_password;
	string m_url;
	PullPointSubscriptionBindingProxy PullPoint;
	_tev__GetEventPropertiesResponse GEVresp;
};

#endif
