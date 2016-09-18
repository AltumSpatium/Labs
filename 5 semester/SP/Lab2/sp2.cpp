#include <windows.h>
#include <tchar.h>

#define ID_ADD_BTN 1001
#define ID_CLEAR_BTN 1002
#define ID_TORIGHT_BTN 1003
#define ID_DELETE_BTN 1004
#define ID_LISTBOX1 1005
#define ID_LISTBOX2 1006
#define ID_EDIT 1007

HINSTANCE hInst;
HWND hAddBtn, hClearBtn, hToRightBtn, hDeleteBtn;
HWND hListBox1, hListBox2;
HWND hEdit;

TCHAR buf[128];
int index = 0;

void AddBtn_Click(HWND hWnd)
{
	GetDlgItemText(hWnd, ID_EDIT, buf, 128);
	SetDlgItemText(hWnd, ID_EDIT, (LPWSTR)"");

	bool isEmpty = true;
	for (int i = 0; buf[i] != '\0'; i++)
	{
		if (buf[i] != ' ')
		{
			isEmpty = false;
			break;
		}
	}

	if (!isEmpty && SendMessage(hListBox1, LB_FINDSTRINGEXACT, 0, (LPARAM)buf) == LB_ERR)
		SendMessage(hListBox1, LB_ADDSTRING, 0, (LPARAM)buf);
}

void ClearBtn_Click()
{
	SendMessage(hListBox1, LB_RESETCONTENT, 0, 0);
	SendMessage(hListBox2, LB_RESETCONTENT, 0, 0);
}

void ToRightBtn_Click()
{
	if ((index = SendMessage(hListBox1, LB_GETCURSEL, 0, 0)) != LB_ERR)
	{
		SendMessage(hListBox1, LB_GETTEXT, index, (LPARAM)buf);
		SendMessage(hListBox1, LB_DELETESTRING, index, 0);
		SendMessage(hListBox2, LB_ADDSTRING, 0, (LPARAM)buf);
	}
}

void DeleteBtn_Click()
{
	if ((index = SendMessage(hListBox1, LB_GETCURSEL, 0, 0)) != LB_ERR)
		SendMessage(hListBox1, LB_DELETESTRING, index, 0);
	if ((index = SendMessage(hListBox2, LB_GETCURSEL, 0, 0)) != LB_ERR)
		SendMessage(hListBox2, LB_DELETESTRING, index, 0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		hAddBtn = CreateWindow(_T("BUTTON"), _T("Add"),
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			10, 10, 40, 20, hWnd, (HMENU)ID_ADD_BTN,
			hInst, NULL);

		hClearBtn = CreateWindow(_T("BUTTON"), _T("Clear"),
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			60, 10, 45, 20, hWnd, (HMENU)ID_CLEAR_BTN,
			hInst, NULL);

		hToRightBtn = CreateWindow(_T("BUTTON"), _T("To Right"),
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			115, 10, 65, 20, hWnd, (HMENU)ID_TORIGHT_BTN,
			hInst, NULL);

		hDeleteBtn = CreateWindow(_T("BUTTON"), _T("Delete"),
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			190, 10, 50, 20, hWnd, (HMENU)ID_DELETE_BTN,
			hInst, NULL);

		hListBox1 = CreateWindow(_T("LISTBOX"), _T(""),
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			10, 50, 80, 200, hWnd, (HMENU)ID_LISTBOX1,
			hInst, NULL);

		hListBox2 = CreateWindow(_T("LISTBOX"), _T(""),
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			100, 50, 80, 200, hWnd, (HMENU)ID_LISTBOX2,
			hInst, NULL);

		hEdit = CreateWindow(_T("EDIT"), _T(""),
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			190, 50, 60, 20, hWnd, (HMENU)ID_EDIT,
			hInst, NULL);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_ADD_BTN:
			AddBtn_Click(hWnd);
			break;
		case ID_CLEAR_BTN:
			ClearBtn_Click();
			break;
		case ID_TORIGHT_BTN:
			ToRightBtn_Click();
			break;
		case ID_DELETE_BTN:
			DeleteBtn_Click();
			break;
		}
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
	WNDCLASSEX wcex;

	wcex.lpszClassName = _T("SP2");
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

	HWND hWnd = CreateWindow(_T("SP2"),
		_T("SP. Lab #2"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		300, 300, NULL, NULL,
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