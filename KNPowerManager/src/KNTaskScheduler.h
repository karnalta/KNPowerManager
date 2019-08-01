#ifndef KNTaskScheduler_H
#define KNTaskScheduler_H

#include "Declarations.h"
#include "KNTask.h"

/// <summary>
/// Scheduled Task Manager.
/// </summary>
class KNTaskScheduler
{
private:
	Vector<KNTask*> _tasks;

public:	
	/// <summary>
	/// Initializes a new instance of the <see cref="KNTaskScheduler"/> class.
	/// </summary>
	KNTaskScheduler();
	
	/// <summary>
	/// Adds a task.
	/// </summary>
	/// <param name="task">The task.</param>
	void AddTask(KNTask* task);
	
	/// <summary>
	/// Process (in running loop).
	/// </summary>
	void Process();
};

#endif

