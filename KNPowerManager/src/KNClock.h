#ifndef KNClock_H
#define KNClock_H

#include "Declarations.h"

/*
	Time Management class
*/
class KNClock
{
private:
	RTC_DS1307* _rtc = NULL;
	EthernetUDP _udp;
	NTPClient* _ntpClient = NULL;

public:
	// Constructor
	KNClock();

	// Update time from BE NTP server
	void UpdateFromNTPServer();

	// Return date & time as a formatted string
	String GetDateTime();
};

#endif

