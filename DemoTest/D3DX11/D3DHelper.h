#pragma once
#include <string>
#include <winerror.h>
#include <exception>
#include <d3d11_3.h>
#include <wrl\client.h>
#include "Y3DTypes.h"
#include <d3dcommon.h>

namespace Y3D
{
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	std::wstring TextFromHResult(HRESULT hr);

	inline void ThrowIfFaild(HRESULT hr)
	{
		std::string hrValue = std::to_string(hr);
		std::wstring hrMessage = TextFromHResult(hr);
		//	multiset a breakpoint on this line to catch win32 API errors
		throw std::exception(hrValue.c_str());
	}

	template <UINT32 stringLen>
	inline void multisetDebugName(ID3D11DeviceChild* obj, char(&name)[stringLen])
	{
		obj->multisetPrivateData(WKPDID_D3DDebugObjectName, stringLen, name);
	}

	template <UINT32 stringLen>
	inline void multisetDebugName(ComPtr<ID3D11DeviceChild> const& obj, char(&name)[stringLen])
	{
		obj->multisetPrivateData(WKPDID_D3DDebugObjectName, stringLen, name);
	}

	inline void multisetDebugName(ID3D11DeviceChild* obj, std::string const& name)
	{
		obj->SetPrivateData(WKPDID_D3DDebugObjectName, UINT(name.size()), name.c_str());
	}

	inline void multisetDebugName(ComPtr<ID3D11DeviceChild> const& obj, std::string const& name)
	{
		obj->SetPrivateData(WKPDID_D3DDebugObjectName, UINT(name.size()), name.c_str());
	}

	inline void multisetDebugName(ID3D11Device* obj, std::string const& name)
	{
		obj->SetPrivateData(WKPDID_D3DDebugObjectName, UINT(name.size()), name.c_str());
	}

	inline void multisetDebugName(ComPtr<ID3D11Device> const& obj, std::string const& name)
	{
		obj->SetPrivateData(WKPDID_D3DDebugObjectName, UINT(name.size()), name.c_str());
	}

	//struct DXEventSection
	//{
	//	~DXEventSection()
	//	{
	//		if (_annotation)
	//		{
	//			_annotation->EndEvent();
	//		}
	//	}
	//	DXEventSection(DXEventSection&& other) : _annotation(other._annotation)
	//	{
	//		other._annotation = nullptr;
	//	}
	//private:
	//	friend class DeviceAndContext;
	//	ID3DUserDefinedAnnotation* _annotation = nullptr;

	//	DXEventSection(ID3DUserDefinedAnnotation* annotation, std::wstring const& section) : _annotation(annotation)
	//	{
	//		if (_annotation && _annotation->GetStatus())
	//		{
	//			_annotation->BeginEvent(section.c_str());
	//		}
	//		else
	//		{
	//			_annotation = nullptr;
	//		}
	//	}
	//	DXEventSection(ID3DUserDefinedAnnotation* annotation, wchar_t* section) : _annotation(annotation)
	//	{
	//		if (_annotation && _annotation->GetStatus())
	//		{
	//			_annotation->BeginEvent(section);
	//		}
	//		else
	//		{
	//			_annotation = nullptr;
	//		}
	//	}

	//	DXEventSection(DXEventSection const&) = delete;
	//	DXEventSection& operator=(DXEventSection const&) = delete;
	//	DXEventSection& operator=(DXEventSection&&) = delete;
	//};
}

