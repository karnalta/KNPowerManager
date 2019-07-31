#include "KNTask.h"

KNTask::KNTask(String name, byte hour, byte minute, byte second, taskCallback callback, bool log)
{
	_name = name;
	_type = 1;
	_hour = hour;
	_minute = minute;
	_second = second;
	_callback = callback;
	_log = log;
}

KNTask::KNTask(String name,  int second, taskCallback callback, bool log)
{
	_name = name;
	_type = 0;
	_second = second;
	_callback = callback;
	_log = log;
}

KNTask::KNTask(String name,  byte dayOfWeek, byte hour, byte minute, byte second, taskCallback callback, bool log)
{
	_name = name;
	_type = 2;
	_dayOfWeek = dayOfWeek;
	_hour = hour;
	_minute = minute;
	_second = second;
	_callback = callback;
	_log = log;
}

void KNTask::Callback()
{
	_callback();
}
