#ifndef KNTaskScheduler_H
#define KNTaskScheduler_H

#include "Declarations.h"
#include "KNTask.h"

class KNTaskScheduler
{
private:
	Vector<KNTask*> _tasks;

public:
	// Constructor
	KNTaskScheduler();

	// Add a task
	void AddTask(KNTask* task);

	// Process tasks
	void Process();
};

#endif

