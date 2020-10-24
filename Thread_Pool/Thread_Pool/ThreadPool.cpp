#include "ThreadPool.h"

#define POOL_CAPACITY 100
#define DEFAULT_AMOUNT_OF_INITIALIZING_THREADS 3

ThreadPool::ThreadPool(int max_count,int init_count)
{
	if (max_count <= 0 || max_count>100)
		maxCount = POOL_CAPACITY;
	else
		maxCount = max_count;
	if (init_count < 3 || init_count >= maxCount) {
		initialized_threads_amount = DEFAULT_AMOUNT_OF_INITIALIZING_THREADS;
		init_value = DEFAULT_AMOUNT_OF_INITIALIZING_THREADS;
	}
	else {
		initialized_threads_amount = init_count;
		init_value = init_count;
	}
	working_count = 0;
	alive = true;

	bool isInit =InitializeCriticalSectionAndSpinCount(&thread_queue_of_section, 2000);
	InitializeConditionVariable(&thread_queue_variable);	
	
	isInit = InitializeCriticalSectionAndSpinCount(&critical_section, 2000);
	InitializeConditionVariable(&task_variable);

	thread_manager= CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)startManager, this, 0, NULL);
	manager_state = TRUE;
	thread_handle = (HANDLE*)calloc(POOL_CAPACITY, sizeof(HANDLE));
	for (int i = 0; i < init_count; i++) {
		thread_handle[i]= CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)threadStart, this, 0, NULL);
	}

	logger.log(CREATED_MESSAGE(init_count, maxCount, POOL_CAPACITY));
}

int ThreadPool::addThreadPoolTask(LPTHREAD_START_ROUTINE functionAddr, LPVOID params)
{
	if (functionAddr == NULL || !alive) {
		return -1;
	}
	if (initialized_threads_amount == maxCount)
		logger.log(OUT_OF_POOL_CAPACITY_MESSAGE);
	EnterCriticalSection(&critical_section);
	task_queue.push(new Task(functionAddr,params));
	WakeConditionVariable(&task_variable);
	LeaveCriticalSection(&critical_section);

	logger.log(TASK_ADDED);
	return 0;
}

DWORD WINAPI ThreadPool::threadStart(ThreadPool* pool)
{
	Thread* thread = new Thread();

	EnterCriticalSection(&pool->thread_queue_of_section);
	pool->thread_queue.push(thread);
	WakeConditionVariable(&pool->thread_queue_variable);
	LeaveCriticalSection(&pool->thread_queue_of_section);

	pool->logger.log(INIT_THREAD(GetCurrentThreadId()));

	while (pool->manager_state) {

		EnterCriticalSection(&thread->crSection);
		while (thread->taskForExecution == NULL && pool->manager_state)
			SleepConditionVariableCS(&thread->cnVariable, &thread->crSection, INFINITE);
		if (!pool->manager_state) {
			InterlockedExchange(&pool->working_count, pool->working_count - 1);
			LeaveCriticalSection(&thread->crSection);			
			if (thread->taskForExecution != NULL) {
				pool->logger.log("aaaaa1 " + std::to_string(GetCurrentThreadId()) + "\n");
			}
			return 0;
		}
		Task* task = thread->taskForExecution;
		thread->taskForExecution = NULL;
		InterlockedExchange(&pool->working_count, pool->working_count - 1);
		LeaveCriticalSection(&thread->crSection);


		try {
				task->functionAddress(task->parameters);
		}
		catch (exception ex) {
			pool->logger.log(EXCEPTION_RAISED(GetCurrentThreadId(), ex.what()));
		}
		delete task;
	}
	if (thread->taskForExecution != NULL) {
		pool->logger.log( std::to_string(GetCurrentThreadId()) + "\n");
	}
	return 0;
}

