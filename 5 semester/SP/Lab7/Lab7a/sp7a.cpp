#include <windows.h>
#include <tchar.h>

HINSTANCE hInst;
HANDLE hMutex, hThread[3];

typedef struct clr
{
	HWND hWnd;
	int i;
	clr() {};
	clr(HWND hWnd, int i) : hWnd(hWnd), i(i) {};
} WNDCOLOR;

WNDCOLOR colors[3];

DWORD WINAPI ThreadFunc(LPVOID tParam)
{
	WNDCOLOR* color = (WNDCOLOR*)tParam;
	while (true)
	{
		WaitForSingleObject(hMutex, INFINITE);
		HBRUSH hBrush = CreateSolidBrush(
			 color->i == 0 ? RGB(248, 255, 16) :
			(color->i == 1 ? RGB(47, 110, 255) :
							 RGB(0, 0, 0)));
		SetClassLong(color->hWnd, GCL_HBRBACKGROUND, (LONG)hBrush);
		InvalidateRect(color->hWnd, NULL, TRUE);
		Sleep(1000);
		ReleaseMutex(hMutex);
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		WaitForMultipleObjects(3, hThread, TRUE, INFINITE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	hInst = hInstance;

	WNDCLASSEX wcex;

	wcex.lpszClassName = _T("SP7a");
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

	HWND hWnd = CreateWindow(_T("SP7a"),
		_T("SP. Lab #7a"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		400, 400, NULL, NULL,
		hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	for (int i = 0; i < 3; i++)
	{
		colors[i] = WNDCOLOR(hWnd, i);
		hThread[i] = CreateThread(NULL, 0, ThreadFunc, &colors[i], 0, NULL);
	}

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}