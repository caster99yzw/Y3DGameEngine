#include "D3DHelper.h"
#include <comdef.h>
#include <corecrt_wstdio.h>

using namespace Y3D;

std::wstring Y3D::TextFromHResult(HRESULT hr)
{
	_com_error error(hr);
	wchar_t msg[1024];
	//swprintf_s(msg, L"Error 0x%08x: %s", hr, error.ErrorMessage());
	return msg;
}