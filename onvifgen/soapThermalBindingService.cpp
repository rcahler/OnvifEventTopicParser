/* soapThermalBindingService.cpp
   Generated by gSOAP 2.8.66 for onvif.h

gSOAP XML Web services tools
Copyright (C) 2000-2018, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#include "soapThermalBindingService.h"

ThermalBindingService::ThermalBindingService() : soap(SOAP_IO_DEFAULT)
{	ThermalBindingService_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

ThermalBindingService::ThermalBindingService(const ThermalBindingService& rhs)
{	soap_copy_context(this, &rhs);
}

ThermalBindingService::ThermalBindingService(const struct soap &_soap) : soap(_soap)
{ }

ThermalBindingService::ThermalBindingService(soap_mode iomode) : soap(iomode)
{	ThermalBindingService_init(iomode, iomode);
}

ThermalBindingService::ThermalBindingService(soap_mode imode, soap_mode omode) : soap(imode, omode)
{	ThermalBindingService_init(imode, omode);
}

ThermalBindingService::~ThermalBindingService()
{
	this->destroy();
}

void ThermalBindingService::ThermalBindingService_init(soap_mode imode, soap_mode omode)
{	soap_imode(this, imode);
	soap_omode(this, omode);
	static const struct Namespace namespaces[] = {
        {"SOAP-ENV", "http://www.w3.org/2003/05/soap-envelope", "http://schemas.xmlsoap.org/soap/envelope/", NULL},
        {"SOAP-ENC", "http://www.w3.org/2003/05/soap-encoding", "http://schemas.xmlsoap.org/soap/encoding/", NULL},
        {"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
        {"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
        {"c14n", "http://www.w3.org/2001/10/xml-exc-c14n#", NULL, NULL},
        {"ds", "http://www.w3.org/2000/09/xmldsig#", NULL, NULL},
        {"saml1", "urn:oasis:names:tc:SAML:1.0:assertion", NULL, NULL},
        {"saml2", "urn:oasis:names:tc:SAML:2.0:assertion", NULL, NULL},
        {"wsu", "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd", NULL, NULL},
        {"xenc", "http://www.w3.org/2001/04/xmlenc#", NULL, NULL},
        {"wsc", "http://docs.oasis-open.org/ws-sx/ws-secureconversation/200512", NULL, NULL},
        {"wsse", "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd", "http://docs.oasis-open.org/wss/oasis-wss-wssecurity-secext-1.1.xsd", NULL},
        {"chan", "http://schemas.microsoft.com/ws/2005/02/duplex", NULL, NULL},
        {"wsa5", "http://www.w3.org/2005/08/addressing", "http://schemas.xmlsoap.org/ws/2004/08/addressing", NULL},
        {"wsdd", "http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01", NULL, NULL},
        {"ns12", "http://www.onvif.org/ver20/analytics", NULL, NULL},
        {"ns2", "http://www.onvif.org/ver10/pacs", NULL, NULL},
        {"ns10", "http://www.onvif.org/ver20/analytics/radiometry", NULL, NULL},
        {"xmime", "http://tempuri.org/xmime.xsd", NULL, NULL},
        {"xop", "http://www.w3.org/2004/08/xop/include", NULL, NULL},
        {"wsrfbf", "http://docs.oasis-open.org/wsrf/bf-2", NULL, NULL},
        {"tt", "http://www.onvif.org/ver10/schema", NULL, NULL},
        {"wstop", "http://docs.oasis-open.org/wsn/t-1", NULL, NULL},
        {"wsrfr", "http://docs.oasis-open.org/wsrf/r-2", NULL, NULL},
        {"ns1", "http://www.onvif.org/ver10/accesscontrol/wsdl", NULL, NULL},
        {"ns11", "http://www.onvif.org/ver10/network/wsdl/", NULL, NULL},
        {"ns13", "http://www.onvif.org/ver10/schedule/wsdl", NULL, NULL},
        {"ns14", "http://www.onvif.org/ver10/thermal/wsdl", NULL, NULL},
        {"ns3", "http://www.onvif.org/ver10/accessrules/wsdl", NULL, NULL},
        {"ns4", "http://www.onvif.org/ver10/actionengine/wsdl", NULL, NULL},
        {"ns5", "http://www.onvif.org/ver10/advancedsecurity/wsdl", NULL, NULL},
        {"ns6", "http://www.onvif.org/ver10/credential/wsdl", NULL, NULL},
        {"ns7", "http://www.onvif.org/ver10/doorcontrol/wsdl", NULL, NULL},
        {"ns8", "http://www.onvif.org/ver20/media/wsdl", NULL, NULL},
        {"ns9", "http://www.onvif.org/ver10/provisioning/wsdl", NULL, NULL},
        {"tad", "http://www.onvif.org/ver10/analyticsdevice/wsdl", NULL, NULL},
        {"tan", "http://www.onvif.org/ver20/analytics/wsdl", NULL, NULL},
        {"tds", "http://www.onvif.org/ver10/device/wsdl", NULL, NULL},
        {"tev", "http://www.onvif.org/ver10/events/wsdl", NULL, NULL},
        {"wsnt", "http://docs.oasis-open.org/wsn/b-2", NULL, NULL},
        {"timg", "http://www.onvif.org/ver20/imaging/wsdl", NULL, NULL},
        {"tls", "http://www.onvif.org/ver10/display/wsdl", NULL, NULL},
        {"tmd", "http://www.onvif.org/ver10/deviceIO/wsdl", NULL, NULL},
        {"tptz", "http://www.onvif.org/ver20/ptz/wsdl", NULL, NULL},
        {"trc", "http://www.onvif.org/ver10/recording/wsdl", NULL, NULL},
        {"trp", "http://www.onvif.org/ver10/replay/wsdl", NULL, NULL},
        {"trt", "http://www.onvif.org/ver10/media/wsdl", NULL, NULL},
        {"trv", "http://www.onvif.org/ver10/receiver/wsdl", NULL, NULL},
        {"tse", "http://www.onvif.org/ver10/search/wsdl", NULL, NULL},
        {NULL, NULL, NULL, NULL}
    };
	soap_set_namespaces(this, namespaces);
}

void ThermalBindingService::destroy()
{	soap_destroy(this);
	soap_end(this);
}

void ThermalBindingService::reset()
{	this->destroy();
	soap_done(this);
	soap_initialize(this);
	ThermalBindingService_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

#ifndef WITH_PURE_VIRTUAL
ThermalBindingService *ThermalBindingService::copy()
{	ThermalBindingService *dup = SOAP_NEW_UNMANAGED(ThermalBindingService(*(struct soap*)this));
	return dup;
}
#endif

ThermalBindingService& ThermalBindingService::operator=(const ThermalBindingService& rhs)
{	soap_done(this);
	soap_copy_context(this, &rhs);
	return *this;
}

int ThermalBindingService::soap_close_socket()
{	return soap_closesock(this);
}

int ThermalBindingService::soap_force_close_socket()
{	return soap_force_closesock(this);
}

int ThermalBindingService::soap_senderfault(const char *string, const char *detailXML)
{	return ::soap_sender_fault(this, string, detailXML);
}

int ThermalBindingService::soap_senderfault(const char *subcodeQName, const char *string, const char *detailXML)
{	return ::soap_sender_fault_subcode(this, subcodeQName, string, detailXML);
}

int ThermalBindingService::soap_receiverfault(const char *string, const char *detailXML)
{	return ::soap_receiver_fault(this, string, detailXML);
}

int ThermalBindingService::soap_receiverfault(const char *subcodeQName, const char *string, const char *detailXML)
{	return ::soap_receiver_fault_subcode(this, subcodeQName, string, detailXML);
}

void ThermalBindingService::soap_print_fault(FILE *fd)
{	::soap_print_fault(this, fd);
}

#ifndef WITH_LEAN
#ifndef WITH_COMPAT
void ThermalBindingService::soap_stream_fault(std::ostream& os)
{	::soap_stream_fault(this, os);
}
#endif

char *ThermalBindingService::soap_sprint_fault(char *buf, size_t len)
{	return ::soap_sprint_fault(this, buf, len);
}
#endif

void ThermalBindingService::soap_noheader()
{	this->header = NULL;
}

void ThermalBindingService::soap_header(struct _wsse__Security *wsse__Security, char *wsa5__MessageID, struct wsa5__RelatesToType *wsa5__RelatesTo, struct wsa5__EndpointReferenceType *wsa5__From, struct wsa5__EndpointReferenceType *wsa5__ReplyTo, struct wsa5__EndpointReferenceType *wsa5__FaultTo, char *wsa5__To, char *wsa5__Action, struct chan__ChannelInstanceType *chan__ChannelInstance, struct wsdd__AppSequenceType *wsdd__AppSequence)
{	::soap_header(this);
	this->header->wsse__Security = wsse__Security;
	this->header->wsa5__MessageID = wsa5__MessageID;
	this->header->wsa5__RelatesTo = wsa5__RelatesTo;
	this->header->wsa5__From = wsa5__From;
	this->header->wsa5__ReplyTo = wsa5__ReplyTo;
	this->header->wsa5__FaultTo = wsa5__FaultTo;
	this->header->wsa5__To = wsa5__To;
	this->header->wsa5__Action = wsa5__Action;
	this->header->chan__ChannelInstance = chan__ChannelInstance;
	this->header->wsdd__AppSequence = wsdd__AppSequence;
}

::SOAP_ENV__Header *ThermalBindingService::soap_header()
{	return this->header;
}

#ifndef WITH_NOIO
int ThermalBindingService::run(int port)
{	if (!soap_valid_socket(this->master) && !soap_valid_socket(this->bind(NULL, port, 100)))
		return this->error;
	for (;;)
	{	if (!soap_valid_socket(this->accept()))
		{	if (this->errnum == 0) // timeout?
				this->error = SOAP_OK;
			break;
		}
		if (this->serve())
			break;
		this->destroy();
	}
	return this->error;
}

#if defined(WITH_OPENSSL) || defined(WITH_GNUTLS)
int ThermalBindingService::ssl_run(int port)
{	if (!soap_valid_socket(this->master) && !soap_valid_socket(this->bind(NULL, port, 100)))
		return this->error;
	for (;;)
	{	if (!soap_valid_socket(this->accept()))
		{	if (this->errnum == 0) // timeout?
				this->error = SOAP_OK;
			break;
		}
		if (this->ssl_accept() || this->serve())
			break;
		this->destroy();
	}
	return this->error;
}
#endif

SOAP_SOCKET ThermalBindingService::bind(const char *host, int port, int backlog)
{	return soap_bind(this, host, port, backlog);
}

SOAP_SOCKET ThermalBindingService::accept()
{	return soap_accept(this);
}

#if defined(WITH_OPENSSL) || defined(WITH_GNUTLS)
int ThermalBindingService::ssl_accept()
{	return soap_ssl_accept(this);
}
#endif
#endif

int ThermalBindingService::serve()
{
#ifndef WITH_FASTCGI
	this->keep_alive = this->max_keep_alive + 1;
#endif
	do
	{
#ifndef WITH_FASTCGI
		if (this->keep_alive > 0 && this->max_keep_alive > 0)
			this->keep_alive--;
#endif
		if (soap_begin_serve(this))
		{	if (this->error >= SOAP_STOP)
				continue;
			return this->error;
		}
		if ((dispatch() || (this->fserveloop && this->fserveloop(this))) && this->error && this->error < SOAP_STOP)
		{
#ifdef WITH_FASTCGI
			soap_send_fault(this);
#else
			return soap_send_fault(this);
#endif
		}
#ifdef WITH_FASTCGI
		soap_destroy(this);
		soap_end(this);
	} while (1);
#else
	} while (this->keep_alive);
#endif
	return SOAP_OK;
}

static int serve___ns14__GetServiceCapabilities(ThermalBindingService*);
static int serve___ns14__GetConfigurationOptions(ThermalBindingService*);
static int serve___ns14__GetConfiguration(ThermalBindingService*);
static int serve___ns14__GetConfigurations(ThermalBindingService*);
static int serve___ns14__SetConfiguration(ThermalBindingService*);
static int serve___ns14__GetRadiometryConfigurationOptions(ThermalBindingService*);
static int serve___ns14__GetRadiometryConfiguration(ThermalBindingService*);
static int serve___ns14__SetRadiometryConfiguration(ThermalBindingService*);

int ThermalBindingService::dispatch()
{
	soap_peek_element(this);
	if (!soap_match_tag(this, this->tag, "ns14:GetServiceCapabilities"))
		return serve___ns14__GetServiceCapabilities(this);
	if (!soap_match_tag(this, this->tag, "ns14:GetConfigurationOptions"))
		return serve___ns14__GetConfigurationOptions(this);
	if (!soap_match_tag(this, this->tag, "ns14:GetConfiguration"))
		return serve___ns14__GetConfiguration(this);
	if (!soap_match_tag(this, this->tag, "ns14:GetConfigurations"))
		return serve___ns14__GetConfigurations(this);
	if (!soap_match_tag(this, this->tag, "ns14:SetConfiguration"))
		return serve___ns14__SetConfiguration(this);
	if (!soap_match_tag(this, this->tag, "ns14:GetRadiometryConfigurationOptions"))
		return serve___ns14__GetRadiometryConfigurationOptions(this);
	if (!soap_match_tag(this, this->tag, "ns14:GetRadiometryConfiguration"))
		return serve___ns14__GetRadiometryConfiguration(this);
	if (!soap_match_tag(this, this->tag, "ns14:SetRadiometryConfiguration"))
		return serve___ns14__SetRadiometryConfiguration(this);
	return this->error = SOAP_NO_METHOD;
}

static int serve___ns14__GetServiceCapabilities(ThermalBindingService *soap)
{	struct __ns14__GetServiceCapabilities soap_tmp___ns14__GetServiceCapabilities;
	_ns14__GetServiceCapabilitiesResponse ns14__GetServiceCapabilitiesResponse;
	ns14__GetServiceCapabilitiesResponse.soap_default(soap);
	soap_default___ns14__GetServiceCapabilities(soap, &soap_tmp___ns14__GetServiceCapabilities);
	if (!soap_get___ns14__GetServiceCapabilities(soap, &soap_tmp___ns14__GetServiceCapabilities, "-ns14:GetServiceCapabilities", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->GetServiceCapabilities(soap_tmp___ns14__GetServiceCapabilities.ns14__GetServiceCapabilities, ns14__GetServiceCapabilitiesResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	ns14__GetServiceCapabilitiesResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || ns14__GetServiceCapabilitiesResponse.soap_put(soap, "ns14:GetServiceCapabilitiesResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || ns14__GetServiceCapabilitiesResponse.soap_put(soap, "ns14:GetServiceCapabilitiesResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___ns14__GetConfigurationOptions(ThermalBindingService *soap)
{	struct __ns14__GetConfigurationOptions soap_tmp___ns14__GetConfigurationOptions;
	_ns14__GetConfigurationOptionsResponse ns14__GetConfigurationOptionsResponse;
	ns14__GetConfigurationOptionsResponse.soap_default(soap);
	soap_default___ns14__GetConfigurationOptions(soap, &soap_tmp___ns14__GetConfigurationOptions);
	if (!soap_get___ns14__GetConfigurationOptions(soap, &soap_tmp___ns14__GetConfigurationOptions, "-ns14:GetConfigurationOptions", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->GetConfigurationOptions(soap_tmp___ns14__GetConfigurationOptions.ns14__GetConfigurationOptions, ns14__GetConfigurationOptionsResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	ns14__GetConfigurationOptionsResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || ns14__GetConfigurationOptionsResponse.soap_put(soap, "ns14:GetConfigurationOptionsResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || ns14__GetConfigurationOptionsResponse.soap_put(soap, "ns14:GetConfigurationOptionsResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___ns14__GetConfiguration(ThermalBindingService *soap)
{	struct __ns14__GetConfiguration soap_tmp___ns14__GetConfiguration;
	_ns14__GetConfigurationResponse ns14__GetConfigurationResponse;
	ns14__GetConfigurationResponse.soap_default(soap);
	soap_default___ns14__GetConfiguration(soap, &soap_tmp___ns14__GetConfiguration);
	if (!soap_get___ns14__GetConfiguration(soap, &soap_tmp___ns14__GetConfiguration, "-ns14:GetConfiguration", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->GetConfiguration(soap_tmp___ns14__GetConfiguration.ns14__GetConfiguration, ns14__GetConfigurationResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	ns14__GetConfigurationResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || ns14__GetConfigurationResponse.soap_put(soap, "ns14:GetConfigurationResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || ns14__GetConfigurationResponse.soap_put(soap, "ns14:GetConfigurationResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___ns14__GetConfigurations(ThermalBindingService *soap)
{	struct __ns14__GetConfigurations soap_tmp___ns14__GetConfigurations;
	_ns14__GetConfigurationsResponse ns14__GetConfigurationsResponse;
	ns14__GetConfigurationsResponse.soap_default(soap);
	soap_default___ns14__GetConfigurations(soap, &soap_tmp___ns14__GetConfigurations);
	if (!soap_get___ns14__GetConfigurations(soap, &soap_tmp___ns14__GetConfigurations, "-ns14:GetConfigurations", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->GetConfigurations(soap_tmp___ns14__GetConfigurations.ns14__GetConfigurations, ns14__GetConfigurationsResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	ns14__GetConfigurationsResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || ns14__GetConfigurationsResponse.soap_put(soap, "ns14:GetConfigurationsResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || ns14__GetConfigurationsResponse.soap_put(soap, "ns14:GetConfigurationsResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___ns14__SetConfiguration(ThermalBindingService *soap)
{	struct __ns14__SetConfiguration soap_tmp___ns14__SetConfiguration;
	_ns14__SetConfigurationResponse ns14__SetConfigurationResponse;
	ns14__SetConfigurationResponse.soap_default(soap);
	soap_default___ns14__SetConfiguration(soap, &soap_tmp___ns14__SetConfiguration);
	if (!soap_get___ns14__SetConfiguration(soap, &soap_tmp___ns14__SetConfiguration, "-ns14:SetConfiguration", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->SetConfiguration(soap_tmp___ns14__SetConfiguration.ns14__SetConfiguration, ns14__SetConfigurationResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	ns14__SetConfigurationResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || ns14__SetConfigurationResponse.soap_put(soap, "ns14:SetConfigurationResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || ns14__SetConfigurationResponse.soap_put(soap, "ns14:SetConfigurationResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___ns14__GetRadiometryConfigurationOptions(ThermalBindingService *soap)
{	struct __ns14__GetRadiometryConfigurationOptions soap_tmp___ns14__GetRadiometryConfigurationOptions;
	_ns14__GetRadiometryConfigurationOptionsResponse ns14__GetRadiometryConfigurationOptionsResponse;
	ns14__GetRadiometryConfigurationOptionsResponse.soap_default(soap);
	soap_default___ns14__GetRadiometryConfigurationOptions(soap, &soap_tmp___ns14__GetRadiometryConfigurationOptions);
	if (!soap_get___ns14__GetRadiometryConfigurationOptions(soap, &soap_tmp___ns14__GetRadiometryConfigurationOptions, "-ns14:GetRadiometryConfigurationOptions", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->GetRadiometryConfigurationOptions(soap_tmp___ns14__GetRadiometryConfigurationOptions.ns14__GetRadiometryConfigurationOptions, ns14__GetRadiometryConfigurationOptionsResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	ns14__GetRadiometryConfigurationOptionsResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || ns14__GetRadiometryConfigurationOptionsResponse.soap_put(soap, "ns14:GetRadiometryConfigurationOptionsResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || ns14__GetRadiometryConfigurationOptionsResponse.soap_put(soap, "ns14:GetRadiometryConfigurationOptionsResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___ns14__GetRadiometryConfiguration(ThermalBindingService *soap)
{	struct __ns14__GetRadiometryConfiguration soap_tmp___ns14__GetRadiometryConfiguration;
	_ns14__GetRadiometryConfigurationResponse ns14__GetRadiometryConfigurationResponse;
	ns14__GetRadiometryConfigurationResponse.soap_default(soap);
	soap_default___ns14__GetRadiometryConfiguration(soap, &soap_tmp___ns14__GetRadiometryConfiguration);
	if (!soap_get___ns14__GetRadiometryConfiguration(soap, &soap_tmp___ns14__GetRadiometryConfiguration, "-ns14:GetRadiometryConfiguration", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->GetRadiometryConfiguration(soap_tmp___ns14__GetRadiometryConfiguration.ns14__GetRadiometryConfiguration, ns14__GetRadiometryConfigurationResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	ns14__GetRadiometryConfigurationResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || ns14__GetRadiometryConfigurationResponse.soap_put(soap, "ns14:GetRadiometryConfigurationResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || ns14__GetRadiometryConfigurationResponse.soap_put(soap, "ns14:GetRadiometryConfigurationResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___ns14__SetRadiometryConfiguration(ThermalBindingService *soap)
{	struct __ns14__SetRadiometryConfiguration soap_tmp___ns14__SetRadiometryConfiguration;
	_ns14__SetRadiometryConfigurationResponse ns14__SetRadiometryConfigurationResponse;
	ns14__SetRadiometryConfigurationResponse.soap_default(soap);
	soap_default___ns14__SetRadiometryConfiguration(soap, &soap_tmp___ns14__SetRadiometryConfiguration);
	if (!soap_get___ns14__SetRadiometryConfiguration(soap, &soap_tmp___ns14__SetRadiometryConfiguration, "-ns14:SetRadiometryConfiguration", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->SetRadiometryConfiguration(soap_tmp___ns14__SetRadiometryConfiguration.ns14__SetRadiometryConfiguration, ns14__SetRadiometryConfigurationResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	ns14__SetRadiometryConfigurationResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || ns14__SetRadiometryConfigurationResponse.soap_put(soap, "ns14:SetRadiometryConfigurationResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || ns14__SetRadiometryConfigurationResponse.soap_put(soap, "ns14:SetRadiometryConfigurationResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}
/* End of server object code */
