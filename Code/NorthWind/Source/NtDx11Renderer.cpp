
#include "NtCoreLib.h"
#pragma hdrstop

#include "NtD3DMapping.h"

namespace nt { namespace renderer {

NtDx11Renderer::NtDx11Renderer()
: NtD3DRenderer()
{
	m_swapchain          = nullptr;
	m_device             = nullptr;
	m_deviceContext      = nullptr;
	m_renderTargetView   = nullptr;
	m_depthStencilBuffer = nullptr;
	m_depthStencilState  = nullptr;
	m_depthStencilView   = nullptr;
}


NtDx11Renderer::~NtDx11Renderer()
{
	Release();
}

/*virtual*/ bool NtDx11Renderer::Initialize(
	int width, 
	int height, 
	HWND hwnd, 
	bool vsync, 
	bool fullscreen, 
	bool enable4xMsaa,
	float screenDepth, 
	float screenNear)
{
	// vsync ����
	m_vsync = vsync;


	// DirectX Graphics Infrastructure factory ����
	IDXGIFactory* factory = nullptr;
	HRF(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory));

	// ���丮�� �̿��� ���� �׷��Ƚ� �������̽�(���� ī��) ����� ����
	IDXGIAdapter* adapter = nullptr;
	HRF(factory->EnumAdapters(0, &adapter));
	
	/* Direct3d 10 ������ ������
	LARGE_INTEGER umdVersion;
	HRF(adapter->CheckInterfaceSupport(__uuidof(ID3D11Device), &umdVersion));
	*/

	// ���� output����� enumerate (moniter)
	IDXGIOutput* adapterOutput = nullptr;
	HRF(adapter->EnumOutputs(0, &adapterOutput));

	// DXGI_FORMAT_R8G8B8A8_UNORM ǥ�ÿ� �´� ���� ��������
	ntUint numMode = 0;
	HRF(adapterOutput->GetDisplayModeList(g_colorFormat[ColorFormat::NT_FMT_A8R8G8B8], DXGI_ENUM_MODES_INTERLACED, &numMode, NULL));

	// ������ ����� ���տ� ���� ���÷��� ������ ��ŭ ����
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numMode];
	NtAsserte(displayModeList != nullptr);

	// displayModeList �ٽ� ä���
	HRF(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numMode, displayModeList));

	// ȭ�� �ʺ�, ���̿� �´� �ػ� ã��
	int numerate = 0;
	int denominator = 0;
	for (ntUint i = 0; i < numMode; ++i)
	{
		if ((displayModeList[i].Width == (ntUint)width) &&
			displayModeList[i].Height == (ntUint)height)
		{
			numerate = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
			break;
		}
	}

	// ���� ī�� description ã��
	DXGI_ADAPTER_DESC adapterDesc;
	Crt::MemSet(&adapterDesc, sizeof(adapterDesc));
	HRF(adapter->GetDesc(&adapterDesc));

	// �Ҵ�� ���� �޸𸮸� mb�� ��ȯ
	m_videoCardMemory = (ntInt)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// 
	ntChar videoDesc[128] = {0, };
	size_t strLength = 0;
	errno_t err = wcstombs_s(&strLength, videoDesc, 128, adapterDesc.Description, 128);
	if (err != 0)
	{
		return false;
	}

	// create device
	ntUint createDeviceFlags = 0;
