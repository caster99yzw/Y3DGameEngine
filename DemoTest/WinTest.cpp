#include "Window/Win32Entity.h"

using namespace Y3D;

#define MAX_LOADSTRING 100

/** Global variables */
HINSTANCE			hInst;
TCHAR const			szWindowTitle[] = "windowDemo";
TCHAR const			szClassName[] = "windowClass";

Win32Entity			entity;

/** Forward declarations of functions*/
ATOM				RegisterWindowClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT32, WPARAM, LPARAM);



int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	MSG msg = { nullptr };

	if (!RegisterWindowClass(hInstance))
	{
		return -1;
	}

	if (!InitInstance(hInstance, nCmdShow))
	{
		return -1;
	}

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

ATOM RegisterWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= nullptr;
	wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= nullptr;
	wcex.lpszClassName	= szClassName;
	wcex.hIconSm		= nullptr;

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	RECT WindowRect;
	WindowRect.left = 0;
	WindowRect.top = 0;
	WindowRect.right = 1659;
	WindowRect.bottom = 1050;

	DWORD const nWindowStyle = WS_OVERLAPPEDWINDOW;
	AdjustWindowRect(&WindowRect, nWindowStyle, false);

	UINT32 const nWindowWidth = WindowRect.right - WindowRect.left;
	UINT32 const nWindowHeight = WindowRect.bottom - WindowRect.top;

	if (!entity.Create(szWindowTitle, szClassName, hInstance, 
		CW_USEDEFAULT, CW_USEDEFAULT, nWindowWidth, nWindowHeight, nWindowStyle))
	{
		return FALSE;
	}

	entity.Show(nCmdShow);

	return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT32 message, WPARAM wParam, LPARAM lParam)
{
	return entity.OnWndProc(nullptr, hWnd, message, wParam, lParam);
}
