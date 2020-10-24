#include <iostream>
#include <windows.h>
#include <memoryapi.h>
#include <string>

TCHAR memory_name[] = TEXT("Critical_Section");

int constexpr process_max_amount = 5;

typedef struct {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
} PROCESS_INFO;


int main() {

	HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(CRITICAL_SECTION), memory_name);

	if (hMapFile == NULL) {
		std::cout << "Couldn't create critical section memory" << std::endl;
		return -1;
	}

	auto cs = (CRITICAL_SECTION*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(CRITICAL_SECTION));
	if (cs == NULL) {
		return -1;
	}

	InitializeCriticalSection(cs);
	std::cout << "Create critical section" << std::endl;

	PROCESS_INFO pInfos[process_max_amount];
	for (int i = 0; i < process_max_amount; i++) {
		ZeroMemory(&pInfos[i], sizeof(PROCESS_INFO));
		pInfos[i].si.cb = sizeof(STARTUPINFO);
		TCHAR buffer[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, buffer, MAX_PATH);
		std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
		std::wstring(buffer).substr(0, pos);
		if (!CreateProcess(std::wstring(std::wstring(buffer).substr(0, pos) + L"\\Test_Process.exe").c_str(), NULL, 0, 0, TRUE, 0, 0, NULL, &pInfos[i].si, &pInfos[i].pi)) {
			std::cout << "Failed to create process:" << GetLastError() << std::endl;
		}
	}

	for (int i = 0; i < process_max_amount; i++) {
		DWORD res = WaitForSingleObject(pInfos[i].pi.hProcess, INFINITE);
		CloseHandle(pInfos[i].pi.hProcess);
		CloseHandle(pInfos[i].pi.hThread);
	}

	DeleteCriticalSection(cs);
	UnmapViewOfFile(cs);
	CloseHandle(hMapFile);
	return 0;
}