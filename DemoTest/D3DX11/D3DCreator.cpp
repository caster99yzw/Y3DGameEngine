#include "D3DCreator.h"

using namespace Y3D;

D3DCreator::D3DCreator()
{
	CreateDeviceAndContext();
	CreateSwapChain();
}

D3DCreator::~D3DCreator()
{
	FreeAllDXObjects();
}

void D3DCreator::CreateDeviceAndContext()
{
	//	This flag adds support for surface with a different color channel ordering
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	//	If the project is in a debug build, enable debugging via SDK layers with this flag.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//	This array defines the multiset of DirectX hardware feature levels this app will support.
	//	Note the ordering should be preserved.
	//	Don't forget to declare your application's minimum required feature level in its
	//	description.  All applications are assumed to support 9.1 unless otherwise stated.
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	//	Create the Direct3D 11 API device object and a corresponding context.
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;

	D3D11CreateDevice(
		nullptr,					// Specify nullptr to use the default adapter.
		D3D_DRIVER_TYPE_HARDWARE,	// Create a device using the hardware graphics driver.
		0,							// Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
		creationFlags,				// multiset debug and Direct2D compatibility flags.
		featureLevels,				// List of feature levels this app can support.
		ARRAYSIZE(featureLevels),	// Size of the list above.
		D3D11_SDK_VERSION,			// Always multiset this to D3D11_SDK_VERSION for Windows Store apps.
		&device,					// Returns the Direct3D device created.
		&m_d3dFeatureLevel,			// Returns feature level of device created.
		&context					// Returns the device immediate context.
	);

	// Store pointers to the Direct3D 11.3 API device and immediate context.
	device.As(&m_d3dDevice);
	context.As(&m_d3dContext);
}

void D3DCreator::CreateSwapChain()
{
	ID3D11RenderTargetView* defaultView[] = { nullptr };

	constexpr UINT32 s = ARRAYSIZE(defaultView);
	m_d3dContext->OMSetRenderTargets(ARRAYSIZE(defaultView), defaultView, nullptr);
	m_d3dRenderTargetView = nullptr;
	m_d3dContext->Flush1(D3D11_CONTEXT_TYPE_ALL, nullptr);

	int X = 0, Y = 0;

	// resize

	{
		//	Create a new one using the same adapter as the existing Direct3D device.
		DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };

		swapChainDesc.BufferDesc.Width = X;
		swapChainDesc.BufferDesc.Height = Y;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.OutputWindow = reinterpret_cast<HWND>(m_window.GetWinHandle());
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		//	This sequence obtains the DXGI factory that was used to create the Direct3D device above.
		ComPtr<IDXGIDevice3> dxgiDevice;
		m_d3dDevice.As(&dxgiDevice);

		ComPtr<IDXGIAdapter> dxgiAdapter;
		dxgiDevice->GetAdapter(&dxgiAdapter);

		ComPtr<IDXGIFactory4> dxgiFactory;
		dxgiFactory->GetParent(IID_PPV_ARGS(&dxgiFactory));

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc = { 0 };

		ComPtr<IDXGISwapChain> swapChain;
		dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &swapChainDesc, &swapChain);
		swapChain.As(&m_swapChain);

		//	Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and
		//	ensures that the application will only render after each VSync, minimizing power consumption.
		//ThrowIfFailed(dxgiDevice->multisetMaximumFrameLatency(1));
	}

	//	Create a render target view of the swap chain back buffer.
	ComPtr<ID3D11Texture2D1> backBuffer;
	m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

	m_d3dDevice->CreateRenderTargetView1(backBuffer.Get(), nullptr, &m_d3dRenderTargetView);

	//	multiset the 3D rendering viewport to target the entire window.
	m_screenViewport = CD3D11_VIEWPORT(0.0f, 0.0f, (float)X, (float)Y);

	m_d3dContext->RSSetViewports(1, &m_screenViewport);
}

void D3DCreator::FreeAllDXObjects()
{
	m_d3dContext->ClearState();
	m_d3dContext->Flush1(D3D11_CONTEXT_TYPE_ALL, nullptr);
	m_d3dDevice = nullptr;
	m_d3dContext = nullptr;
	m_swapChain = nullptr;
	m_d3dRenderTargetView = nullptr;
}


