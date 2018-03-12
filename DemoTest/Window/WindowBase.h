#pragma once
#include "Y3DTypes.h"

namespace Y3D
{
	class WindowBase
	{
	public:

								WindowBase() = default;
		virtual					~WindowBase() {};

		virtual void*			GetWinHandle() = 0;
		
		virtual UINT32			GetWidth() = 0;
		virtual UINT32			GetHeight() = 0;
		virtual bool			IsFullScreen() = 0;

		virtual INT32			GetMouseX() = 0;
		virtual INT32			GetMouseY() = 0;

		virtual void			multisetWidthAndHeight(INT32 width, INT32 height) = 0;
	};
}