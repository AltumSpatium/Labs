#include <windows.h>
#include <tchar.h>
#include <string>

using namespace std;

HINSTANCE hInst;
UINT msgColor, msgFigure, msgDraw;

static string figure = "square";
static int red = 1, blue = 0, green = 0;
static int x = -1, y = -1;
static bool isDraw = true;

void GetWindowMessage(UINT msg, LPARAM lParam)
{
	if (msg == msgColor)
	{
		red = lParam == 1 ? 1 : 0;
		blue = lParam == 2 ? 1 : 0;
		green = lParam == 3 ? 1 : 0;
	}
	else if (msg == msgFigure)
		figure = lParam == 1 ? "square" :
		(lParam == 2 ? "circle" :
		(lParam == 3 ? "star" : "rhombus"));
	else if (msg == msgDraw)
		isDraw = lParam == 1 ? true : false;
}

void PaintFigure(HWND hWnd)
{
	HDC hdc;
	PAINTSTRUCT ps;
	POINT starPoints[] = {
		{ 0, 0 },{ 5, 20 },{ 25, 15 },{ 10, 30 },{ 15, 50 },
		{ 0, 35 },{ -15, 50 },{ -10, 30 },{ -25, 15 },{ -5, 20 } };
	POINT rhombusPoints[] = {
		{ 0, 0 },{ 20, 20 },{ 0, 20 },{ -20, 0 } };

	hdc = BeginPaint(hWnd, &ps);

	if (!isDraw || (x == -1 && y == -1)) return;

	SelectObject(hdc, CreateSolidBrush(RGB(red * 255, green * 255, blue * 255)));

	if (figure == "square")
		Rectangle(hdc, x, y, x + 40, y + 40);
	else if (figure == "circle")
		Ellipse(hdc, x - 20, y - 20, x + 20, y + 20);
	else if (figure == "star")
	{
		for (int i = 0; i < 10; i++)
			starPoints[i].x += x, starPoints[i].y += y;
		Polygon(hdc, starPoints, 10);
	}
	else if (figure == "rhombus")
	{
		for (int i = 0; i < 4; i++)
			rhombusPoints[i].x += x, rhombusPoints[i].y += y;
		Polygon(hdc, rhombusPoints, 4);
	}

	EndPaint(hWnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	GetWindowMessage(msg, lParam);

	switch (msg)
	{
	case WM_PAINT:
		PaintFigure(hWnd);
		break;
	case WM_LBUTTONDOWN:
		x = LOWORD(lParam), y = HIWORD(lParam);
		InvalidateRect(hWnd, NULL, TRUE);
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

	wcex.lpszClassName = _T("SP5b");
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

	HWND hWnd = CreateWindow(_T("SP5b"),
		_T("SP. Lab #5b"),
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