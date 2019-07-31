#ifndef KNCardStorage_H
#define KNCardStorage_H

#include "Declarations.h"

/*
	SD Card Controller class.
*/
class KNCardStorage
{
private:

public:
	// Constructor
	KNCardStorage();

	// Write to the log file
	void WriteToLog(String line, bool ln = true);

	File GetLog();
};

#endif