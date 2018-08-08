#include "analytics.hpp"

#include "wsseapi.h"

Analytics::Analytics(){}

void Analytics::SetParameters(std::string user, std::string pass, std::string url)
{
	m_username = user;
	m_password = pass;
	m_url = url;

	RuleEngineBindingProxy.soap_endpoint = m_url.c_str();
	AnalyticsDeviceBindingProxy.soap_endpoint = m_url.c_str();
}

void Analytics::SetVideoAnalytic(std::string s)
{
	anToken = s;
}

int Analytics::GetSupportedAnalyticsModules()
{
	soap_wsse_add_Security(&RuleEngineBindingProxy);
	soap_wsse_add_UsernameTokenDigest(&RuleEngineBindingProxy, "Id", m_username.c_str(), m_password.c_str());

	_tan__GetSupportedAnalyticsModules GSAM;
	GSAM.ConfigurationToken = anToken;
	return RuleEngineBindingProxy.GetSupportedAnalyticsModules(&GSAM, GSAMresp);
}

int Analytics::GetSupportedRules()
{
	soap_wsse_add_Security(&RuleEngineBindingProxy);
	soap_wsse_add_UsernameTokenDigest(&RuleEngineBindingProxy, "Id", m_username.c_str(), m_password.c_str());

	_tan__GetRules GR;
	GR.ConfigurationToken = anToken;
	return RuleEngineBindingProxy.GetRules(&GR, GRresp);
}
