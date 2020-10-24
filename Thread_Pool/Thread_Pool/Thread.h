#pragma once
#include <windows.h>
#include "Task.h"

class Thread {
	public:
		Thread();
		~Thread();

		CRITICAL_SECTION crSection;
		CONDITION_VARIABLE cnVariable;
		Task* taskForExecution;
		BOOL alive;
};