#ifndef _Device_
#define _Device_

#include <string>
#include "soapDeviceBindingProxy.h"
#include "soapStub.h"

using namespace std;

struct DevInfData {
	_tds__GetDeviceInformationResponse response;
};

struct CapData {
	tt__Capabilities* caps;
};

struct RelayData {
	_tds__GetRelayOutputsResponse response;
};

struct DeviceData {
	DevInfData devInfo;
	CapData capData;
	RelayData relayData;
};

class Device {

public: //Constructor
	Device();
public: //Functions
	void setCredentials(string user, string pass, string url);
	int SyncCamTime();
	int GetDeviceInformation();
	int GetCapabilities();
	int GetRelayOutputs();
	int GetDiscoveryMode();
	int GetNetworkInterfaces();
	DeviceData ReturnDeviceData();
private: //Local functions
	int LocalAddUsernameTokenDigest(struct soap *soapOff, double cam_pc_offset);
	double findDiffTime(struct tm local_sys, tt__DateTime cameraTime, bool isDST);
private: //Classwide variables
	string m_username;
	string m_password;
	string m_url;
	DeviceBindingProxy deviceBindProxy;
	_tds__GetDeviceInformationResponse GDIresp;
	_tds__GetCapabilitiesResponse GCresp;
	_tds__GetRelayOutputsResponse GROresp;
	_tds__GetDiscoveryModeResponse GDMresp;
	_tds__GetNetworkInterfacesResponse GNIresp;
public://Xaddr
	string anXaddr;
	string deXaddr;
	string evXaddr;
	string imXaddr;
	string meXaddr;
	string ptzXaddr;
};

#endif // !"_Device_"

