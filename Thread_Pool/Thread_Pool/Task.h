#pragma once
#include <windows.h>

class Task {
	public:
		LPTHREAD_START_ROUTINE functionAddress;
		LPVOID parameters;
		Task(LPTHREAD_START_ROUTINE funcAddr, LPVOID params);
};