//// This method is called in the event handler for the DisplayContentsInvalidated event.
//void D3DCreator::ValidateDevice()
//{
//	// The D3D Device is no longer valid if the default adapter changed since the device
//	// was created or if the device has been removed.
//
//	// First, get the information for the default adapter from when the device was created.
//
//	ComPtr<IDXGIDevice3> dxgiDevice;
//	ThrowIfFailed(_d3dDevice.As(&dxgiDevice));
//
//	ComPtr<IDXGIAdapter> deviceAdapter;
//	ThrowIfFailed(dxgiDevice->GetAdapter(&deviceAdapter));
//
//	ComPtr<IDXGIFactory4> deviceFactory;
//	ThrowIfFailed(deviceAdapter->GetParent(IID_PPV_ARGS(&deviceFactory)));
//
//	ComPtr<IDXGIAdapter1> previousDefaultAdapter;
//	ThrowIfFailed(deviceFactory->EnumAdapters1(0, &previousDefaultAdapter));
//
//	DXGI_ADAPTER_DESC1 previousDesc;
//	ThrowIfFailed(previousDefaultAdapter->GetDesc1(&previousDesc));
//
//	// Next, get the information for the current default adapter.
//
//	ComPtr<IDXGIFactory4> currentFactory;
//	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&currentFactory)));
//
//	ComPtr<IDXGIAdapter1> currentDefaultAdapter;
//	ThrowIfFailed(currentFactory->EnumAdapters1(0, &currentDefaultAdapter));
//
//	DXGI_ADAPTER_DESC1 currentDesc;
//	ThrowIfFailed(currentDefaultAdapter->GetDesc1(&currentDesc));
//
//	// If the adapter LUIDs don't match, or if the device reports that it has been removed,
//	// a new D3D device must be created.
//
//	if (previousDesc.AdapterLuid.LowPart != currentDesc.AdapterLuid.LowPart ||
//		previousDesc.AdapterLuid.HighPart != currentDesc.AdapterLuid.HighPart ||
//		FAILED(_d3dDevice->GetDeviceRemovedReason()))
//	{
//		// Release references to resources related to the old device.
//		dxgiDevice = nullptr;
//		deviceAdapter = nullptr;
//		deviceFactory = nullptr;
//		previousDefaultAdapter = nullptr;
//
//		// Create a new device and swap chain.
//		HandleDeviceLost(_d3dDevice->GetDeviceRemovedReason());
//	}
//}
//
//// Recreate all device resources and multiset them back to the current state.
//void D3DCreator::HandleDeviceLost(HRESULT hr)
//{
//	ThrowIfFailed(hr); // TODO try to see when this will happen.
//
//	_swapChain = nullptr;
//
//	// 	if (m_deviceNotify != nullptr)
//	// 	{
//	// 		m_deviceNotify->OnDeviceLost();
//	// 	}
//
//	CreateDeviceAndContext();
//	CreateSwapChain();
//
//	// 	if (m_deviceNotify != nullptr)
//	// 	{
//	// 		m_deviceNotify->OnDeviceRestored();
//	// 	}
//}
//
//
//// Present the contents of the swap chain to the screen.
//void D3DCreator::Present()
//{
//	// The first argument (value: 1) instructs DXGI to block until VSync, putting the application
//	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
//	// frames that will never be displayed to the screen.
//	DXGI_PRESENT_PARAMETERS parameters = { 0 };
//	HRESULT hr = _swapChain->Present1(0, 0, &parameters);
//
//
//	// Discard the contents of the render target.
//	// This is a valid operation only when the existing contents will be entirely
//	// overwritten. If dirty or scroll rects are used, this call should be removed.
//	_d3dContext->DiscardView1(_d3dRenderTargetView.Get(), nullptr, 0);
//
//	// If the device was removed either by a disconnection or a driver upgrade, we 
//	// must recreate all device resources.
//	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_REmultiset)
//	{
//		HandleDeviceLost(hr);
//	}
//	else
//	{
//		ThrowIfFailed(hr);
//	}
//}
//
//void D3DCreator::UpdateWindowSize()
//{
//	CreateSwapChain();
//}


//#ifdef _DEBUG
//
//void D3DCreator::CreateDebugFacility()
//{
//	ComPtr<ID3D11InfoQueue> infoQueue;
//	ThrowIfFailed(_d3dDevice->QueryInterface<ID3D11InfoQueue>(&infoQueue));
//	ThrowIfFailed(_d3dDevice->QueryInterface<ID3D11Debug>(&_debug));
//
//	// 	infoQueue->multisetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
//	// 	infoQueue->multisetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
//	// 	infoQueue->multisetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING, true);
//	// 	infoQueue->multisetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_INFO, true);
//	// 	infoQueue->multisetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_MESSAGE, true);
//	// 	infoQueue->multisetBreakOnCategory(D3D11_MESSAGE_CATEGORY_STATE_CREATION, false);
//
//
//	ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
//	DXGIGetDebugInterface1(0, IID_PPV_ARGS(&_dxgiDebug));
//	DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiInfoQueue));
//
//	dxgiInfoQueue->multisetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
//	dxgiInfoQueue->multisetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
//	dxgiInfoQueue->multisetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING, true);
//	dxgiInfoQueue->multisetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_INFO, true);
//	dxgiInfoQueue->multisetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_MESSAGE, true);
//	dxgiInfoQueue->multisetBreakOnCategory(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_CREATION, false);
//
//	ThrowIfFailed(_d3dContext->QueryInterface<ID3DUserDefinedAnnotation>(&_annotation));
//}
//
//#endif // _DEBUG