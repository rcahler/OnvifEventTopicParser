#include "DeviceIO.hpp"
#include "wsseapi.h"

DeviceIO::DeviceIO(){}

void DeviceIO::SetParameters(std::string user, std::string pass, std::string url)
{
	m_username = user;
	m_password = pass;
	m_url = url;

	deviceIO.soap_endpoint = m_url.c_str();

	SOAP_OK;
}

int DeviceIO::GetRelayOutputs()
{
	soap_wsse_add_Security(&deviceIO);
	soap_wsse_add_UsernameTokenDigest(&deviceIO, "Id", m_username.c_str(), m_password.c_str());

	_tds__GetRelayOutputs GRO;
	int soap_call_result = deviceIO.GetRelayOutputs(&GRO, GROresp);

	relay_outputs = GROresp.RelayOutputs;

	return soap_call_result;
}

int DeviceIO::GetDigitalInputs()
{
	soap_wsse_add_Security(&deviceIO);
	soap_wsse_add_UsernameTokenDigest(&deviceIO, "Id", m_username.c_str(), m_password.c_str());

	_tmd__GetDigitalInputs GDI;
	int soap_call_result = deviceIO.GetDigitalInputs(&GDI, GDIresp);

	digital_inputs = GDIresp.DigitalInputs;

	return soap_call_result;
}
