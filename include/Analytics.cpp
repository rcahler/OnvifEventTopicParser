#include "Analytics.hpp"

Analytics::Analytics()
{
}

void Analytics::SetParameters(std::string user, std::string pass, std::string url)
{
	m_username = user;
	m_password = pass;
	m_url = url;

	RuleEngineBindingProxy.soap_endpoint = m_url.c_str();
	AnalyticsDeviceBindingProxy.soap_endpoint = m_url.c_str();


	//RuleEngineBindingProxy.GetSupportedAnalyticsModules();
	//RuleEngineBindingProxy.GetRules();
	AnalyticsDeviceBindingProxy;
}
