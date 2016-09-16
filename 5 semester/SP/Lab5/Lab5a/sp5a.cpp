#include <windows.h>
#include <tchar.h>

#define ID_RADIO_RED 101
#define ID_RADIO_BLUE 102
#define ID_RADIO_GREEN 103

#define ID_RADIO_SQUARE 201
#define ID_RADIO_CIRCLE 202
#define ID_RADIO_STAR 203
#define ID_RADIO_RHOMBUS 204

#define ID_CHECKBOX 301

HINSTANCE hInst;
HWND hRadioRed, hRadioBlue, hRadioGreen;
HWND hRadioSquare, hRadioCircle, hRadioStar, hRadioRhombus;
HWND hCheckbox;

UINT msgColor, msgFigure, msgDraw;

static bool checked = true;

void CreateButtons(HWND hWnd)
{
	hRadioRed = CreateWindow(_T("BUTTON"), _T("Red"),
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		10, 10, 100, 20, hWnd, (HMENU)ID_RADIO_RED,
		hInst, NULL);

	hRadioBlue = CreateWindow(_T("BUTTON"), _T("Blue"),
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		10, 30, 100, 20, hWnd, (HMENU)ID_RADIO_BLUE,
		hInst, NULL);

	hRadioGreen = CreateWindow(_T("BUTTON"), _T("Green"),
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		10, 50, 100, 20, hWnd, (HMENU)ID_RADIO_GREEN,
		hInst, NULL);

	hRadioSquare = CreateWindow(_T("BUTTON"), _T("Square"),
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		120, 10, 100, 20, hWnd, (HMENU)ID_RADIO_SQUARE,
		hInst, NULL);

	hRadioCircle = CreateWindow(_T("BUTTON"), _T("Circle"),
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		120, 30, 100, 20, hWnd, (HMENU)ID_RADIO_CIRCLE,
		hInst, NULL);

	hRadioStar = CreateWindow(_T("BUTTON"), _T("Star"),
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		120, 50, 100, 20, hWnd, (HMENU)ID_RADIO_STAR,
		hInst, NULL);

	hRadioRhombus = CreateWindow(_T("BUTTON"), _T("Rhombus"),
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		120, 70, 100, 20, hWnd, (HMENU)ID_RADIO_RHOMBUS,
		hInst, NULL);

	hCheckbox = CreateWindow(_T("BUTTON"), _T("Draw"),
		WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
		230, 10, 100, 20, hWnd, (HMENU)ID_CHECKBOX,
		hInst, NULL);

	SendMessage(hRadioRed, BM_SETCHECK, true, 0);
	SendMessage(hRadioSquare, BM_SETCHECK, true, 0);
	SendMessage(hCheckbox, BM_SETCHECK, true, 0);

	SendMessage(HWND_BROADCAST, msgColor, 0, 1);
	SendMessage(HWND_BROADCAST, msgFigure, 0, 1);
	SendMessage(HWND_BROADCAST, msgDraw, 0, 1);
}

void SendCommands(UINT param)
{
	if (param == ID_RADIO_RED || param == ID_RADIO_BLUE || param == ID_RADIO_GREEN)
	{
		SendMessage(hRadioRed, BM_SETCHECK, false, 0);
		SendMessage(hRadioBlue, BM_SETCHECK, false, 0);
		SendMessage(hRadioGreen, BM_SETCHECK, false, 0);

		int n = param == ID_RADIO_RED ? 1 : (param == ID_RADIO_BLUE ? 2 : 3);
		HWND hRadio = param == ID_RADIO_RED ? hRadioRed : (param == ID_RADIO_BLUE ? hRadioBlue : hRadioGreen);

		SendMessage(hRadio, BM_SETCHECK, true, 0);
		SendMessage(HWND_BROADCAST, msgColor, 0, n);
	}
	else if (param == ID_RADIO_SQUARE || param == ID_RADIO_CIRCLE ||
		param == ID_RADIO_STAR || param == ID_RADIO_RHOMBUS)
	{
		SendMessage(hRadioSquare, BM_SETCHECK, false, 0);
		SendMessage(hRadioCircle, BM_SETCHECK, false, 0);
		SendMessage(hRadioStar, BM_SETCHECK, false, 0);
		SendMessage(hRadioRhombus, BM_SETCHECK, false, 0);

		int n = param == ID_RADIO_SQUARE ? 1 :
			(param == ID_RADIO_CIRCLE ? 2 :
			(param == ID_RADIO_STAR ? 3 : 4));
		HWND hRadio = param == ID_RADIO_SQUARE ? hRadioSquare :
			(param == ID_RADIO_CIRCLE ? hRadioCircle :
			(param == ID_RADIO_STAR ? hRadioStar : hRadioRhombus));

		SendMessage(hRadio, BM_SETCHECK, true, 0);
		SendMessage(HWND_BROADCAST, msgFigure, 0, n);
	}
	else if (param == ID_CHECKBOX)
	{
		SendMessage(HWND_BROADCAST, msgDraw, 0, checked ? 0 : 1);
		checked = !checked;
		SendMessage(hCheckbox, BM_SETCHECK, checked, 0);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		CreateButtons(hWnd);
		break;
	case WM_COMMAND:
		SendCommands(LOWORD(wParam));
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

	wcex.lpszClassName = _T("SP5a");
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

	HWND hWnd = CreateWindow(_T("SP5a"),
		_T("SP. Lab #5a"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		360, 140, NULL, NULL,
		hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	msgColor = RegisterWindowMessage(L"MessageColor");
	msgFigure = RegisterWindowMessage(L"MessageFigure");
	msgDraw = RegisterWindowMessage(L"MessageDraw");

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}