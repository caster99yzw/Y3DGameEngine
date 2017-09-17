#pragma once
#include "WindowBase.h"
#include <Windows.h>

namespace Y3D
{
	class Win32Entity : public WindowBase
	{
	public:
						Win32Entity();
						~Win32Entity() override;

	private:

		HWND			m_hWnd;

		/** Width and height of render window's frame */
		INT32			m_nClientWidth;
		INT32			m_nClientHeight;

		/** Width and height of client window's frame, including toolbar and title */
		INT32			m_nWindowWidth;
		INT32			m_nWindowHeight;

		DWORD			m_nStyle;

	public:

		bool			Create(LPCTSTR pszTitle, LPCTSTR pszClassName, HINSTANCE hInstance, UINT32 nX, UINT32 ny, UINT32 nWidth, UINT32 nHeight, DWORD nStyle);

		void			Show(INT32 const nCmdShow);

		void*			GetWinHandle() override;

		INT32			GetMouseX() override;
		INT32			GetMouseY() override;
		bool			IsFullScreen() override;
		void			SetWidthAndHeight(INT32 width, INT32 height) override;

		UINT32			GetWidth() override;
		UINT32			GetHeight() override;

		UINT32			GetClientWidth();
		UINT32			GetClientHeight();
		
		FLOAT32			GetWindowScale();

		virtual LRESULT	OnWndProc(void* pGame, HWND hWnd, UINT32 message, WPARAM wParam, LPARAM lParam);

	private:

		void			GetCursorPosClient(POINT& ClientPos);
	};
}