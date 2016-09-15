#include <windows.h>
#include <tchar.h>
#include "resource.h"

#define ID_BUTTON_DRAW 1001
#define ID_BUTTON_CLEAR 1002

HINSTANCE hInst;
HWND hBtnDraw, hBtnClear;

static bool isDraw = false;

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hbm)
{
	POINT ptSize, ptOrg;
	HBITMAP hOldbm;
	BITMAP bm;
	HDC hMemDC = CreateCompatibleDC(hdc);

	if (hOldbm = (HBITMAP)SelectObject(hMemDC, hbm))
	{
		SetMapMode(hMemDC, GetMapMode(hdc));
		GetObject(hbm, sizeof(BITMAP), (LPSTR)&bm);

		ptSize.x = bm.bmWidth;
		ptSize.y = bm.bmHeight;
		ptOrg.x = 0;
		ptOrg.y = 0;

		DPtoLP(hdc, &ptSize, 1);
		DPtoLP(hMemDC, &ptOrg, 1);

		BitBlt(hdc, x, y,
			ptSize.x, ptSize.y,
			hMemDC, ptOrg.x,
			ptOrg.y, SRCCOPY);

		SelectObject(hMemDC, hOldbm);
	}

	DeleteDC(hMemDC);
}

void DrawButton(LPDRAWITEMSTRUCT lpInfo)
{
	HBITMAP hbm;
	int resID;
	
	if (lpInfo->CtlType != ODT_BUTTON)
		return;

	if ((resID = lpInfo->CtlID == ID_BUTTON_DRAW ? ID_BTN_DRAW : (lpInfo->CtlID == ID_BUTTON_CLEAR ? ID_BTN_CLEAR : -1)) == -1)
		return;

	if (lpInfo->itemState & ODS_SELECTED)
		resID = resID == ID_BTN_DRAW ? ID_BTN_DRAW_PRESSED : ID_BTN_CLEAR_PRESSED;

	if (!(hbm = LoadBitmap(hInst, MAKEINTRESOURCE(resID))))
		return;

	if ((lpInfo->itemAction & ODA_DRAWENTIRE) ||
		(lpInfo->itemAction & ODA_SELECT))
		DrawBitmap(lpInfo->hDC, lpInfo->rcItem.left, lpInfo->rcItem.top, hbm);
}

void DrawPicture(HWND hWnd)
{
	PAINTSTRUCT ps;
	RECT rect = { 0, 0, 1000, 600 };
	HBRUSH hWhiteBrush = CreateSolidBrush(RGB(255, 255, 255));
	HDC hdc = BeginPaint(hWnd, &ps);

	if (isDraw)
	{
		//Sky
		SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(186, 225, 252)));
		SelectObject(hdc, CreateSolidBrush(RGB(186, 225, 252)));
		Rectangle(hdc, 100, 0, 1000, 320);

		//Grass
		SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(55, 137, 61)));
		SelectObject(hdc, CreateSolidBrush(RGB(55, 137, 61)));
		Rectangle(hdc, 100, 320, 1000, 600);

		//Rails
		SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(67, 30, 30)));
		SelectObject(hdc, CreateSolidBrush(RGB(67, 30, 30)));
		Rectangle(hdc, 100, 310, 1000, 320);

		//Sun
		SelectObject(hdc, CreatePen(PS_SOLID, 7, RGB(255, 255, 0)));
		SelectObject(hdc, CreateSolidBrush(RGB(255, 255, 0)));
		Ellipse(hdc, 100, 0, 200, 100);
		MoveToEx(hdc, 150, 100, NULL);
		LineTo(hdc, 150, 150);
		MoveToEx(hdc, 200, 50, NULL);
		LineTo(hdc, 250, 50);
		MoveToEx(hdc, 100, 0, NULL);
		LineTo(hdc, 230, 130);
		MoveToEx(hdc, 200, 0, NULL);
		LineTo(hdc, 100, 100);

		//Wires
		SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));
		MoveToEx(hdc, 100, 170, NULL);
		LineTo(hdc, 1000, 170);

		//Wheels
		SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(146, 143, 143)));
		SelectObject(hdc, CreateSolidBrush(RGB(146, 143, 143)));
		for (int i = 300; i < 701; i += 100) Ellipse(hdc, i, 280, i + 40, 310);

		//Train
		SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(84, 82, 85)));
		SelectObject(hdc, CreateSolidBrush(RGB(84, 82, 85)));
		Rectangle(hdc, 290, 200, 750, 295);

		//Windows
		SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(67, 129, 252)));
		SelectObject(hdc, CreateSolidBrush(RGB(67, 129, 252)));
		for (int i = 315; i < 716; i += 75) Rectangle(hdc, i, 230, i + 40, 260);

		//Pantograph
		SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(0, 0, 0)));
		POINT pantograph[6] = { { 510, 200 },{ 510, 190 },{ 500, 180 },{ 510, 170 },{ 520, 180 },{ 510, 190 } };
		Polyline(hdc, pantograph, 6);
	}
	else FillRect(hdc, &rect, hWhiteBrush);

	EndPaint(hWnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		hBtnDraw = CreateWindow(_T("BUTTON"), _T(""),
			WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
			20, 20, 48, 48, hWnd, (HMENU)ID_BUTTON_DRAW,
			hInst, NULL);

		hBtnClear = CreateWindow(_T("BUTTON"), _T(""),
			WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
			20, 70, 48, 48, hWnd, (HMENU)ID_BUTTON_CLEAR,
			hInst, NULL);
		break;
	case WM_DRAWITEM:
		DrawButton((LPDRAWITEMSTRUCT)lParam);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_BUTTON_DRAW:
			isDraw = true;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_BUTTON_CLEAR:
			isDraw = false;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		DrawPicture(hWnd);
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

	wcex.lpszClassName = _T("SP3");
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

	HWND hWnd = CreateWindow(_T("SP3"),
		_T("SP. Lab #3"),
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