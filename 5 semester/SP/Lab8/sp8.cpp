#include <windows.h>
#include <tchar.h>
#include <vector>
#include <tlhelp32.h>

using namespace std;

#define ID_LISTBOX_PROC 1001
#define ID_LISTBOX_MOD 1002

#define ID_MENUITEM1 2001
#define ID_MENUITEM2 2002

HINSTANCE hInst;
HWND hListboxProc, hListboxMod;
HMENU hMenu;
HANDLE hProcess;
DWORD value = -1;

vector<PROCESSENTRY32> vProc;

string str;
static int index;

void CreateLB(HWND hWnd)
{
	hListboxProc = CreateWindow(_T("LISTBOX"), _T(""),
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
		15, 25, 355, 120, hWnd, (HMENU)ID_LISTBOX_PROC, hInst, NULL);
	hListboxMod = CreateWindow(_T("LISTBOX"), _T(""),
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
		15, 150, 300, 100, hWnd, (HMENU)ID_LISTBOX_MOD, hInst, NULL);
}

void StrForID(DWORD id)
{
	switch (id)
	{
	case IDLE_PRIORITY_CLASS:
		str = "IDLE_PRIORITY_CLASS";
		break;
	case NORMAL_PRIORITY_CLASS:
		str = "NORMAL_PRIORITY_CLASS";
		break;
	case HIGH_PRIORITY_CLASS:
		str = "HIGH_PRIORITY_CLASS";
		break;
	case REALTIME_PRIORITY_CLASS:
		str = "REALTIME_PRIORITY_CLASS";
		break;
	}
}

void ListProcesses()
{
	HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe;
	WCHAR buf[1024];

	if (hSnapshot == INVALID_HANDLE_VALUE) return;

	vProc.clear();
	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapshot, &pe);

	do
	{
		vProc.push_back(pe);

		int pos = 0, ind = 0;
		while (pe.szExeFile[pos] != '\0')
			buf[ind++] = pe.szExeFile[pos++];
		pos = 0;

		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);
		buf[ind++] = WCHAR(' '), buf[ind++] = WCHAR('-'), buf[ind++] = WCHAR('>'), buf[ind++] = WCHAR(' ');
		StrForID(GetPriorityClass(hProcess));
		while (str[pos] != '\0')
			buf[ind++] = WCHAR(str[pos++]);
		buf[ind] = '\0';

		SendMessage(hListboxProc, LB_ADDSTRING, 0, (LPARAM)buf);
	} while (Process32Next(hSnapshot, &pe));

	CloseHandle(hSnapshot);
}

void ListModules(DWORD CONST dwProcessID)
{
	HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessID);
	MODULEENTRY32 me;
	TCHAR buf[1024];

	SendMessage(hListboxMod, LB_RESETCONTENT, 0, 0);

	if (hSnapshot == INVALID_HANDLE_VALUE) return;

	me.dwSize = sizeof(MODULEENTRY32);
	Module32First(hSnapshot, &me);
	do
	{
		SendMessage(hListboxMod, LB_ADDSTRING, 0, (LPARAM)me.szModule);
	} while (Module32Next(hSnapshot, &me));

	CloseHandle(hSnapshot);
}

void OpenMenu(HWND hWnd, LPARAM lParam)
{
	if ((index = SendMessage(hListboxProc, LB_GETCURSEL, 0, 0)) != LB_ERR)
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, vProc[index].th32ProcessID);
		value = GetPriorityClass(hProcess);
	}

	hMenu = CreatePopupMenu();

	AppendMenu(hMenu, value == IDLE_PRIORITY_CLASS ? MF_DISABLED : 0, IDLE_PRIORITY_CLASS, L"IDLE_PRIORITY_CLASS");
	AppendMenu(hMenu, value == NORMAL_PRIORITY_CLASS ? MF_DISABLED : 0, NORMAL_PRIORITY_CLASS, L"NORMAL_PRIORITY_CLASS");
	AppendMenu(hMenu, value == HIGH_PRIORITY_CLASS ? MF_DISABLED : 0, HIGH_PRIORITY_CLASS, L"HIGH_PRIORITY_CLASS");
	AppendMenu(hMenu, value == REALTIME_PRIORITY_CLASS ? MF_DISABLED : 0, REALTIME_PRIORITY_CLASS, L"REALTIME_PRIORITY_CLASS");

	TrackPopupMenu(hMenu, TPM_LEFTALIGN, LOWORD(lParam), HIWORD(lParam), 0, hWnd, NULL);
	value = -1;
}

void SetPriority(DWORD priority)
{
	if (priority != IDLE_PRIORITY_CLASS && priority != NORMAL_PRIORITY_CLASS &&
		priority != HIGH_PRIORITY_CLASS && priority != REALTIME_PRIORITY_CLASS)
		return;
	SetPriorityClass(hProcess, priority);
	SendMessage(hListboxProc, LB_RESETCONTENT, 0, 0);
	ListProcesses();
	SendMessage(hListboxMod, LB_RESETCONTENT, 0, 0);
}

void ProcessCommand(WPARAM wParam)
{
	if ((index = SendMessage(hListboxProc, LB_GETCURSEL, 0, 0)) != LB_ERR)
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, vProc[index].th32ProcessID);
		ListModules(vProc[index].th32ProcessID);
	}
	else return;

	SetPriority(LOWORD(wParam));
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		CreateLB(hWnd);
		ListProcesses();
		SendMessage(hListboxProc, LB_RESETCONTENT, 0, 0);
		ListProcesses();
		break;
	case WM_CONTEXTMENU:
		OpenMenu(hWnd, lParam);
		break;
	case WM_COMMAND:
		ProcessCommand(wParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	hInst = hInstance;

	WNDCLASSEX wcex;

	wcex.lpszClassName = _T("SP8");
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wcex);

	HWND hWnd = CreateWindow(_T("SP8"),
		_T("SP. Lab #8"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		400, 400, NULL, NULL,
		hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}