#ifndef KNCardStorage_H
#define KNCardStorage_H

#include "Declarations.h"

/// <summary>
/// SD Card logging operation.
/// </summary>
class KNCardStorage
{
private:
	bool _ready = false;

public:	
	/// <summary>
	/// Initializes a new instance of the <see cref="KNCardStorage"/> class.
	/// </summary>
	KNCardStorage();
	
	/// <summary>
	/// Writes to log file.
	/// </summary>
	/// <param name="line">The line.</param>
	/// <param name="ln">if set to <c>true</c> [ln].</param>
	void WriteToLog(String line, bool ln = true);
	
	/// <summary>
	/// Retrieve log file.
	/// </summary>
	/// <returns></returns>
	File GetLog();
};

#endif