#pragma once
#pragma warning(disable : 4996)
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <windows.h>
using namespace std;


#define NEW_THREAD_MESSAGE(id) "New thread( "+std::to_string(id)+") was created cause no threads were available\n"
#define OUT_OF_POOL_CAPACITY_MESSAGE "ThreadPool limit was reached. Task was added and no threads were created\n"
#define TASK_ADDED "New task was added\n"
#define CREATED_MESSAGE(num,capacity,reseved) "Created "+std::to_string(num)+" threads. Pool user's capacity: "+std::to_string(capacity)+". Reseved amount of threads: "+std::to_string(reseved)+"\n"
#define DESTROYED_MESSAGE "ThreadPool was destroyed\n"
#define EXCEPTION_RAISED(threadID,message) "At thread "+std::to_string(threadID)+" was thrown\nException message:\n"+message+"\n"
#define INIT_THREAD(threadID) "Thread "+std::to_string(threadID)+" was added to queue\n"

class Logger {
public:
	void log(string message);
	Logger();
	~Logger();
private:
	const string filepath = "..\\logger.txt";
	ofstream output_file;
	CRITICAL_SECTION critical_section;
	string GetTime();
};