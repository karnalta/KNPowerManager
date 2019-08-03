#include "KNClock.h"

KNClock::KNClock()
{
	// Setup RTC
	_rtc = new RTC_DS1307();
	if (!_rtc->begin())
		KNLog::LogEvent(&(knclock_table[0]));
	
	if (!_rtc->isrunning())
		_rtc->adjust(DateTime(F(__DATE__), F(__TIME__)));

	// Setup NTP client
	_ntpClient = new NTPClient(_udp, NTP_BELGIUM, 7200, 3600);
	_ntpClient->begin();
}

void KNClock::UpdateFromNTPServer()
{
	// Update NTP
	if (!_ntpClient->update())
		KNLog::LogEvent(&(knclock_table[1]));
	else
	{
		String formattedDate = _ntpClient->getFormattedDate();

		// Update RTC module
		_rtc->adjust(DateTime(formattedDate.substring(0, 4).toInt(), 
								formattedDate.substring(5, 7).toInt(), 
								formattedDate.substring(8, 10).toInt(), 
								formattedDate.substring(11, 13).toInt(), 
								formattedDate.substring(14, 16).toInt(), 
								formattedDate.substring(17, 19).toInt()));

		KNLog::LogEvent(&(knclock_table[2]));
	}
}

String KNClock::GetDateTime()
{
	DateTime now = _rtc->now();
	char message[20];

	int Year = now.year();
	int Month = now.month();
	int Day = now.day();
	int Hour = now.hour();
	int Minute = now.minute();
	int Second = now.second();

	sprintf(message, "%02d-%02d-%d %02d:%02d:%02d", Day, Month, Year, Hour, Minute, Second);

	return message;
}
