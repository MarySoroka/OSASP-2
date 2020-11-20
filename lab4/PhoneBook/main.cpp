#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <string>
#include <vector>

#include "PhoneBookLib.h"
#pragma comment(lib, "comctl32.lib")

const wchar_t* headers[] = {
		{ L"Telephone" },
		{ L"First name" },
		{ L"Last name" },
		{ L"Middle name" },
		{ L"Street" },
		{ L"House" },
		{ L"Housing" },
		{ L"Flat" }
};

HINSTANCE hInst;

PhoneBook phoneBook;
#define IDC_LISTVIEW (HMENU)101
#define IDC_TELEDIT (HMENU)102
#define IDC_FNEDIT (HMENU)103
#define IDC_MNEDIT (HMENU)104
#define IDC_LNEDIT (HMENU)105
#define IDC_HSEDIT (HMENU)106
#define IDC_HSNGEDIT (HMENU)107
#define IDC_FLTEDIT (HMENU)108
#define IDC_STREDIT (HMENU)109
#define IDC_SEARCHBTN (HMENU)110

typedef struct TMainWindow {
	HWND hWnd;
	HWND hListView;
	HWND hTel;
	HWND hFn;
	HWND hMn;
	HWND hLn;
	HWND hHs;
	HWND hHsng;
	HWND hFlt;
	HWND hStr;
	HWND hSearch;

}TMainWindow, * PMainWindow;

std::wstring ExePath() {
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	return std::wstring(buffer).substr(0, pos-6);
}

HWND CreateListView(HWND parent, int columns) {

	RECT rcClient;                       
	GetClientRect(parent, &rcClient);

	HWND hWndListView = CreateWindow(WC_LISTVIEW, L"", WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_EDITLABELS | WS_EX_CLIENTEDGE, 0, rcClient.top + 50,
										rcClient.right - rcClient.left, rcClient.bottom - rcClient.top -50, parent, IDC_LISTVIEW, hInst, NULL);	

	LVCOLUMN c = { 0 };
	c.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for (int i = 0; i < columns; i++) {
		c.fmt = LVCFMT_LEFT;
		c.pszText = (LPWSTR)headers[i];
		c.iSubItem = i;
		c.cx = 100;
		ListView_InsertColumn(hWndListView, i, &c);
	}	
	return hWndListView;
}

void UpdateList(HWND hListView, std::list<Record*> records) {
	ListView_DeleteAllItems(hListView);
	LVITEM item;
	ZeroMemory(&item, sizeof(LVITEM));
	item.mask = LVIF_TEXT | LVIF_STATE;
	for (auto it = records.begin(); it != records.end(); it++) {
		auto i = std::distance(records.begin(), it);
		item.iItem = i;
		item.iSubItem = 0;
		item.pszText = (LPWSTR)(*it)->telephone->c_str();
		ListView_InsertItem(hListView, &item);		
		ListView_SetItemText(hListView, i, 1, (LPWSTR)(*it)->fname->c_str());
		ListView_SetItemText(hListView, i, 2, (LPWSTR)(*it)->lname->c_str());
		ListView_SetItemText(hListView, i, 3, (LPWSTR)(*it)->mname->c_str());
		ListView_SetItemText(hListView, i, 4, (LPWSTR)(*it)->street->c_str());
		std::wstring tmp = std::to_wstring((*it)->house);
		ListView_SetItemText(hListView, i, 5, (LPWSTR)tmp.c_str());
		tmp = std::to_wstring((*it)->housing);
		ListView_SetItemText(hListView, i, 6, (LPWSTR)tmp.c_str());
		tmp = std::to_wstring((*it)->flat);
		ListView_SetItemText(hListView, i, 7, (LPWSTR)tmp.c_str());
	}
}