#if defined(_DEBUG) || defined(DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// DX 11�� feture level ����
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	/*res = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		createDeviceFlags,
		0, 0,
		D3D11_SDK_VERSION,
		&m_device,
		&featureLevel,
		&m_deviceContext);
	if (FAILED(res))
	{
		return false;
	}*/

	/*ntUint m4xMsaaQuality = 0;
	res = m_device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		4,
		&m4xMsaaQuality);
	if (FAILED(res))
	{
		return false;
	}
	NtAsserte(m4xMsaaQuality > 0);*/

	// swap chain desc �ʱ�ȭ
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	Crt::MemSet(&swapChainDesc, sizeof(swapChainDesc));

	// single back buffer
	swapChainDesc.BufferCount = 1;

	// back buffer �ʺ�/���� ����
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;

	// back buffer ������ ���� 32��Ʈ�� ����
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// back buffer ���������� ����
	if (m_vsync == true)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerate;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// back buffer usage ����
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// handle ����
	swapChainDesc.OutputWindow = hwnd;

	// ��Ƽ ���ø�
	if (enable4xMsaa)
	{
		// 4X MSAA�� ���
		//swapChainDesc.SampleDesc.Count = 4;
		//swapChainDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}

	// ������ ��� ����
	swapChainDesc.Windowed = fullscreen ? false : true;

	// scan line ordering ,scaling �� unspecified��
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// 
	swapChainDesc.Flags = 0;

	// DXGI�� Alt + Enter�� �����Ͽ� ��ü â ��ȯ�� ���ϵ��� �޽���ť�� ����͸� ���� �ʵ��� �Ѵ�.
	//HRF(factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER));

	// swap chain ����
	HRF(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL, 
		NULL, //&featureLevel,
		NULL,
		D3D11_SDK_VERSION, 
		&swapChainDesc, &m_swapchain, &m_device, &featureLevel, &m_deviceContext));

	// release tools
	SAFE_DELETE_ARRAY(displayModeList);
	SAFE_RELEASE(adapterOutput);
	SAFE_RELEASE(adapter);
	SAFE_RELEASE(factory);
	
	// back buffer ������ ������
	ID3D11Texture2D* backBuffer = nullptr;
	HRF(m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer));

	// backbuffer pointer�� ����Ÿ�ٺ� ����
	HRF(m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView));

	// ���� ����� ����
	SAFE_RELEASE(backBuffer);

	// ���̹��� desc �ʱ�ȭ
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	Crt::MemSet(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// ���� ���۸� ���� �ؽ��� ����
	HRF(m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer));

	// ���ٽ� desc ����
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	Crt::MemSet(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xff;
	depthStencilDesc.StencilWriteMask = 0xff;

	// ���ٽ� op - frontface
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// backface
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 
	HRF(m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState));

	// ���ٽǻ��� ����
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// 
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	Crt::MemSet(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// ���� ���ٽ� �� ����
	HRF(m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView));
	
	// ����Ÿ�ٺ�� ���� ���ٽ� ���۸� ��������������(output merger)�� ���ε�
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// ��� �������� �׸��� raster desc ����
	if (NtRenderStateHandler::Initialize(m_device) == false)
	{
        return false;
	}

	// rasterizer state ����
	m_deviceContext->RSSetState(NtRenderStateHandler::RSSolid);

	// ������ ����Ʈ ����
	D3D11_VIEWPORT viewport;
	Crt::MemSet(&viewport, sizeof(viewport));

	viewport.Width = static_cast<ntFloat>(width);
	viewport.Height = static_cast<ntFloat>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// 
	m_deviceContext->RSSetViewports(1, &viewport);

	// �������� ��� ����
	float fov = (ntFloat)NtMathf::PI * 0.25f;
	
	// 
	XMMATRIX p = XMMatrixPerspectiveFovLH(fov, g_app->AspectRatio(), screenNear, screenDepth);
	XMStoreFloat4x4(&m_proj, p);
	
	// ���带 ������ķ� �ʱ�ȭ
	XMStoreFloat4x4(&m_world, ms_identity);
	
	// 2d �������� ����� ���翵 ��� ����
	// create an orthogonal graphic projection matrix for 2D rendering
	m_ortho = XMMatrixOrthographicLH((ntFloat)width, (ntFloat)height, screenNear, screenDepth);

	return true;
}

