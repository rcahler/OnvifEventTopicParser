#pragma once

#include "soapAnalyticsDeviceBindingProxy.h"
#include "soapRuleEngineBindingProxy.h"

class Analytics {

public:
	Analytics();
	void SetParameters(std::string user, std::string pass, std::string url);
	int GetSupportedAnalyticsModules();
	int GetSupportedRules();
private: //Classwide variables
	std::string m_username;
	std::string m_password;
	std::string m_url;
	AnalyticsDeviceBindingProxy AnalyticsDeviceBindingProxy;
	RuleEngineBindingProxy RuleEngineBindingProxy;
};