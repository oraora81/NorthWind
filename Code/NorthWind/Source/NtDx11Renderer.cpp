
#include "NtCoreLib.h"
#pragma hdrstop

#include "NtD3DMapping.h"

namespace nt { namespace renderer {

NtDx11Renderer::NtDx11Renderer()
: NtD3DRenderer()
, m_swapchain(nullptr)
, m_device(nullptr)
, m_deviceContext(nullptr)
, m_renderTargetView(nullptr)
, m_depthStencilBuffer(nullptr)
, m_depthStencilState(nullptr)
, m_depthStencilView(nullptr)
, m_4xMassQuality(0)
{
	
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
	// vsync 설정
	m_vsync = vsync;


	// DirectX Graphics Infrastructure factory 생성
	IDXGIFactory* factory = nullptr;
	HRF(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory));

	// 팩토리를 이용해 메인 그래픽스 인터페이스(비디오 카드) 어댑터 생성
	IDXGIAdapter* adapter = nullptr;
	HRF(factory->EnumAdapters(0, &adapter));
	
	/* Direct3d 10 버전만 지원함
	LARGE_INTEGER umdVersion;
	HRF(adapter->CheckInterfaceSupport(__uuidof(ID3D11Device), &umdVersion));
	*/

	// 메인 output어댑터 enumerate (moniter)
	IDXGIOutput* adapterOutput = nullptr;
	HRF(adapter->EnumOutputs(0, &adapterOutput));

	// DXGI_FORMAT_R8G8B8A8_UNORM 표시에 맞는 모드들 가져오기
	ntUint numMode = 0;
	HRF(adapterOutput->GetDisplayModeList(g_colorFormat[ColorFormat::NT_FMT_A8R8G8B8], DXGI_ENUM_MODES_INTERLACED, &numMode, NULL));

	// 비디오와 모니터 조합에 의한 디스플레이 설정수 만큼 생성
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numMode];
	NtAsserte(displayModeList != nullptr);

	// displayModeList 다시 채우기
    // D3D_FEATURE_LEVEL_11_1에서는 GetDisplayModeList1을 사용
	HRF(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numMode, displayModeList));

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
	HRF(adapter->GetDesc(&adapterDesc));

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

	// create device
	ntUint createDeviceFlags = 0;
