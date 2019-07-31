#include "KNCardStorage.h"

KNCardStorage::KNCardStorage()
{
	// Init SD card
	if (SD.begin(4))
	{
		// Check if log file exist
		if (!SD.exists("/log"))
			SD.mkdir("/log");
		if (!SD.exists("/log/main.log"))
		{
			File logFile = SD.open("/log/main.log", FILE_WRITE);
			logFile.close();
		}

		// Check if html files exist
		if (!SD.exists("/html/ind_hea.htm"))
			KNLog::LogEvent(&(kncardstorage_table[1]));
		if (!SD.exists("/html/ind_foo.htm"))
			KNLog::LogEvent(&(kncardstorage_table[1]));
	}
	else
		KNLog::LogEvent(&(kncardstorage_table[0]));
}

void KNCardStorage::WriteToLog(String line, bool ln = true)
{
	File logFile = SD.open("/log/main.log", FILE_WRITE);
	if (logFile)
	{
		ln == true ? logFile.println(line) : logFile.print(line);
		logFile.close();
		delay(100);
	}
}

File KNCardStorage::GetLog()
{
	return SD.open("/log/main.log", FILE_READ);
}
