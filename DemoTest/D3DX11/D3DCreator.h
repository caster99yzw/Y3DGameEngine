#pragma once

#include <d3d11_4.h>
#include <dxgi1_5.h>
#include <dxgidebug.h>
#include <d3d11sdklayers.h>

#include "..\Window\Win32Entity.h"
#include "D3DHelper.h"

namespace Y3D
{

	class D3DCreator 
	{
	public:

		D3DCreator();
		~D3DCreator();

	private:

		Win32Entity						m_window;

		ComPtr<ID3D11Device3>			m_d3dDevice;
		ComPtr<ID3D11DeviceContext3>	m_d3dContext;
		ComPtr<IDXGISwapChain3>			m_swapChain;

		ComPtr<ID3D11RenderTargetView1>	m_d3dRenderTargetView;
		D3D11_VIEWPORT					m_screenViewport;
		D3D_FEATURE_LEVEL				m_d3dFeatureLevel;

	public:

		void ValidatDevice();
		void HandleDeviceLost(HRESULT hr);

		void Present();
		void UpdateWindowSize();

		ID3D11Device3* GetD3DDevice() const;
		ID3D11DeviceContext3* GetD3DDeviceContext() const;
		IDXGISwapChain3* GetSwapChain() const;
		ID3D11RenderTargetView1* GetBackBufferRenderTargetView() const;
		D3D_FEATURE_LEVEL GetDeviceFeatureLevel() const;
		D3D11_VIEWPORT GetScreenViewport() const;

		//DXEventSection
		//DXEventSection

	private:

		void CreateDeviceAndContext();
		void CreateSwapChain();
		void FreeAllDXObjects();
	};
}