LRESULT CALLBACK WndProc(_In_ HWND   hWnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
	PMainWindow pSelf;

	if (uMsg == WM_CREATE) {
		pSelf = (PMainWindow)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(TMainWindow));
		if (pSelf != NULL) {
			pSelf->hWnd = hWnd;
			pSelf->hListView = CreateListView(hWnd,8);
			pSelf->hTel = CreateWindow(WC_EDIT , TEXT(""), WS_CHILD | WS_OVERLAPPED | WS_EX_CLIENTEDGE | WS_VISIBLE | WS_BORDER| ES_LEFT | ES_AUTOHSCROLL,
										3, 3, 95, 20, hWnd, IDC_TELEDIT, hInst, NULL);
			pSelf->hFn = CreateWindow(WC_EDIT, TEXT(""), WS_CHILD | WS_OVERLAPPED | WS_EX_CLIENTEDGE | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
										103, 3, 95, 20, hWnd, IDC_FNEDIT, hInst, NULL);
			pSelf->hMn = CreateWindow(WC_EDIT, TEXT(""), WS_CHILD | WS_OVERLAPPED | WS_EX_CLIENTEDGE | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
										203, 3, 95, 20, hWnd, IDC_MNEDIT, hInst, NULL);
			pSelf->hLn = CreateWindow(WC_EDIT, TEXT(""), WS_CHILD | WS_OVERLAPPED | WS_EX_CLIENTEDGE | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
										303, 3, 95, 20, hWnd, IDC_LNEDIT, hInst, NULL);
			pSelf->hStr = CreateWindow(WC_EDIT, TEXT(""), WS_CHILD | WS_OVERLAPPED | WS_EX_CLIENTEDGE | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
										403, 3, 95, 20, hWnd, IDC_FLTEDIT, hInst, NULL);
			pSelf->hHs = CreateWindow(WC_EDIT, TEXT(""), WS_CHILD | WS_OVERLAPPED | WS_EX_CLIENTEDGE | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_NUMBER,
										503, 3, 95, 20, hWnd, IDC_HSEDIT, hInst, NULL);
			pSelf->hHsng = CreateWindow(WC_EDIT, TEXT(""), WS_CHILD | WS_OVERLAPPED | WS_EX_CLIENTEDGE | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_NUMBER,
										603, 3, 95, 20, hWnd, IDC_HSNGEDIT, hInst, NULL);
			pSelf->hFlt = CreateWindow(WC_EDIT, TEXT(""), WS_CHILD | WS_OVERLAPPED | WS_EX_CLIENTEDGE | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_NUMBER,
										703, 3, 75, 20, hWnd, IDC_FLTEDIT, hInst, NULL);
			pSelf->hSearch = CreateWindow(WC_BUTTON, TEXT("Search"), WS_CHILD | WS_OVERLAPPED | WS_EX_CLIENTEDGE | WS_VISIBLE | WS_BORDER | BS_CENTER,
				3, 25, 774, 23 , hWnd, IDC_SEARCHBTN, hInst, NULL);

			SetWindowText(pSelf->hHs, L"0");
			SetWindowText(pSelf->hHsng, L"0");
			SetWindowText(pSelf->hFlt, L"0");

			phoneBook.loadFromFile(ExePath()+L"\\data.txt");
			std::list<Record*> tmp;
			for (int i = 0; i < phoneBook.records.size(); i++)
				tmp.push_back(phoneBook[i]);
			UpdateList(pSelf->hListView, tmp);
			

			SetWindowLong(hWnd, 0, (LONG)pSelf);

		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	PAINTSTRUCT ps;
	HDC hdc;

	pSelf = (PMainWindow)GetWindowLong(hWnd, 0);
	if (!pSelf) {
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	switch (uMsg) {
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		EndPaint(hWnd, &ps);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == (WORD)IDC_SEARCHBTN) {
			wchar_t telBuff[1024];
			wchar_t fnameBuff[1024];
			wchar_t mnameBuff[1024];
			wchar_t lnameBuff[1024];
			wchar_t streetBuff[1024];
			wchar_t houseBuff[1024];
			wchar_t housingBuff[1024];
			wchar_t flatBuff[1024];
			
			GetWindowText(pSelf->hTel, (LPWSTR)telBuff, 1024);
			GetWindowText(pSelf->hFn, (LPWSTR)fnameBuff, 1024);
			GetWindowText(pSelf->hMn, (LPWSTR)mnameBuff, 1024);
			GetWindowText(pSelf->hLn, (LPWSTR)lnameBuff, 1024);
			GetWindowText(pSelf->hStr, (LPWSTR)streetBuff, 1024);
			GetWindowText(pSelf->hHs, (LPWSTR)houseBuff, 1024);
			GetWindowText(pSelf->hHsng, (LPWSTR)housingBuff, 1024);
			GetWindowText(pSelf->hFlt, (LPWSTR)flatBuff, 1024);

			std::wstring tel(telBuff);
			std::wstring fname(fnameBuff);
			std::wstring mname(mnameBuff);
			std::wstring lname(lnameBuff);
			std::wstring street(streetBuff);
			std::wstring house(houseBuff);
			std::wstring housing(housingBuff);
			std::wstring flat(flatBuff);

			Record r(&tel, &fname, &mname, &lname, &street, std::stoi(house), std::stoi(housing), std::stoi(flat));
			UpdateList(pSelf->hListView, phoneBook.search(&r));
		}
		break;
	case WM_DESTROY:
		phoneBook.clear();
		HeapFree(GetProcessHeap(), 0, pSelf);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int CALLBACK wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {

	const wchar_t* className = L"MainWindowClass";
	const wchar_t* title = L"PhoneBook";

	hInst = hInstance;

	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof icc;
	icc.dwICC =  ICC_COOL_CLASSES | ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icc);

	WNDCLASSEX wcex = { };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	//self pointer memory reservation
	wcex.cbWndExtra = 4;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = className;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, _T("Failed to register main class"), title, NULL);
		return -1;
	}
	HWND hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
		className, title, WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

	if (!hWnd) {

		HRESULT tmp = HRESULT_FROM_WIN32(GetLastError());
		MessageBox(NULL, _T("Failed to create main window"), title, NULL);
		return -1;
	}


	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		int tmp = CommDlgExtendedError();
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;

}