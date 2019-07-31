#ifndef KNLog_H
#define KNLog_H

#include "Declarations.h"
#include "KNClock.h"
#include "KNCardStorage.h"

static KNClock* LogClock;
static KNCardStorage* LogCardStorage;
static char StringBuffer[256];

/*
	Logging And Debugging class.
*/
class KNLog
{
public:
	// Log an event from a EPROM string
	static void LogEvent(const char* const* stringAddress, bool ln = true, bool appendDate = true);

	// Log an event from a String
	static void LogEvent(String string, bool ln = true, bool appendDate = true);

	// Get the log file
	static File GetLog();
};

#endif

