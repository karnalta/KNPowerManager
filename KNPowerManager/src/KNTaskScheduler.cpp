#include "KNTaskScheduler.h"

KNTaskScheduler::KNTaskScheduler()
{

}

void KNTaskScheduler::AddTask(KNTask* task)
{
	if (task != NULL)
		_tasks.PushBack(task);
}

void KNTaskScheduler::Process()
{
	DateTime now = RTC_DS1307::now();

	// For each task
	for (int i = 0; i < _tasks.Size(); i++)
	{
		KNTask* task = _tasks[i];
		TimeSpan delta = now - task->GetLastExecDate();

		bool execTask = false;
		int taskType = task->GetType();

		if (taskType == 0)
		{
			if (delta.totalseconds() >= task->GetSecond())
				execTask = true;
		}
		else if (taskType == 1)
		{
			DateTime taskDate = DateTime(now.year(), now.month(), now.day(), task->GetHour(), task->GetMinute(), task->GetSecond());

			// Check if we are in time range for this task
			if ((now - taskDate).totalseconds() > 0 && (now - taskDate).totalseconds() < TASK_MAX_DELAY)
			{
				// Check last run date
				if ((now - task->GetLastExecDate()).totalseconds() > (TASK_MAX_DELAY * 2))
					execTask = true;
			}
		}
		else if (taskType == 2)
		{
			// Check if it's the correct day
			if (now.dayOfTheWeek() == task->GetDayOfWeek())
			{
				DateTime taskDate = DateTime(now.year(), now.month(), now.day(), task->GetHour(), task->GetMinute(), task->GetSecond());

				// Check if we are in time range for this task
				if ((now - taskDate).totalseconds() > 0 && (now - taskDate).totalseconds() < TASK_MAX_DELAY)
				{
					// Check last run date
					if ((now - task->GetLastExecDate()).totalseconds() > (TASK_MAX_DELAY * 2))
						execTask = true;
				}
			}
		}

		if (execTask)
		{
			if (task->GetLog())
			{
				// Log
				KNLog::LogEvent(&(kntaskscheduler_table[0]), false, true);
				KNLog::LogEvent(task->GetName(), true, false);
			}

			// Callback
			task->Callback();

			// Reset
			task->SetLastExecDate(now);
		}
	}
}
