
#include "NtCoreLib.h"
#pragma hdrstop

#include "NtD3DMapping.h"

namespace NT
{

namespace RENDERER
{

NtDirectX11Renderer::NtDirectX11Renderer()
: NtD3DRenderer()
{
	m_swapchain = nullptr;
	m_device = nullptr;
	m_deviceContext = nullptr;
	m_renderTargetView = nullptr;
	m_depthStencilBuffer = nullptr;
	m_depthStencilState = nullptr;
	m_depthStencilView = nullptr;
	m_rasterState = nullptr;
}


NtDirectX11Renderer::~NtDirectX11Renderer()
{
	Release();
}

/*virtual*/ bool NtDirectX11Renderer::Initialize(int width, int height, HWND hwnd, bool vsync, bool fullscreen, float screenDepth, float screenNear)
{
	// vsync setting
	m_vsync = vsync;


	// create a DirectX Graphics Infrastructure factory
	IDXGIFactory* factory = nullptr;
	HRESULT res = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(res))
	{
		return false;
	}

	// use the factory to create an adapter for the promary graphics interface(video card)
	IDXGIAdapter* adapter = nullptr;
	res = factory->EnumAdapters(0, &adapter);
	if (FAILED(res))
	{
		return false;
	}

	// enumerate the primary adapter output (moniter)
	IDXGIOutput* adapterOutput = nullptr;
	res = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(res))
	{
		return false;
	}

	// get the number of modes that fis the DXGI_FORMAT_R8G8B8A8_UNORM display
	// format for the adapter output (moniter)
	ntUint numMode = 0;
	res = adapterOutput->GetDisplayModeList(g_colorFormat[eColorFormat::NT_FMT_A8R8G8B8], DXGI_ENUM_MODES_INTERLACED, &numMode, NULL);
	if (FAILED(res))
	{
		return false;
	}

	// create a list to hole all the possible display modes for this moniter / video card combination
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numMode];
	NtAsserte(displayModeList != nullptr);

	// not fill the display mode list structures
	res = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numMode, displayModeList);
	if (FAILED(res))
	{
		return false;
	}

	// now go through all the display modes and find the one that matches the screen width and height
	// When a match is found store the numerate and denominator of the refresh rate for that monitor
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

	// get the adapter (video card) description
	DXGI_ADAPTER_DESC adapterDesc;
	Crt::MemSet(&adapterDesc, sizeof(adapterDesc));
	res = adapter->GetDesc(&adapterDesc);
	if (FAILED(res))
	{
		return false;
	}

	// store the dedicated video card memory in megabytes;
	m_videoCardMemory = (ntInt)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// convert the name of the video card to a character array and store it
	ntChar videoDesc[128] = {0, };
	size_t strLength = 0;
	errno_t err = wcstombs_s(&strLength, videoDesc, 128, adapterDesc.Description, 128);
	if (err != 0)
	{
		return false;
	}

	// Initialze the swap chain desc
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	Crt::MemSet(&swapChainDesc, sizeof(swapChainDesc));

	// set to a single back buffer
	swapChainDesc.BufferCount = 1;

	// set the width and height of the back buffer
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;

	// set regular 32bit surface for the back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// set the refresh rate of the back buffer
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

	// set the usage of the back buffer
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// set the handle for the window to render to
	swapChainDesc.OutputWindow = hwnd;

	// turn multisampling off
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// set the scan line ordering and scaling to unspecified
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// discard the back buffer contents after presenting
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// dont set the advanced flags
	swapChainDesc.Flags = 0;

	// set the feture level to DX 11
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	// create the swap chain, Direct3d Device, and Direct3D device context,
	res = D3D11CreateDeviceAndSwapChain(
		adapter, 
		D3D_DRIVER_TYPE_UNKNOWN, 
		NULL,
		NULL, 
		NULL, //&featureLevel,
		NULL,
		D3D11_SDK_VERSION, 
		&swapChainDesc, &m_swapchain, &m_device, &featureLevel, &m_deviceContext);
	if (FAILED(res))
	{
		return false;
	}

	switch(featureLevel)
	{
	case D3D_FEATURE_LEVEL_9_1:
	case D3D_FEATURE_LEVEL_9_2:
	case D3D_FEATURE_LEVEL_9_3:
		m_shaderModel[0] = "vs_2_0";
		m_shaderModel[1] = "ps_2_0";
		break;
	case D3D_FEATURE_LEVEL_10_0:
	case D3D_FEATURE_LEVEL_10_1:
		m_shaderModel[0] = "vs_4_0";
		m_shaderModel[1] = "ps_4_0";
		break;
	case D3D_FEATURE_LEVEL_11_0:
		m_shaderModel[0] = "vs_5_0";
		m_shaderModel[1] = "ps_5_0";
		break;

	default:
		return false;
	}

	// release tools
	SAFE_DELETE_ARRAY(displayModeList);
	SAFE_RELEASE(adapterOutput);
	SAFE_RELEASE(adapter);
	SAFE_RELEASE(factory);

	//S_OK != D3D11CreateDeviceAndSwapChain(
	//	adapter, 
	//	D3D_DRIVER_TYPE_UNKNOWN, //D3D_DRIVER_TYPE_HARDWARE, 
	//	NULL, 
	//	NULL, //D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT, 
	//	NULL, 
	//	NULL,
	//	D3D11_SDK_VERSION, 
	//	&swapChainDesc,
	//	&m_d3dSwapChain, 
	//	&m_d3dDevice, 
	//	NULL, 
	//	&m_d3dDeviceContext)

	// get the pointer to the back buffer
	ID3D11Texture2D* backBuffer = nullptr;
	res = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (FAILED(res))
	{
		return false;
	}

	// create the render target view with the backbuffer pointer
	res = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);
	if (FAILED(res))
	{
		return false;
	}

	// release pointer to the backbuffer as we no longer need it
	SAFE_RELEASE(backBuffer);

	// initialize the descriptor of the depth buffer
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

	// create the texture for the depth buffer using the filled out description
	res = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(res))
	{
		return false;
	}

	// initialize the description of the stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	Crt::MemSet(&depthStencilDesc, sizeof(depthStencilDesc));

	// set up the desc of the stencil state
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xff;
	depthStencilDesc.StencilWriteMask = 0xff;

	// stencil operation if pixel is front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// stencil operation if pixel if back_-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// create the depth stencil state
	res = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(res))
	{
		return false;
	}

	// set the depth stencil state
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// initialize the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	Crt::MemSet(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// set up the depth stencil view desc
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// create the depth stencil view
	res = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(res))
	{
		return false;
	}

	// bind the render target view and depth stencil buffer to the output render pipeline
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// setup the raster description which will determine how and what polygons will be drawn
	D3D11_RASTERIZER_DESC rasterDesc;
	Crt::MemSet(&rasterDesc, sizeof(rasterDesc));

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// create the rasterize state from the description we just filled out
	res = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(res))
	{
		return false;
	}

	// now set the rasterizer state
	m_deviceContext->RSSetState(m_rasterState);

	// setup the viewport for rendering
	D3D11_VIEWPORT viewport;
	Crt::MemSet(&viewport, sizeof(viewport));

	viewport.Width = (ntFloat)width;
	viewport.Height = (ntFloat)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// create viewport
	m_deviceContext->RSSetViewports(1, &viewport);

	// setup the projection matrix
	float fieldOfView = (ntFloat)NtMath<float>::PI / 4.0f;
	float screenAspect = (ntFloat)width / (ntFloat)height;

	// create the projection matrix for 3D rendering
	m_proj = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
	//D3DXMatrixPerspectiveFovLH(&m_proj, fieldOfView, screenAspect, screenNear, screenDepth);

	// initialize the world matrix to the identity matrix
	m_world = XMMatrixIdentity();
	//D3DXMatrixIdentity(&m_world);

	// create an orthogonal graphic projection matrix for 2D rendering
	XMMatrixOrthographicLH((ntFloat)width, (ntFloat)height, screenNear, screenDepth);
	//D3DXMatrixOrthoLH(&m_ortho, (ntFloat)width, (ntFloat)height, screenNear, screenDepth);

	return true;
}


