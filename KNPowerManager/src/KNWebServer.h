#ifndef KNWebServer_H
#define KNWebServer_H

#include "Declarations.h"
#include "KNPwrSwitch.h"

/// <summary>
/// WebUI Provider.
/// </summary>
class KNWebServer
{
private:
	EthernetServer* _webServer = NULL;
	String _httpReq;

	void PrintReply(EthernetClient client, String sdFile);
	void IndexReply(EthernetClient client, KNPwrSwitch** psArray);
	void DownloadLogReply(EthernetClient client);

public:
	// Constructor
	KNWebServer(byte* ip, byte* mac, byte* gateway, byte* subnet);

	// Process event (called in loop)
	void Process(KNPwrSwitch** psArray);

	// Query a DNS name
	IPAddress QueryDNS(String domain);
};

#endif
