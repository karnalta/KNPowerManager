#ifndef KNTask_H
#define KNTask_H

#include "Declarations.h"

typedef void (*taskCallback)();

class KNTask
{
private:
	String _name = "";
	byte _type = 0; // 0 = Every seconds | 1 = Every day | 2 = Every week
	byte _dayOfWeek = 0;
	byte _hour = 0;
	byte _minute = 0;
	int _second = 0;
	DateTime _lastExecDate = RTC_DS1307::now();
	taskCallback _callback;
	bool _log = false;

public:
	inline String GetName() { return _name; }
	inline byte GetType() { return _type; }
	inline DateTime GetLastExecDate() { return _lastExecDate; }
	inline void SetLastExecDate(DateTime date) { _lastExecDate = date; }
	inline byte GetDayOfWeek() { return _dayOfWeek; }
	inline byte GetHour() { return _hour; }
	inline byte GetMinute() { return _minute; }
	inline int GetSecond() { return _second; }
	inline bool GetLog() { return _log; }

	// Constructors (Everyday at xx:xx:xx)
	KNTask(String name, byte hour, byte minute, byte second, taskCallback callback, bool log = true);
	
	// Constructor (Every xx seconds)
	KNTask(String name,  int second, taskCallback callback, bool log = true);

	// Constructor (Every week at xx:xx:xx)
	KNTask(String name,  byte dayOfWeek, byte hour, byte minute, byte second, taskCallback callback, bool log = true);

	// Call callback function
	void Callback();
};

#endif

