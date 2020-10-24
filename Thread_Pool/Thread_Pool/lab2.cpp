#include <windows.h>
#include <iostream>
#include <fstream>
#include "Logger.h"
#include "ThreadPool.h"

DWORD WINAPI TestFunction(LPVOID param);

int main()
{
    ThreadPool pool(10,3);
    for (int i = 0; i < 30; i++) {
        pool.addThreadPoolTask(TestFunction, (LPVOID)i);
        Sleep(100);
    }
    return 0;
}


DWORD WINAPI TestFunction(LPVOID param)
{
    DWORD procId = GetCurrentProcessId();
    DWORD threadId =  GetCurrentThreadId();
    Sleep(100*(int)param);
    std::cout << (int)param << " function has been executed in thread - " << threadId << std::endl;
    return 0;
}
