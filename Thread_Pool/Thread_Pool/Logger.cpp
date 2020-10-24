#include "Logger.h"

void Logger::log(string message)
{
	EnterCriticalSection(&critical_section);
	if (output_file.is_open()) {
		output_file << GetTime()<<" --- "<<message;
	}
	LeaveCriticalSection(&critical_section);
}

Logger::Logger()
{
	bool isInit = InitializeCriticalSectionAndSpinCount(&critical_section, 2000);
	output_file.open(filepath);
}

Logger::~Logger()
{
	output_file.close();
	DeleteCriticalSection(&critical_section);
}

string Logger::GetTime()
{
	char* tmp;
	time_t end_time =time(NULL);
	tmp = ctime(&end_time);
	tmp[strlen(tmp) - 1] = '\0';
	return tmp;
}

