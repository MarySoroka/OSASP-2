#include <iostream>
#include <windows.h>
#include <memoryapi.h>


TCHAR memory_name[] = TEXT("Critical_Section");

int constexpr loopCycles = 5;

int main() {

	HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, memory_name);

	if (hMapFile == NULL) {
		std::cout << "Shared memory of critical section is not availible" << std::endl;
		return -1;
	}

	auto critical_section = (CRITICAL_SECTION*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(CRITICAL_SECTION));
	if (critical_section == NULL) {
		std::cout << "Couldn't map view critical section" << std::endl;
		CloseHandle(hMapFile);
		return -1;
	}
	int process_id = GetCurrentProcessId();

	for (int i = 0; i < loopCycles; i++) {
		while (!TryEnterCriticalSection(critical_section)) {
			Sleep(10);
		}
		std::cout << "Process - " << process_id << " is on " << i << " loop and left" << std::endl;

		LeaveCriticalSection(critical_section);
		Sleep(10);

	}

	UnmapViewOfFile(critical_section);
	CloseHandle(hMapFile);
	return 0;
}