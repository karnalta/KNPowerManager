#ifndef KNClock_H
#define KNClock_H

#include "Declarations.h"

/// <summary>
/// Time Management.
/// </summary>
class KNClock
{
private:
	RTC_DS1307* _rtc = NULL;
	EthernetUDP _udp;
	NTPClient* _ntpClient = NULL;

public:	
	/// <summary>
	/// Initializes a new instance of the <see cref="KNClock"/> class.
	/// </summary>
	KNClock();
	
	/// <summary>
	/// Updates datetime from NTP server.
	/// </summary>
	void UpdateFromNTPServer();
	
	/// <summary>
	///  Return date & time as a formatted string.
	/// </summary>
	/// <returns></returns>
	String GetDateTime();
};

#endif

