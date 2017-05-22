
#include "NtCoreLib.h"
#pragma hdrstop

#include "NtD3DMapping.h"

namespace nt { namespace renderer {

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
	// vsync 설정
	m_vsync = vsync;


	// DirectX Graphics Infrastructure factory 생성
	IDXGIFactory* factory = nullptr;
	HRESULT res = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(res))
	{
		return false;
	}

	// 팩토리를 이용해 메인 그래픽스 인터페이스(비디오 카드) 어댑터 생성
	IDXGIAdapter* adapter = nullptr;
	res = factory->EnumAdapters(0, &adapter);
	if (FAILED(res))
	{
		return false;
	}

	// 메인 output어댑터 enumerate (moniter)
	IDXGIOutput* adapterOutput = nullptr;
	res = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(res))
	{
		return false;
	}

	// DXGI_FORMAT_R8G8B8A8_UNORM 표시에 맞는 모드들 가져오기
	ntUint numMode = 0;
	res = adapterOutput->GetDisplayModeList(g_colorFormat[eColorFormat::NT_FMT_A8R8G8B8], DXGI_ENUM_MODES_INTERLACED, &numMode, NULL);
	if (FAILED(res))
	{
		return false;
	}

	// 비디오와 모니터 조합에 의한 디스플레이 설정수 만큼 생성
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numMode];
	NtAsserte(displayModeList != nullptr);

	// displayModeList 다시 채우기
	res = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numMode, displayModeList);
	if (FAILED(res))
	{
		return false;
	}

	// 화면 너비, 높이에 맞는 해상도 찾기
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

	// 비디오 카드 description 찾기
	DXGI_ADAPTER_DESC adapterDesc;
	Crt::MemSet(&adapterDesc, sizeof(adapterDesc));
	res = adapter->GetDesc(&adapterDesc);
	if (FAILED(res))
	{
		return false;
	}

	// 할당된 비디오 메모리를 mb로 변환
	m_videoCardMemory = (ntInt)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// 
	ntChar videoDesc[128] = {0, };
	size_t strLength = 0;
	errno_t err = wcstombs_s(&strLength, videoDesc, 128, adapterDesc.Description, 128);
	if (err != 0)
	{
		return false;
	}

	// swap chain desc 초기화
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	Crt::MemSet(&swapChainDesc, sizeof(swapChainDesc));

	// single back buffer
	swapChainDesc.BufferCount = 1;

	// back buffer 너비/높이 설정
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;

	// back buffer 포맷은 정규 32비트로 설정
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// back buffer 리프레시율 설정
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

	// back buffer usage 설정
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// handle 셋팅
	swapChainDesc.OutputWindow = hwnd;

	// 멀티 샘플링은 끔
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// 윈도우 모드 설정
	swapChainDesc.Windowed = fullscreen ? false : true;

	// scan line ordering ,scaling 은 unspecified로
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// 
	swapChainDesc.Flags = 0;

	// DX 11용 feture level 설정
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	// swap chain, Direct3d Device, Direct3D device context 생성
	res = D3D11CreateDeviceAndSwapChain(
		nullptr, 
		D3D_DRIVER_TYPE_HARDWARE,
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

	// back buffer 포인터 얻어오기
	ID3D11Texture2D* backBuffer = nullptr;
	res = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (FAILED(res))
	{
		return false;
	}

	// backbuffer pointer로 렌더타겟뷰 생성
	res = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);
	if (FAILED(res))
	{
		return false;
	}

	// 얻어온 백버퍼 해제
	SAFE_RELEASE(backBuffer);

	// 깊이버퍼 desc 초기화
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

	// 깊이 버퍼를 위한 텍스쳐 생성
	res = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(res))
	{
		return false;
	}

	// 스텐실 desc 설정
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	Crt::MemSet(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xff;
	depthStencilDesc.StencilWriteMask = 0xff;

	// 스텐실 op - frontface
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
	res = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(res))
	{
		return false;
	}

	// 스텐실상태 설정
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// 
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	Crt::MemSet(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// 깊이 스텐실 뷰 생성
	res = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(res))
	{
		return false;
	}

	// 렌더타겟뷰랑 깊이 스텐실 버퍼를 렌더파이프라인(output merger)에 바인딩
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// 어떻게 폴리곤을 그릴지 raster desc 설정
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

	//
	res = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(res))
	{
		return false;
	}

	// rasterizer state 설정
	m_deviceContext->RSSetState(m_rasterState);

	// 렌더링 뷰포트 설정
	D3D11_VIEWPORT viewport;
	Crt::MemSet(&viewport, sizeof(viewport));

	viewport.Width = (ntFloat)width;
	viewport.Height = (ntFloat)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// 
	m_deviceContext->RSSetViewports(1, &viewport);

	// 프로젝션 행렬 생성
	float fieldOfView = (ntFloat)NtMath<float>::PI / 4.0f;
	float screenAspect = (ntFloat)width / (ntFloat)height;

	// 
	m_proj = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
	
	// 월드를 단위행렬로 초기화
	m_world = XMMatrixIdentity();
	
	// 2d 렌더링에 사용할 정사영 행렬 생성
	// create an orthogonal graphic projection matrix for 2D rendering
	XMMatrixOrthographicLH((ntFloat)width, (ntFloat)height, screenNear, screenDepth);

	return true;
}


/*virtual*/ void NtDirectX11Renderer::Release()
{
	// 
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
	// 
	ntFloat color[4] = {r, g, b, a};

	// back buffer 클리어
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// depth buffer 클리어
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


/*virtual*/ void NtDirectX11Renderer::EndScene()
{
	// 렌더링이 완료되었으므로 백버퍼를 표시한다
	if (m_vsync)
	{
		// 화면 리프레시율 고정 (60hz)
		m_swapchain->Present(1, 0);
	}
	else
	{
		// 가능한한 많이 빠르게
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

}	// namespace renderer
}	// namespace nt