/*virtual*/ void NtDirectX11Renderer::Release()
{
	// before shutting down set to windowed mode or when you release the swap chain it will throw an exception
	if (m_swapchain)
	{
		m_swapchain->SetFullscreenState(FALSE, nullptr);
	}

	SAFE_RELEASE(m_rasterState);
	SAFE_RELEASE(m_depthStencilView);
	SAFE_RELEASE(m_depthStencilState);
	SAFE_RELEASE(m_depthStencilBuffer);
	SAFE_RELEASE(m_renderTargetView);
	SAFE_RELEASE(m_deviceContext);
	SAFE_RELEASE(m_device);
	SAFE_RELEASE(m_swapchain);
}

/*virtual*/ void NtDirectX11Renderer::BeginScene(float r, float g, float b, float a)
{
	// setup the color to clear the buffer to
	ntFloat color[4] = {r, g, b, a};

	// clear the back buffer
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// clear the depth buffer
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


/*virtual*/ void NtDirectX11Renderer::EndScene()
{
	// present the back buffer to the screen since rendering is complete
	if (m_vsync)
	{
		// lock to screen refresh rate
		m_swapchain->Present(1, 0);
	}
	else
	{
		// present as fast as possible
		m_swapchain->Present(0, 0);
	}
}


ID3D11Device* NtDirectX11Renderer::Device() const
{
	return m_device;
}

ID3D11DeviceContext* NtDirectX11Renderer::DeviceContext() const
{
	return m_deviceContext;
}


bool NtDirectX11Renderer::CreateShaderResourceView(ID3D11Texture1D* tex,D3D11_SHADER_RESOURCE_VIEW_DESC* SRVDesc,ID3D11ShaderResourceView** textureView)
{
	HRESULT res = Device()->CreateShaderResourceView(tex, SRVDesc, textureView);

	return FAILED(res) ? false : true;
}

bool NtDirectX11Renderer::CreateShaderResourceView(ID3D11Texture2D* tex,D3D11_SHADER_RESOURCE_VIEW_DESC* SRVDesc,ID3D11ShaderResourceView** textureView)
{
	HRESULT res = Device()->CreateShaderResourceView(tex, SRVDesc, textureView);

	return FAILED(res) ? false : true;
}

bool NtDirectX11Renderer::CreateShaderResourceView(ID3D11Texture3D* tex,D3D11_SHADER_RESOURCE_VIEW_DESC* SRVDesc,ID3D11ShaderResourceView** textureView)
{
	HRESULT res = Device()->CreateShaderResourceView(tex, SRVDesc, textureView);

	return FAILED(res) ? false : true;
}

}	// namespace RENDERER
}	// namespace NT
