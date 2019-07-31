#include "KNWebServer.h"

void KNWebServer::PrintReply(EthernetClient client, String sdFile)
{
	File htmlFile = SD.open(sdFile);
	if (htmlFile)
	{
		while (htmlFile.available())
			client.write(htmlFile.read());
	}
	
	htmlFile.close();
}

void KNWebServer::IndexReply(EthernetClient client, KNPwrSwitch** psArray)
{
	// Reply to client
	client.println("HTTP/1.1 200 OK");
	client.println("Content-Type: text/html");
	client.println();

	// Reply index.html header
	PrintReply(client, "/html/ind_hea.htm");

	// Reply content
	for (int i = 0; i < PWR_SWITCH_CNT; i++)
	{
		client.write("<tr>");
		client.write("<td>"); client.write(psArray[i]->GetName()); client.write("</td>");
		client.write("<td>"); client.write(psArray[i]->GetPowerStateDesc().c_str());  client.write("</td>");
		client.write("<td><a href=\"index.html?cmd=QSWITCH&id="); client.write(String(i).c_str()); client.write("\">Quick Switch Power</a></td>");
		client.write("<td align=\"right\">"); client.write(String(psArray[i]->GetRealPower()).c_str()); client.write(" W/h</td>");
		client.write("</tr>\n");
	}

	// Reply index.html footer
	PrintReply(client, "/html/ind_foo.htm");
}

void KNWebServer::DownloadLogReply(EthernetClient client)
{
	// Reply to client
	client.println("HTTP/1.1 200 OK");
	client.println("Content-Type: application/force-download");
	client.println("Content-Disposition: attachment;filename=\"KNMain.log\"");
	client.println();

	// Print content
	File logFile = KNLog::GetLog();
	if (logFile)
	{
		while (logFile.available())
		{
			client.write(logFile.read());
		}

		logFile.close();
	}
}

KNWebServer::KNWebServer(byte* ip, byte* mac, byte* gateway, byte* subnet)
{
	// Init TCP/IP
	Ethernet.begin(mac, ip, gateway, gateway, subnet);

	// Init WebServer
	_webServer = new EthernetServer(80);
	_webServer->begin();
}

// this method makes a HTTP connection to the server:
IPAddress KNWebServer::QueryDNS(String domain) 
{
	DNSClient dnsClient;
	dnsClient.begin(Ethernet.dnsServerIP());

	KNLog::LogEvent(&(knwebserver_table[0]), false, true);
	KNLog::LogEvent(&(knwebserver_table[1]), false, false);
	KNLog::LogEvent(domain, false, false);

	IPAddress ipAddress;
	dnsClient.getHostByName(domain.c_str(), ipAddress);

	return ipAddress;
}

void KNWebServer::Process(KNPwrSwitch** psArray)
{
	// Listen for incoming clients
	EthernetClient client = _webServer->available();
	if (client)
	{
		KNLog::LogEvent(&(knwebserver_table[1]));

		// An http request ends with a blank line
		int currentReqBufferIndex = 0;

		while (client.connected())
		{
			if (client.available())
			{
				// Read
				char c = client.read();

				// Storing max 100 char of the HTTP request first line
				if (currentReqBufferIndex < (HTTP_BUFFER_SIZE - 1))
				{
					_httpReq += c;
					currentReqBufferIndex++;
				}

				// Check for HTTP request end
				if (c == '\n')
				{
					KNLog::LogEvent(&(knwebserver_table[2]), false, true);
					KNLog::LogEvent(_httpReq, true, false);

					if (_httpReq.startsWith("GET / "))
					{					
						IndexReply(client, psArray);
					}
					else if (_httpReq.startsWith("GET /index.html?cmd=DownloadLog "))
					{
						DownloadLogReply(client);
					}
					else if (_httpReq.startsWith("GET /index.html?cmd=QSWITCH&id="))
					{
						int idIndex = _httpReq.indexOf("id=");
						byte id = (byte)_httpReq.substring(idIndex + 3, idIndex + 4).toInt();

						psArray[id]->QuickSwitchPower();

						// Redirect to homepage to avoid browser to submit several time the same command due to refresh
						client.println("HTTP/1.1 307 Temporary Redirect");
						client.println("Location: /");
						client.println("Connection: Close");
						client.println();
					}

					// Reset
					currentReqBufferIndex = 0;
					_httpReq = "";
					break;
				}
			}
		}

		// Give the web browser time to receive the data
		delay(1);

		// Close the connection
		client.stop();
	}
}
