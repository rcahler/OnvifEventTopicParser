#ifndef _Device_
#define _Device_

#include <string>
#include "soapDeviceBindingProxy.h"

class Device {

public: //Constructor
	Device();
public: //Functions
	int SyncCamTime();
	int GetDeviceInformation();
	int GetCapabilities();
	int GetRelayOutputs();
	int GetServiceCapabilities();
	void SetParameters(std::string user, std::string pass, std::string url);
private: //Local functions
	int LocalAddUsernameTokenDigest(struct soap *soapOff, double cam_pc_offset);
	double findDiffTime(struct tm local_sys, tt__DateTime cameraTime, bool isDST);
private: //Classwide variables
	std::string m_username;
	std::string m_password;
	std::string m_url;
	DeviceBindingProxy deviceBindProxy;
public://Xaddr and resps
	std::string anXaddr;
	std::string deXaddr;
	std::string evXaddr;
	std::string imXaddr;
	std::string meXaddr;
	std::string ptzXaddr;
	std::string ioXaddr;
	_tds__GetDeviceInformationResponse GDIresp;
	_tds__GetCapabilitiesResponse GCresp;
	_tds__GetRelayOutputsResponse GROresp;
	_tds__GetServiceCapabilitiesResponse GSCresp;
public://Variables from the camera that need to be passed
	std::string Manufacturer;
	std::string FirmwareVersion;
	std::string HardwareId;
	std::string Model;
	std::string SerialNumber;
	int num_of_input_connectors;
};

#endif // !"_Device_"