DWORD WINAPI ThreadPool::startManager(ThreadPool* pool)
{
	bool allTaskExecuted = false;

	EnterCriticalSection(&pool->thread_queue_of_section);
	while (pool->thread_queue.size() < pool->initialized_threads_amount)
		SleepConditionVariableCS(&pool->thread_queue_variable, &pool->thread_queue_of_section, INFINITE);
	LeaveCriticalSection(&pool->thread_queue_of_section);

	while (pool->alive || !pool->task_queue.empty()) {
	
		EnterCriticalSection(&pool->critical_section);
		while (pool->task_queue.empty() && pool->alive)
			SleepConditionVariableCS(&pool->task_variable, &pool->critical_section, 2000);
		if (!pool->alive && pool->task_queue.empty()) {
			LeaveCriticalSection(&pool->critical_section);
			while (!pool->allTaskExecuted()) { pool->logger.log( std::to_string(GetCurrentThreadId()) + " " + std::to_string(pool->task_queue.size()) + "\n"); Sleep(4000); }
			InterlockedExchange(&pool->manager_state, FALSE);
			return 0;
		}
		Task* task = pool->task_queue.front();
		pool->task_queue.pop();
		LeaveCriticalSection(&pool->critical_section);
		
		EnterCriticalSection(&pool->thread_queue_of_section);
		if (pool->working_count < pool->initialized_threads_amount ) {
			while (pool->thread_queue.front()->taskForExecution != NULL) {
				Thread* thread = pool->thread_queue.front();
				pool->thread_queue.pop();
				pool->thread_queue.push(thread);
			}
			pool->thread_queue.front()->taskForExecution = task;
			WakeConditionVariable(&pool->thread_queue.front()->cnVariable);
			LeaveCriticalSection(&pool->thread_queue_of_section);
			pool->logger.log("in " + std::to_string((int)task->parameters) + "\n");
		}
		else if (pool->initialized_threads_amount < pool->maxCount) {
			pool->initialized_threads_amount++;
			pool->thread_handle[pool->initialized_threads_amount] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadStart, pool, 0, NULL);
			SleepConditionVariableCS(&pool->thread_queue_variable, &pool->thread_queue_of_section, INFINITE);
			pool->thread_queue.back()->taskForExecution = task;
			WakeConditionVariable(&pool->thread_queue.back()->cnVariable);
			LeaveCriticalSection(&pool->thread_queue_of_section);
			pool->logger.log(NEW_THREAD_MESSAGE(GetThreadId(pool->thread_handle[pool->initialized_threads_amount])));
			pool->logger.log( std::to_string((int)task->parameters) + "\n");
		}
		InterlockedExchange(&pool->working_count, pool->working_count + 1);
		
	}
	while (!pool->allTaskExecuted()) { pool->logger.log( std::to_string(GetCurrentThreadId()) + " " + std::to_string(pool->task_queue.size()) + "\n"); Sleep(4000); }
	InterlockedExchange(&pool->manager_state, FALSE);
	return 0;
}

bool ThreadPool::allTaskExecuted()
{
	EnterCriticalSection(&thread_queue_of_section);
	bool allTaskExecuted = true;
	for (int i = 0; i < thread_queue.size(); i++) {
		Thread* thread = thread_queue.front();
		thread_queue.pop();
		thread_queue.push(thread);
		allTaskExecuted = allTaskExecuted && (thread->taskForExecution == NULL);
	}
	LeaveCriticalSection(&thread_queue_of_section);
	return allTaskExecuted;
}



ThreadPool::~ThreadPool()
{
	alive = false;
	WaitForSingleObject(thread_manager, INFINITE);
	WakeAllConditionVariable(&thread_queue_variable);
	WakeAllConditionVariable(&task_variable);
	std::cout << "End with working count " << working_count << std::endl;
	for (int i = 0; i < thread_queue.size(); i++) {
		Thread* thread = thread_queue.front();
		WakeAllConditionVariable(&thread->cnVariable);
		thread_queue.pop();
		thread_queue.push(thread);
	}
	WaitForMultipleObjects(initialized_threads_amount, thread_handle, TRUE, INFINITE);
	while (!thread_queue.empty()) {
		DeleteCriticalSection(&thread_queue.front()->crSection);
		thread_queue.pop();
	}
	for (int i = 0; i < initialized_threads_amount; i++) {
		CloseHandle(thread_handle[i]);
	}

	DeleteCriticalSection(&thread_queue_of_section);
	DeleteCriticalSection(&critical_section);
	CloseHandle(thread_manager);
	logger.log(DESTROYED_MESSAGE);
}
