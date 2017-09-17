#include "Win32Entity.h"

namespace Y3D
{
	Win32Entity::Win32Entity()
		: m_hWnd(nullptr)
		, m_nClientWidth(0)
		, m_nClientHeight(0)
		, m_nWindowWidth(0)
		, m_nWindowHeight(0)
		, m_nStyle(0) {}

	Win32Entity::~Win32Entity()
	{
		//
	}

	bool Win32Entity::Create(LPCTSTR pszTitle, LPCTSTR pszClassName, HINSTANCE hInstance, UINT32 nX, UINT32 nY, UINT32 nWidth, UINT32 nHeight, DWORD nStyle)
	{
		m_hWnd = CreateWindow(pszClassName, pszTitle, nStyle, nX, nY, nWidth, nHeight, nullptr, nullptr, hInstance, nullptr);
		if (!m_hWnd) return false;

		m_nStyle = nStyle;

		SetWidthAndHeight(nWidth, nHeight);

		return true;
	}

	void Win32Entity::Show(INT32 const nCmdShow)
	{
		ShowWindow(m_hWnd, nCmdShow);
		UpdateWindow(m_hWnd);
	}

	LRESULT Win32Entity::OnWndProc(void* pGame, HWND hWnd, UINT32 message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
			{
			m_hWnd = nullptr;
			m_nClientWidth = 0;
			m_nClientHeight = 0;
			m_nWindowWidth = 0;
			m_nWindowHeight = 0;
			m_nStyle = 0;
			PostQuitMessage(0);
			return 0;
			}
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	void* Win32Entity::GetWinHandle()
	{
		return static_cast<void*>(m_hWnd);
	}

	INT32 Win32Entity::GetMouseX()
	{
		POINT CursorPos;
		GetCursorPosClient(CursorPos);
		return CursorPos.x;
	}

	INT32 Win32Entity::GetMouseY()
	{
		POINT CursorPos;
		GetCursorPosClient(CursorPos);
		return CursorPos.y;
	}

	bool Win32Entity::IsFullScreen()
	{
		return false;
	}
	void Win32Entity::SetWidthAndHeight(INT32 width, INT32 height)
	{
		m_nWindowWidth = width;
		m_nWindowHeight = height;

		RECT RectClient;
		GetClientRect(m_hWnd, &RectClient);
		m_nClientWidth = RectClient.right - RectClient.left;
		m_nClientHeight = RectClient.bottom - RectClient.top;
	}

	UINT32 Win32Entity::GetWidth()
	{
		return m_nWindowWidth;
	}

	UINT32 Win32Entity::GetHeight()
	{
		return m_nWindowHeight;
	}

	UINT32 Win32Entity::GetClientWidth()
	{
		return m_nClientWidth;
	}

	UINT32 Win32Entity::GetClientHeight()
	{
		return m_nClientHeight;
	}

	FLOAT32 Win32Entity::GetWindowScale()
	{
		return 1.f;
	}

	void Win32Entity::GetCursorPosClient(POINT& ClientPos)
	{
		GetCursorPos(&ClientPos);
		ScreenToClient(m_hWnd, &ClientPos);
	}
}

