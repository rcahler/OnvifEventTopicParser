#include "PumpDataToFile.hpp"
#include <sstream>
#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>
#include <string>
#include "split.h"
#include "ParseEventProperties.h"

//Existed to figure out what all was needed

//MediaCaps
int MaximumNumberOfProfiles;
bool RTPMulticast;

//GetEventProperties
vector<string> TopicNamespaceLocation;
bool FixedTopicSet;
vector<soap_dom_element> dom;
vector<string> TopicExpressionDialect;
vector<string> MessageContentFilterDialect;
vector<string> ProducerPropertiesFilterDialect;
vector<string> MessageContentSchemaLocation;

void PumpDataRaw(Camera cam, DeviceData deviceData, EventData eventData) {

	stringstream stream;
	stream << boolalpha;

	//GetDeviceInformation
	string Manu = deviceData.devInfo.response.Manufacturer;
	string FirmwareVersion = deviceData.devInfo.response.FirmwareVersion;
	string HardwareId = deviceData.devInfo.response.HardwareId;
	string Model = deviceData.devInfo.response.Model;
	string SerialNumber = deviceData.devInfo.response.SerialNumber;

	stream << "Manufacturer: " + Manu << endl;
	stream << "FirmwareVersion: " + FirmwareVersion << endl;
	stream << "HardwareId: " + HardwareId << endl;
	stream << "Model: " + Model << endl;
	stream << "SerialNumber: " + SerialNumber << endl;

	//Getcapabilities
	//AnalyticsCaps
		if (deviceData.capData.caps->Analytics) {
			bool AnalyticsModuleSupport = deviceData.capData.caps->Analytics->AnalyticsModuleSupport;
			bool RuleSupport = deviceData.capData.caps->Analytics->RuleSupport;
			string XAddr = deviceData.capData.caps->Analytics->XAddr;
			stream << "AnalyticsModuleSupport: " << AnalyticsModuleSupport << endl;
			stream << "RuleSupport: " << RuleSupport << endl;
			stream << "XAddr: " << XAddr << endl;
		}
		
	//DeviceCaps
	//DeviceIO
		if (deviceData.capData.caps->Device->IO->Extension != NULL) {
			bool Auxilary = deviceData.capData.caps->Device->IO->Extension->Auxiliary; //Extension
			vector<string> AuxiliaryCommands = deviceData.capData.caps->Device->IO->Extension->AuxiliaryCommands; //Extension
			stream << "Auxilary: " << Auxilary << endl;
			stream << "AuxiliaryCommands:" << endl;
			for (int i = 0; i < AuxiliaryCommands.size(); i++) {
				stream << "\t" << AuxiliaryCommands[i] << endl;
			}
		}
		int* InputConnectors = deviceData.capData.caps->Device->IO->InputConnectors;
		int* RelayOutputs = deviceData.capData.caps->Device->IO->RelayOutputs;

		stream << "InputConnectors: " << *InputConnectors << endl;
		stream << "RelayOutputs: " << *RelayOutputs << endl;

	//Network
		bool DynDNS = deviceData.capData.caps->Device->Network->DynDNS;
		if (deviceData.capData.caps->Device->Network->Extension != NULL) {
			bool Dot11Configuration = deviceData.capData.caps->Device->Network->Extension->Dot11Configuration; //Extension
			stream << "Dot11Configuration: " << Dot11Configuration << endl;
		}
		bool IPFilter = deviceData.capData.caps->Device->Network->IPFilter;
		bool IPVersion6 = deviceData.capData.caps->Device->Network->IPVersion6;
		bool ZeroConfiguration = deviceData.capData.caps->Device->Network->ZeroConfiguration;
	
		stream << "DynDNS: " << DynDNS << endl;
		stream << "IPFilter: " << IPFilter << endl;
		stream << "IPVersion6: " << IPVersion6 << endl;
		stream << "ZeroConfiguration: " << ZeroConfiguration << endl;

	//Security
		bool AccessPolicyConfig = deviceData.capData.caps->Device->Security->AccessPolicyConfig;
		if (deviceData.capData.caps->Device->Security->Extension != NULL) {
			bool TLS1_x002e0 = deviceData.capData.caps->Device->Security->Extension->TLS1_x002e0; //Extension
			stream << "TLS1_x002e0: " << TLS1_x002e0 << endl;
		}
		bool KerberosToken = deviceData.capData.caps->Device->Security->KerberosToken;
		bool OnboardKeyGeneration = deviceData.capData.caps->Device->Security->OnboardKeyGeneration;
		bool RELToken = deviceData.capData.caps->Device->Security->RELToken;
		bool SAMLToken = deviceData.capData.caps->Device->Security->SAMLToken;
		bool TLS1_x002e1 = deviceData.capData.caps->Device->Security->TLS1_x002e1;
		bool TLS1_x002e2 = deviceData.capData.caps->Device->Security->TLS1_x002e2;
		bool X_x002e509Token = deviceData.capData.caps->Device->Security->X_x002e509Token;

		stream << "AccessPolicyConfig: " << AccessPolicyConfig << endl;

		stream << "KerberosToken: " << KerberosToken << endl;
		stream << "OnboardKeyGeneration: " << OnboardKeyGeneration << endl;
		stream << "RELToken: " << RELToken << endl;
		stream << "SAMLToken: " << SAMLToken << endl;
		stream << "TLS1_x002e1" << TLS1_x002e1 << endl;
		stream << "TLS1_x002e2: " << TLS1_x002e2 << endl;
		stream << "X_x002e509Token: " << X_x002e509Token << endl;

	//System
		bool DiscoveryBye = deviceData.capData.caps->Device->System->DiscoveryBye;
		bool DiscoveryResolve = deviceData.capData.caps->Device->System->DiscoveryResolve;
		if (deviceData.capData.caps->Device->System->Extension != NULL) {
			bool HttpFirmwareUpgrade = deviceData.capData.caps->Device->System->Extension->HttpFirmwareUpgrade; //Extension
			bool HttpSupportInformation = deviceData.capData.caps->Device->System->Extension->HttpSupportInformation; //Extension
			bool HttpSystemBackup = deviceData.capData.caps->Device->System->Extension->HttpSystemBackup; //Extension
			bool HttpSystemLogging = deviceData.capData.caps->Device->System->Extension->HttpSystemLogging; //Extension
			stream << "HttpFirmwareUpgrade: " << HttpFirmwareUpgrade << endl;
			stream << "HttpSupportInformation: " << HttpSupportInformation << endl;
			stream << "HttpSystemBackup: " << HttpSystemBackup << endl;
			stream << "HttpSystemLogging: " << HttpSystemLogging << endl;
		}
		bool FirmwareUpgrade = deviceData.capData.caps->Device->System->FirmwareUpgrade;
		bool RemoteDiscovery = deviceData.capData.caps->Device->System->RemoteDiscovery;
		vector<tt__OnvifVersion*> SupportedVersions = deviceData.capData.caps->Device->System->SupportedVersions;
		bool SystemBackup = deviceData.capData.caps->Device->System->SystemBackup;
		bool SystemLogging = deviceData.capData.caps->Device->System->SystemLogging;

		stream << "DiscoveryBye: " << DiscoveryBye << endl;
		stream << "DiscoveryResolve: " << DiscoveryResolve << endl;

		
		stream << "FirmwareUpgrade: " << FirmwareUpgrade << endl;
		stream << "RemoteDiscovery: " << RemoteDiscovery << endl;
		stream << "SupportedVersions: " << endl;
		for (int i = 0; i < SupportedVersions.size(); i++) {
			stream << "\t" << SupportedVersions[i]->Major << "." << SupportedVersions[i]->Minor << endl;
		}
		stream << "SystemBackup: " << SystemBackup << endl;
		stream << "SystemLogging: " << SystemLogging << endl;

	//EventCaps
		bool WSPausableSubscriptionManagerInterfaceSupport = deviceData.capData.caps->Events->WSPausableSubscriptionManagerInterfaceSupport;
		bool WSPullPointSupport = deviceData.capData.caps->Events->WSPullPointSupport;
		bool WSSubscriptionPolicySupport = deviceData.capData.caps->Events->WSSubscriptionPolicySupport;

		stream << "WSPausableSubscriptionManagerInterfaceSupport: " << WSPausableSubscriptionManagerInterfaceSupport << endl;
		stream << "WSPullPointSupport: " << WSPullPointSupport << endl;
		stream << "WSSubscriptionPolicySupport: " << WSSubscriptionPolicySupport << endl;

	//ExtensionCaps
		if (deviceData.capData.caps->Extension != NULL) {
			
			tt__AnalyticsDeviceCapabilities* AnalyticsDeviceCapabilities = deviceData.capData.caps->Extension->AnalyticsDevice; //Extension
			if (AnalyticsDeviceCapabilities != NULL) {
				soap_dom_attribute dom = AnalyticsDeviceCapabilities->__anyAttribute;
				for (soap_dom_attribute::iterator itr = dom.att_begin(); itr != dom.att_end(); ++itr) {
					stream << itr.name << ": " << itr.nstr << endl;
				}
			}

			if (deviceData.capData.caps->Extension->DeviceIO != NULL) {
				bool AudioOutputs = deviceData.capData.caps->Extension->DeviceIO->AudioOutputs; //Extension
				bool AudioSources = deviceData.capData.caps->Extension->DeviceIO->AudioSources; //Extension
				bool VideoOutputs = deviceData.capData.caps->Extension->DeviceIO->VideoOutputs; //Extension
				bool VideoSources = deviceData.capData.caps->Extension->DeviceIO->VideoSources; //Extension
				stream << "AudioOutputs: " << AudioOutputs << endl;
				stream << "AudioSources: " << AudioSources << endl;
				stream << "VideoOutputs: " << VideoOutputs << endl;
				stream << "VideoSources: " << VideoSources << endl;
			}

			if (deviceData.capData.caps->Extension->Display != NULL) {
				bool FixedLayout = deviceData.capData.caps->Extension->Display->FixedLayout; //Extension
				stream << "FixedLayout: " << FixedLayout << endl;
			}

			if (deviceData.capData.caps->Extension->Receiver != NULL) {
				int MaximumRTSPURILength = deviceData.capData.caps->Extension->Receiver->MaximumRTSPURILength; //Extension
				bool USCOREMulticast = deviceData.capData.caps->Extension->Receiver->RTP_USCOREMulticast; //Extension
				bool RTP_USCORERTSP_USCORETCP = deviceData.capData.caps->Extension->Receiver->RTP_USCORERTSP_USCORETCP; //Extension
				bool RTP_USCORETCP = deviceData.capData.caps->Extension->Receiver->RTP_USCORETCP; //Extension
				bool SupportedReceivers = deviceData.capData.caps->Extension->Receiver->SupportedReceivers; //Extension
				stream << "MaximumRTSPURILength: " << MaximumRTSPURILength << endl;
				stream << "USCOREMulticast: " << USCOREMulticast << endl;
				stream << "RTP_USCORERTSP_USCORETCP: " << RTP_USCORERTSP_USCORETCP << endl;
				stream << "RTP_USCORETCP: " << RTP_USCORETCP << endl;
				stream << "SupportedReceivers: " << SupportedReceivers << endl;
			}

			if (deviceData.capData.caps->Extension->Recording != NULL) {
				bool DynamicRecordings = deviceData.capData.caps->Extension->Recording->DynamicRecordings; //Extension
				bool DynamicTracks = deviceData.capData.caps->Extension->Recording->DynamicTracks; //Extension
				bool MaxStringLength = deviceData.capData.caps->Extension->Recording->MaxStringLength; //Extension
				bool MediaProfileSource = deviceData.capData.caps->Extension->Recording->MediaProfileSource; //Extension
				bool ReceiverSource = deviceData.capData.caps->Extension->Recording->ReceiverSource; //Extension
				stream << "DynamicRecordings: " << DynamicRecordings << endl;
				stream << "DynamicTracks: " << DynamicTracks << endl;
				stream << "MaxStringLength: " << MaxStringLength << endl;
				stream << "MediaProfileSource: " << MediaProfileSource << endl;
				stream << "ReceiverSource: " << ReceiverSource << endl;
			}

			if (deviceData.capData.caps->Extension->Search != NULL) {
				bool MetadataSearch = deviceData.capData.caps->Extension->Search->MetadataSearch; //Extension
				stream << "MetadataSearch: " << MetadataSearch << endl;
			}
		}

	//MediaCaps
		if (deviceData.capData.caps->Media->Extension != NULL) {
			MaximumNumberOfProfiles = deviceData.capData.caps->Media->Extension->ProfileCapabilities->MaximumNumberOfProfiles;
			stream << "MaximumNumberOfProfiles: " << MaximumNumberOfProfiles << endl;
		}

		RTPMulticast = deviceData.capData.caps->Media->StreamingCapabilities->RTPMulticast;
		stream << "RTPMulticast: " << RTPMulticast << endl;
		stream << "" << endl;


	//GetEventProperties
		TopicNamespaceLocation = eventData.resp.TopicNamespaceLocation;
		FixedTopicSet = eventData.resp.wsnt__FixedTopicSet;
		dom = eventData.resp.wstop__TopicSet->__any;
		vector<vector<string>> EventProperties = ParseEventProperties(dom);
		TopicExpressionDialect = eventData.resp.wsnt__TopicExpressionDialect;
		MessageContentFilterDialect = eventData.resp.MessageContentFilterDialect;
		ProducerPropertiesFilterDialect = eventData.resp.ProducerPropertiesFilterDialect;
		MessageContentSchemaLocation = eventData.resp.MessageContentSchemaLocation;

		for (int i = 0; i < EventProperties.size(); i++) {
			for (int j = 0; j < EventProperties[i].size(); j++) {
				stream << EventProperties[i][j] << " ";
			}
			stream << endl;
		}

		for (int i = 0; i < TopicNamespaceLocation.size(); i++) {
			stream << "TopicNamespaceLocation #" << i << ": " << TopicNamespaceLocation[i] << endl;
		}
		for (int i = 0; i < TopicExpressionDialect.size(); i++) {
			stream << "TopicExpressionDialect #" << i << ": " << TopicExpressionDialect[i] << endl;
		}
		for (int i = 0; i < MessageContentFilterDialect.size(); i++) {
			stream << "MessageContentFilterDialect #" << i << ": " << MessageContentFilterDialect[i] << endl;
		}
		for (int i = 0; i < ProducerPropertiesFilterDialect.size(); i++) {
			stream << "ProducerPropertiesFilterDialect #" << i << ": " << ProducerPropertiesFilterDialect[i] << endl;
		}

		for (int i = 0; i < MessageContentSchemaLocation.size(); i++) {
			stream << "MessageContentSchemaLocation #" << i << ": " << MessageContentSchemaLocation[i] << endl;
		}

	if (cam.path.boo == false) {
		cout << "" << endl;
		cout << stream.str();
	}
	else {
		cam.path.file << stream.str();
	}
}