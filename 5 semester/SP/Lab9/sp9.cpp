#include <windows.h>
#include <tchar.h>
#include <regex>

#define ID_LISTBOX 1001

#define ID_EDIT 2001

#define ID_BTN_SEARCH 3001
#define ID_BTN_CLEAR 3002

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

#define TIMER 3001

using namespace std;

HINSTANCE hInst;
HWND hListbox;
HWND hEdit;
HWND hBtnSearch, hBtnClear;
HANDLE hBgThread;
TCHAR bufExpr[128];

static bool isThreadFinished = false;

void CreateWindowElements(HWND hWnd)
{
	hListbox = CreateWindow(_T("LISTBOX"), NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL,
		10, 10, 700, 400, hWnd, (HMENU)ID_LISTBOX,
		hInst, NULL);

	hEdit = CreateWindow(_T("EDIT"), _T(""),
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		10, 420, 450, 25, hWnd, (HMENU)ID_EDIT,
		hInst, NULL);

	hBtnSearch = CreateWindow(_T("BUTTON"), _T("Search"),
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		470, 420, 100, 25, hWnd, (HMENU)ID_BTN_SEARCH,
		hInst, NULL);

	hBtnClear = CreateWindow(_T("BUTTON"), _T("Clear"),
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		580, 420, 100, 25, hWnd, (HMENU)ID_BTN_CLEAR,
		hInst, NULL);
}

void PrintResult(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	if (isThreadFinished)
		TextOut(hdc, 315, 450, _T("Search finished!"), 16);

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
		DWORD ind = 0, type = 0;
		TCHAR valueBuf[MAX_KEY_LENGTH];
		BYTE value[MAX_KEY_LENGTH];

		while (RegEnumValue(hSubKey, ind++, valueBuf,
			&valueLength, NULL, &type, value, &valLength)
			== ERROR_SUCCESS)
		{
			TCHAR* chars = (TCHAR*)value;
			wstring s1(&chars[0]);
			wstring s2(bufExpr);

			string str(s1.begin(), s1.end());
			regex reg(string(s2.begin(), s2.end()));

			if (regex_match(str, reg) && !s1.empty())
				SendDlgItemMessage(hWnd, ID_LISTBOX, LB_SETITEMDATA,
				(WPARAM)SendDlgItemMessage(hWnd, ID_LISTBOX, LB_ADDSTRING, 0, (LPARAM)chars),
					(LPARAM)chars);

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

bool ReadExpression(HWND hWnd)
{
	GetDlgItemText(hWnd, ID_EDIT, bufExpr, 128);
	SetDlgItemText(hWnd, ID_EDIT, (LPWSTR)"");

	bool isEmpty = true;
	for (int i = 0; bufExpr[i] != '\0'; i++)
	{
		if (bufExpr[i] != ' ')
		{
			isEmpty = false;
			break;
		}
	}

	return !isEmpty;
}

void ProcessCommand(HWND hWnd, WPARAM wParam)
{
	switch (LOWORD(wParam))
	{
	case ID_BTN_SEARCH:
		if (ReadExpression(hWnd))
		{
			if ((hBgThread = CreateThread(NULL, 0, &ThreadProc, hWnd, 0, NULL)) == NULL)
				MessageBox(hWnd, _T("Error opening search thread!"), MB_OK, NULL);
			isThreadFinished = false;
			SetTimer(hWnd, TIMER, 500, NULL);
		}
		break;
	case ID_BTN_CLEAR:
		if (hBgThread != nullptr)
			TerminateThread(hBgThread, 0);
		int count = SendDlgItemMessage(hWnd, ID_LISTBOX, LB_GETCOUNT, NULL, NULL);
		for (int i = 0; i < count; i++)
			SendDlgItemMessage(hWnd, ID_LISTBOX, LB_DELETESTRING, (WPARAM)0, NULL);
		isThreadFinished = false;
		InvalidateRect(hWnd, 0, true);
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
		PrintResult(hWnd);
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