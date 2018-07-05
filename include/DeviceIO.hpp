#pragma once
#include <string>
#include "soapDeviceIOBindingProxy.h"

class DeviceIO {
public:
	DeviceIO();
public:
	void SetParameters(std::string user, std::string pass, std::string url);
	int GetRelayOutputs();
	int GetDigitalInputs();
private:
	std::string m_username;
	std::string m_password;
	std::string m_url;
	DeviceIOBindingProxy deviceIO;
public:
	_tmd__GetDigitalInputsResponse GDIresp;
	_tds__GetRelayOutputsResponse GROresp;
};
