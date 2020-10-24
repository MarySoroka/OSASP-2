#include "Task.h"

Task::Task(LPTHREAD_START_ROUTINE function_address, LPVOID params)
{
	functionAddress = function_address;
	parameters = params;
}
