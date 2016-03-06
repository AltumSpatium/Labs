#include <windows.h>
#include <math.h>

#define PI 3.14159

const double Pi = 3.14159;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void Tree(HDC hdc, double x, double y, double l, double a) 
{          
	if (l > 1) 
	{
		l *= 0.7;
		MoveToEx(hdc, x, y, NULL);
		LineTo(hdc, (int)(x + l * cos(a)), (int)(y - l * sin(a)));
		x = x + l * cos(a);
		y = y - l * sin(a);

		Tree(hdc, x, y, l, a + Pi / 5);
		Tree(hdc, x, y, l, a - Pi / 5);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static char szAppName[] = "Lab7";
	HWND hwnd;
	MSG msg;
	WNDCLASSEX wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szAppName, "Lab7", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static int cxClient, cyClient;
	static HWND button;
	HDC hdc = GetDC(hwnd);
	int x = 0, y = 0;
	PAINTSTRUCT ps;

	switch (iMsg)
	{
	case WM_CREATE:

		button = CreateWindow("button", "Clear", WS_CHILD | WS_VISIBLE, 10, 10, 10 * 20, 10 + 20, hwnd, (HMENU)666, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		
		return 0;

	case WM_SIZE:

		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		return 0;

	case WM_PAINT:

		RECT rect;
		rect = { 0 };
		rect.bottom = cyClient;
		rect.right = cxClient;
		hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &rect, WHITE_BRUSH);
		EndPaint(hwnd, &ps);
		
		return 0;

	case WM_COMMAND:

		if (LOWORD(wParam) == 666)
		{
			RECT rect = { 0 };
			rect.bottom = cyClient;
			rect.right = cxClient;
			InvalidateRect(hwnd, &rect, true);			
			return 0;
		}

		break;

	case WM_LBUTTONUP:

		x = LOWORD(lParam);
		y = HIWORD(lParam);

		Tree(hdc, x, y, 200, Pi / 2);

		break;

	case WM_DESTROY:

		PostQuitMessage(0);

		return 0;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}