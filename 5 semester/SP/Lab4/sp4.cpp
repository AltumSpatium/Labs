#include <windows.h>
#include <tchar.h>
#include "resource.h"

#define TIMER 101

HINSTANCE hInst;

static int x = 0, y = 0;
static int x_dest = 0, y_dest = 0;
static int count = 0;

static bool cState = true;
static bool toR = true;

void Timer(HWND hWnd)
{
	count++;
	if (x != x_dest)
	{
		toR = x > x_dest ? false : true;
		x = x > x_dest ? x - 1 : x + 1;
	}

	if (y != y_dest)
		y = y > y_dest ? y - 1 : y + 1;

	cState = (x == x_dest && y == y_dest) ? true : false;

	InvalidateRect(hWnd, NULL, TRUE);
}

void DrawAnimation(HWND hWnd)
{
	HDC hdc, hCmpDC;
	PAINTSTRUCT ps;
	RECT rect;
	HBITMAP hbm;

	int mBmp[] = { ID_MAN1, ID_MAN2 };
	int mBmpR[] = { ID_MAN1R, ID_MAN2R };
	int index;

	rect.left = rect.top = 0;
	rect.right = rect.bottom = 50;

	hdc = BeginPaint(hWnd, &ps);
	hCmpDC = CreateCompatibleDC(hdc);

	index = (count / 4) % 2;
	hbm = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCE(!cState ? (toR ? mBmp[index] : mBmpR[index]) : (toR ? mBmp[1] : mBmpR[1])),
		IMAGE_BITMAP, 0, 0, NULL);
	SelectObject(hCmpDC, hbm);

	BitBlt(hdc, x, y, rect.right - rect.left,
		rect.bottom - rect.top,
		hCmpDC, 0, 0, SRCCOPY);

	DeleteDC(hCmpDC);
	DeleteObject(hbm);

	EndPaint(hWnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		SetTimer(hWnd, TIMER, 50, NULL);
		break;
	case WM_TIMER:
		Timer(hWnd);
		break;
	case WM_PAINT:
		DrawAnimation(hWnd);
		break;
	case WM_LBUTTONDOWN:
		x_dest = LOWORD(lParam);
		y_dest = HIWORD(lParam);
		break;
	case WM_DESTROY:
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

	wcex.lpszClassName = _T("SP4");
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

	HWND hWnd = CreateWindow(_T("SP4"),
		_T("SP. Lab #4"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1000, 600, NULL, NULL,
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