#pragma once
#include <windows.h>
#include <queue>
#include "Logger.h"
#include "Task.h"
#include "Thread.h"

class ThreadPool {
	public:
		ThreadPool(int max,int init_amount);
		int addThreadPoolTask(LPTHREAD_START_ROUTINE function_addr, LPVOID params);
		~ThreadPool();

	private:
		int maxCount;
		volatile LONG working_count;
		volatile int initialized_threads_amount;
		volatile LONG alive;
		int init_value;

		Logger logger;
		HANDLE thread_manager;
		volatile LONG manager_state;
		HANDLE* thread_handle;

		std::queue<Thread*> thread_queue;
		CRITICAL_SECTION critical_section;
		CONDITION_VARIABLE task_variable;
		
		std::queue<Task*> task_queue;
		CRITICAL_SECTION thread_queue_of_section;
		CONDITION_VARIABLE thread_queue_variable;

		static DWORD WINAPI threadStart(ThreadPool* param);
		static DWORD WINAPI startManager(ThreadPool* param);
		bool allTaskExecuted();
};