#if defined(_DEBUG) || defined(DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// DX 11용 feture level 설정
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

	// 멀티 샘플링
	if (enable4xMsaa)
	{
		// 4X MSAA를 사용
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = m_4xMassQuality - 1;
	}
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}

	// 윈도우 모드 설정
	swapChainDesc.Windowed = fullscreen ? false : true;

	// scan line ordering ,scaling 은 unspecified로
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// 
	swapChainDesc.Flags = 0;

	// DXGI가 Alt + Enter를 감지하여 전체 창 전환을 못하도록 메시지큐를 모니터링 하지 않도록 한다.
	//HRF(factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER));

	// swap chain 생성
	HRF(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL, //&featureLevel,
		NULL,
		D3D11_SDK_VERSION, 
		&swapChainDesc, &m_swapchain, &m_device, &featureLevel, &m_deviceContext));

    HRF(m_device->CheckMultisampleQualityLevels(
        DXGI_FORMAT_R8G8B8A8_UNORM,
        4,
        &m_4xMassQuality));

    NtAsserte(m_4xMassQuality > 0);

    DXGI_OUTPUT_DESC outputDesc;
    Crt::MemSet(&outputDesc, sizeof(DXGI_OUTPUT_DESC));
    HRF(adapterOutput->GetDesc(&outputDesc));

	// release tools
	SAFE_DELETE_ARRAY(displayModeList);
	SAFE_RELEASE(adapterOutput);
	SAFE_RELEASE(adapter);
	SAFE_RELEASE(factory);
	
	// back buffer 포인터 얻어오기
	ID3D11Texture2D* backBuffer = nullptr;
	HRF(m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer));

	// backbuffer pointer로 렌더타겟뷰 생성
	HRF(m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView));

    D3D11_TEXTURE2D_DESC desc;
    backBuffer->GetDesc(&desc);

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

    if (enable4xMsaa)
    {
        depthBufferDesc.SampleDesc.Count = 4;
        depthBufferDesc.SampleDesc.Quality = m_4xMassQuality - 1;
    }
    else
    {
        depthBufferDesc.SampleDesc.Count = 1;
        depthBufferDesc.SampleDesc.Quality = 0;
    }

	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// 깊이 버퍼를 위한 텍스쳐 생성
	HRF(m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer));
    HRF(m_device->CreateDepthStencilView(m_depthStencilBuffer, NULL, &m_depthStencilView));

	// 렌더타겟뷰랑 깊이 스텐실 버퍼를 렌더파이프라인(output merger)에 바인딩
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// 어떻게 폴리곤을 그릴지 raster desc 설정
	if (NtRenderStateHandler::Initialize(m_device) == false)
	{
        return false;
	}

	// rasterizer state 설정
	m_deviceContext->RSSetState(NtRenderStateHandler::RSSolid);

	// 렌더링 뷰포트 설정
	D3D11_VIEWPORT viewport;
	Crt::MemSet(&viewport, sizeof(viewport));

	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// 
	m_deviceContext->RSSetViewports(1, &viewport);

	// 프로젝션 행렬 생성
	float fov = (float)NtMathf::PI * 0.25f;
	
	// 
	XMMATRIX p = XMMatrixPerspectiveFovLH(fov, g_app->AspectRatio(), screenNear, screenDepth);
	XMStoreFloat4x4(&m_proj, p);
	
	// 월드를 단위행렬로 초기화
	XMStoreFloat4x4(&m_world, ms_identity);
	
	// 2d 렌더링에 사용할 정사영 행렬 생성
	// create an orthogonal graphic projection matrix for 2D rendering
	m_ortho = XMMatrixOrthographicLH((float)width, (float)height, screenNear, screenDepth);

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

/*virtual*/ void NtDx11Renderer::BeginScene(const float* color)
{
	// back buffer 클리어
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// depth buffer 클리어
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    //m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


/*virtual*/ void NtDx11Renderer::EndScene()
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

/*virtual*/ bool NtDx11Renderer::Resize(ntInt width, ntInt height)
{
	NtAsserte(m_deviceContext != nullptr);
	NtAsserte(m_device != nullptr);
	NtAsserte(m_swapchain != nullptr);

	//
	SAFE_RELEASE(m_renderTargetView);
	SAFE_RELEASE(m_depthStencilView);
	SAFE_RELEASE(m_depthStencilBuffer);

	// 스왑체인 리사이징하고 렌더타겟뷰 다시 생성
	HRF(m_swapchain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	ID3D11Texture2D* backBuffer = nullptr;
	HRF(m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	
	m_device->CreateRenderTargetView(backBuffer, 0, &m_renderTargetView);
	SAFE_RELEASE(backBuffer);

	// depth stencil 버퍼, 뷰 생성
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

	// 파이프라인에 depth/stencil view를 바인딩
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// 뷰포트 다시 셋팅
	D3D11_VIEWPORT viewport;
	Crt::MemSet(&viewport, sizeof(viewport));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	m_deviceContext->RSSetViewports(1, &viewport);

	float fov = (float)NtMathf::PI * 0.25f;
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

bool NtDx11Renderer::CreateShaderResourceView(ntWchar* fileName, ID3D11ShaderResourceView** SRVMap)
{
    NtAsserte(fileName != nullptr);
    NtAsserte(Crt::StrLen(fileName) > 0);

    const ntWchar* filePath = g_resMgr->GetPath(fileName);
    NtAsserte(filePath != nullptr);

    HRF(D3DX11CreateShaderResourceViewFromFile(Device(),
        filePath,
        0,
        0,
        SRVMap,
        0));
    return true;
}

} }
