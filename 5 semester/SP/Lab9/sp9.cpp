#include <windows.h>
#include <tchar.h>

#define ID_LISTBOX 1001

#define ID_BTN_SEARCH 2001
#define ID_BTN_CLEAR 2002
#define ID_BTN_PAUSE 2003

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

#define TIMER 3001

HINSTANCE hInst;
HWND hListbox;
HWND hBtnSearch, hBtnClear, hBtnPause;
HANDLE hBgThread;
TCHAR pathTemplate[3] = L":\\";

static int count = 0;
static bool isThreadFinished = false, ThreadSuspended = false;

void CreateWindowElements(HWND hWnd)
{
	hListbox = CreateWindow(_T("LISTBOX"), NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL,
		10, 10, 700, 400, hWnd, (HMENU)ID_LISTBOX,
		hInst, NULL);

	hBtnSearch = CreateWindow(_T("BUTTON"), _T("Search"),
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		10, 420, 100, 25, hWnd, (HMENU)ID_BTN_SEARCH,
		hInst, NULL);

	hBtnClear = CreateWindow(_T("BUTTON"), _T("Clear"),
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		120, 420, 100, 25, hWnd, (HMENU)ID_BTN_CLEAR,
		hInst, NULL);

	hBtnPause = CreateWindow(_T("BUTTON"), _T("Pause"),
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		230, 420, 100, 25, hWnd, (HMENU)ID_BTN_PAUSE,
		hInst, NULL);
}

void PrintResultText(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	if (isThreadFinished)
		TextOut(hdc, 250, 420, _T("Search finished!"), 16);

	EndPaint(hWnd, &ps);
}

void GetKey(HWND hWnd, HKEY hPrevKey, LPCWSTR subKey)
{
	HKEY hSubKey;
	DWORD index = 0, length = MAX_KEY_LENGTH;
	TCHAR buf[MAX_KEY_LENGTH];

	if (RegOpenKeyEx(hPrevKey, subKey, 0, KEY_ALL_ACCESS, &hSubKey) == ERROR_SUCCESS)
	{
		DWORD locLength = MAX_KEY_LENGTH;
		TCHAR locBuf[MAX_KEY_LENGTH];

		while (RegEnumKeyEx(hSubKey, index++, locBuf, &locLength,
			NULL, NULL, NULL, NULL) != ERROR_NO_MORE_ITEMS)
		{
			GetKey(hWnd, hSubKey, locBuf);
			locLength = MAX_KEY_LENGTH;
		}

		DWORD valueLength = MAX_KEY_LENGTH;
		DWORD valLength = MAX_KEY_LENGTH;
		DWORD index = 0, type = 0;
		TCHAR valueBuf[MAX_KEY_LENGTH];
		BYTE value[MAX_KEY_LENGTH];

		while (RegEnumValue(hSubKey, index++, valueBuf,
			&valueLength, NULL, &type, value, &valLength)
			== ERROR_SUCCESS)
		{
			TCHAR* chars = (TCHAR*)value;

			if ((type == REG_SZ || type == REG_EXPAND_SZ ||
				type == REG_MULTI_SZ) && valLength > 3)
			{
				if (chars[1] == pathTemplate[0] && chars[2] == pathTemplate[1])
				{
					for (int i = 3; i < valLength; i++)
						if (chars[i] == ' ' || chars[i] == ',' || chars[i] == '\\')
							if (i - 4 >= 0)
								if (chars[i - 4] == '.' && chars[i - 3] != '.')
								{
									chars[i] = '\0';
									break;
								}

					if (GetFileAttributes(chars) == INVALID_FILE_ATTRIBUTES)
						SendDlgItemMessage(hWnd, ID_LISTBOX, LB_SETITEMDATA,
							(WPARAM)SendDlgItemMessage(hWnd, ID_LISTBOX, LB_ADDSTRING, 0, (LPARAM)chars),
							(LPARAM)chars);
				}
			}

			valLength = MAX_KEY_LENGTH;
		}

		RegCloseKey(hSubKey);
	}
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	GetKey((HWND)lpParam, HKEY_LOCAL_MACHINE, NULL);
	isThreadFinished = true;
	return 0;
}

void ProcessCommand(HWND hWnd, WPARAM wParam)
{
	switch (LOWORD(wParam))
	{
	case ID_BTN_SEARCH:
		if ((hBgThread = CreateThread(NULL, 0, &ThreadProc, hWnd, 0, NULL)) == NULL)
			MessageBox(hWnd, _T("Error opening search thread!"), MB_OK, NULL);
		isThreadFinished = ThreadSuspended = false;
		SetTimer(hWnd, TIMER, 500, NULL);		
		break;
	case ID_BTN_CLEAR:
		if (hBgThread != nullptr)
			TerminateThread(hBgThread, 0);
		isThreadFinished = !(ThreadSuspended = true);
		count = SendDlgItemMessage(hWnd, ID_LISTBOX, LB_GETCOUNT, NULL, NULL);
		for (int i = 0; i < count; i++)
			SendDlgItemMessage(hWnd, ID_LISTBOX, LB_DELETESTRING, (WPARAM)0, NULL);
		ResumeThread(hBgThread);
		ThreadSuspended = false;
		break;
	case ID_BTN_PAUSE:
		if (hBgThread != nullptr)
		{
			ThreadSuspended ? SetWindowText(hBtnPause, _T("Pause")) :
				SetWindowText(hBtnPause, _T("Resume"));
			ThreadSuspended ? (ResumeThread(hBgThread), SetTimer(hWnd, TIMER, 500, 0)) :
				(SuspendThread(hBgThread), KillTimer(hWnd, TIMER));
			ThreadSuspended = !ThreadSuspended;
			InvalidateRect(hWnd, 0, 0);
		}
		break;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		CreateWindowElements(hWnd);
		break;
	case WM_PAINT:
		PrintResultText(hWnd);
		break;
	case WM_TIMER:
		if (isThreadFinished)
			InvalidateRect(hWnd, 0, true);
		break;
	case WM_COMMAND:
		ProcessCommand(hWnd, wParam);
		break;
	case WM_DESTROY:
		CloseHandle(hBgThread);
		TerminateThread(hBgThread, 0);
		KillTimer(hWnd, TIMER);
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

	wcex.lpszClassName = _T("SP9");
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

	HWND hWnd = CreateWindow(_T("SP9"),
		_T("SP. Lab #9"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		735, 510, NULL, NULL,
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