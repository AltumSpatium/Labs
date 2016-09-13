#include <windows.h>
#include <tchar.h>
#include "resource.h"

#define TOP 1001
#define BOTTOM 1002
#define LEFT 1003
#define RIGHT 1004

#define TIMER 2001
#define STEP 3

HINSTANCE hInst;
HWND label;

const int sWeight = 100;
const int sHeight = 20;

static int x_pos = 300;
static int y_pos = 200;
static int currentDirection = TOP;

void Move()
{
	if (currentDirection == TOP)
	{
		if (y_pos > 50) y_pos -= STEP;
		else currentDirection = RIGHT;
	}
	else if (currentDirection == RIGHT)
	{
		if (x_pos < 450) x_pos += STEP;
		else currentDirection = BOTTOM;
	}
	else if (currentDirection == BOTTOM)
	{
		if (y_pos < 350) y_pos += STEP;
		else currentDirection = LEFT;
	}
	else if (currentDirection == LEFT)
	{
		if (x_pos > 150) x_pos -= STEP;
		else currentDirection = TOP;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		label = CreateWindow(_T("STATIC"), NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			x_pos - sWeight / 2,
			y_pos - sHeight / 2,
			sWeight,
			sHeight,
			hWnd, NULL, hInst, NULL);
		SetWindowText(label, _T("MovingLabel!!!"));
		break;
	case WM_TIMER:
		Move();
		SetWindowPos(label, hWnd,
			x_pos - sWeight / 2,
			y_pos - sHeight / 2,
			sWeight, sHeight,
			SWP_NOSIZE | SWP_NOZORDER);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_START:
			SetTimer(hWnd, TIMER, 50, NULL);
			break;
		case ID_STOP:
			KillTimer(hWnd, TIMER);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hWnd, TIMER);
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

	WNDCLASS wc;

	memset(&wc, 0, sizeof(WNDCLASS));
	wc.lpszClassName = _T("SP1");
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = MAKEINTRESOURCEW(ID_MENU);
	wc.hIcon = LoadIconW(hInstance, MAKEINTRESOURCEW(ID_ICON));
	wc.hCursor = LoadIconA(hInstance, MAKEINTRESOURCEA(ID_CURSOR));
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wc);

	HWND hWnd = CreateWindow(_T("SP1"),
		_T("SP. Lab #1"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800, 500, NULL, NULL,
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