/*virtual*/ void NtDx11Renderer::Release()
{
	// 
	if (m_swapchain)
	{
		m_swapchain->SetFullscreenState(FALSE, nullptr);
	}

	SAFE_RELEASE(m_depthStencilView);
	SAFE_RELEASE(m_depthStencilState);
	SAFE_RELEASE(m_depthStencilBuffer);
	SAFE_RELEASE(m_renderTargetView);
	SAFE_RELEASE(m_deviceContext);
	SAFE_RELEASE(m_device);
	SAFE_RELEASE(m_swapchain);
}

/*virtual*/ void NtDx11Renderer::BeginScene(const ntFloat* color)
{
	// back buffer Ŭ����
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// depth buffer Ŭ����
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


/*virtual*/ void NtDx11Renderer::EndScene()
{
	// �������� �Ϸ�Ǿ����Ƿ� ����۸� ǥ���Ѵ�
	if (m_vsync)
	{
		// ȭ�� ���������� ���� (60hz)
		m_swapchain->Present(1, 0);
	}
	else
	{
		// �������� ���� ������
		m_swapchain->Present(0, 0);
	}
}

/*virtual*/ bool NtDx11Renderer::Resize(ntInt width, ntInt height)
{
	NtAsserte(m_deviceContext != nullptr);
	NtAsserte(m_device != nullptr);
	NtAsserte(m_swapchain != nullptr);

	//
	SAFE_RELEASE(m_renderTargetView);
	SAFE_RELEASE(m_depthStencilView);
	SAFE_RELEASE(m_depthStencilBuffer);

	// ����ü�� ������¡�ϰ� ����Ÿ�ٺ� �ٽ� ����
	HRF(m_swapchain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	ID3D11Texture2D* backBuffer = nullptr;
	HRF(m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	
	m_device->CreateRenderTargetView(backBuffer, 0, &m_renderTargetView);
	SAFE_RELEASE(backBuffer);

	// depth stencil ����, �� ����
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HRF(m_device->CreateTexture2D(&depthStencilDesc, 0, &m_depthStencilBuffer));
	
	HRF(m_device->CreateDepthStencilView(m_depthStencilBuffer, 0, &m_depthStencilView));

	// ���������ο� depth/stencil view�� ���ε�
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// ����Ʈ �ٽ� ����
	D3D11_VIEWPORT viewport;
	Crt::MemSet(&viewport, sizeof(viewport));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	m_deviceContext->RSSetViewports(1, &viewport);

	float fov = (ntFloat)NtMathf::PI * 0.25f;
	XMMATRIX p = XMMatrixPerspectiveFovLH(fov, g_app->AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&m_proj, p);

	return true;
}

ID3D11Device* NtDx11Renderer::Device() const
{
	return m_device;
}

ID3D11DeviceContext* NtDx11Renderer::DeviceContext() const
{
	return m_deviceContext;
}

bool NtDx11Renderer::CreateShaderResourceView(ID3D11Texture1D* tex,D3D11_SHADER_RESOURCE_VIEW_DESC* SRVDesc,ID3D11ShaderResourceView** textureView)
{
	HRESULT res = Device()->CreateShaderResourceView(tex, SRVDesc, textureView);

	return FAILED(res) ? false : true;
}

bool NtDx11Renderer::CreateShaderResourceView(ID3D11Texture2D* tex,D3D11_SHADER_RESOURCE_VIEW_DESC* SRVDesc,ID3D11ShaderResourceView** textureView)
{
	HRESULT res = Device()->CreateShaderResourceView(tex, SRVDesc, textureView);

	return FAILED(res) ? false : true;
}

bool NtDx11Renderer::CreateShaderResourceView(ID3D11Texture3D* tex,D3D11_SHADER_RESOURCE_VIEW_DESC* SRVDesc,ID3D11ShaderResourceView** textureView)
{
	HRESULT res = Device()->CreateShaderResourceView(tex, SRVDesc, textureView);

	return FAILED(res) ? false : true;
}

} }
