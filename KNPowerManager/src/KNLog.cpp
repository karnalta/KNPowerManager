#include "KNLog.h"

void KNLog::LogEvent(const char* const* stringAddress, bool ln, bool appendDate)
{
	// Retrieve string
	strcpy_P(StringBuffer, (char*)pgm_read_word(stringAddress));
	KNLog::LogEvent(StringBuffer, ln, appendDate);
}

void KNLog::LogEvent(String string, bool ln, bool appendDate)
{
	if (appendDate)
	{
		string = " -- " + string;
		string = LogClock->GetDateTime() + string;
	}

	// Output to serial if in DEBUG_MODE
	if (DEBUG_MODE)
	{
		ln == true ? Serial.println(string) : Serial.print(string);
	}

	// Write to log
	ln == true ? LogCardStorage->WriteToLog(string) : LogCardStorage->WriteToLog(string, false);
}

File KNLog::GetLog()
{
	return LogCardStorage->GetLog();;
}
