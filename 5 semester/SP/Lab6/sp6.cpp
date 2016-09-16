#include <windows.h>
#include <tchar.h>
#include <ctime>

#define ID_BTN_START 1001
#define ID_BTN_STOP 1002

HINSTANCE hInst;
HWND hBtnStart, hBtnStop;
HWND hEdits[3];
HANDLE hThread[3];

static bool isOn = false;

wchar_t* itolpcwstr(int num)
{
	wchar_t buffer[256];
	wsprintfW(buffer, L"%d", num);
	return buffer;
}

void GenerateNum(LPVOID tParam)
{
	HWND edit = (HWND)tParam;
	SetWindowText(edit, itolpcwstr(rand() + GetCurrentThreadId()));
}

DWORD WINAPI funThread(LPVOID tParam)
{
	HWND edit = (HWND)tParam;
	while (true)
	{
		GenerateNum(edit);
		Sleep(2000);
	}
	return 0;
}

void CreateWindowElements(HWND hWnd)
{
	hBtnStart = CreateWindow(_T("BUTTON"), _T("Start"),
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		20, 50, 70, 20, hWnd, (HMENU)ID_BTN_START,
		hInst, NULL);

	hBtnStop = CreateWindow(_T("BUTTON"), _T("Stop"),
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		110, 50, 70, 20, hWnd, (HMENU)ID_BTN_STOP,
		hInst, NULL);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		CreateWindowElements(hWnd);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_BTN_START:
			if (!isOn)
			{
				for (int i = 0; i < 3; i++)
					ResumeThread(hThread[i]);
				isOn = true;
			}
			break;
		case ID_BTN_STOP:
			if (isOn)
			{
				for (int i = 0; i < 3; i++)
					SuspendThread(hThread[i]);
				isOn = false;
			}
			break;
		}
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
	srand(time(0));
	hInst = hInstance;

	WNDCLASSEX wcex;

	wcex.lpszClassName = _T("SP6");
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

	HWND hWnd = CreateWindow(_T("SP6"),
		_T("SP. Lab #6"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		350, 150, NULL, NULL,
		hInstance, NULL);

	for (int i = 0; i < 3; i++)
		hEdits[i] = CreateWindow(_T("EDIT"), _T(""),
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			i * 100 + 20, 10, 90, 20, hWnd,
			(HMENU)101 + i, hInst, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	for (int i = 0; i < 3; i++)
	{
		hThread[i] = CreateThread(NULL, 0, funThread, hEdits[i], 0, NULL);
		SuspendThread(hThread[i]);
	}

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}