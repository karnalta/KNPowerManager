#ifndef KNLog_H
#define KNLog_H

#include "Declarations.h"
#include "KNClock.h"
#include "KNCardStorage.h"

/// <summary>
/// Global access objects
/// </summary>

static KNClock* LogClock;
static KNCardStorage* LogCardStorage;
static char StringBuffer[256];

/// <summary>
/// Logging and debugging.
/// </summary>
class KNLog
{
public:	 
	/// <summary>
	/// Log an event from a EPROM string
	/// </summary>
	/// <param name="stringAddress">The string address.</param>
	/// <param name="ln">if set to <c>true</c> [ln].</param>
	/// <param name="appendDate">if set to <c>true</c> [append date].</param>
	static void LogEvent(const char* const* stringAddress, bool ln = true, bool appendDate = true);
	
	/// <summary>
	/// Log an event from a provided string
	/// </summary>
	/// <param name="string">The string.</param>
	/// <param name="ln">if set to <c>true</c> [ln].</param>
	/// <param name="appendDate">if set to <c>true</c> [append date].</param>
	static void LogEvent(String string, bool ln = true, bool appendDate = true);
	
	/// <summary>
	/// Gets the log file.
	/// </summary>
	/// <returns></returns>
	static File GetLog();
};

#endif

