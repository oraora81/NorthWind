
#include "stdafx.h"
#pragma hdrstop

#include "DXRenderer.h"

namespace NT
{
namespace _dx11
{

//--------------------------------------------------------------
/**
		
*/
int	DXMiscContainer::GetDXGiByteSize(DXGI_FORMAT formatType)
{
	switch(formatType)
	{
		case DXGI_FORMAT_R32G32B32A32_TYPELESS:
		case DXGI_FORMAT_R32G32B32A32_FLOAT   :
		case DXGI_FORMAT_R32G32B32A32_UINT    :
		case DXGI_FORMAT_R32G32B32A32_SINT    :
			return 16;
		case DXGI_FORMAT_R32G32B32_TYPELESS:
		case DXGI_FORMAT_R32G32B32_FLOAT   :
		case DXGI_FORMAT_R32G32B32_UINT    :
		case DXGI_FORMAT_R32G32B32_SINT    :
			return 12;
		case DXGI_FORMAT_R32G32_FLOAT:
		case DXGI_FORMAT_R32G32_UINT :
		case DXGI_FORMAT_R32G32_SINT :
			return 8;
	}

	return 0;
}

D3D11_INPUT_ELEMENT_DESC* DXMiscContainer::InitMiscDataList(int sumProperty, ...)
{
	//D3D11_INPUT_ELEMENT_DESC elements[] = 
	//{
	//	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//};

	NtAsserte(0 < sumProperty);

	D3D11_INPUT_ELEMENT_DESC* elements = new D3D11_INPUT_ELEMENT_DESC[sumProperty];
	NtAsserte(elements != NULL);

	va_list argument;
	va_start(argument, sumProperty);

	for (int i = 0; i < sumProperty; ++i)
	{
		D3D11_INPUT_ELEMENT_DESC& elem = elements[i];

		int elem_type = va_arg(argument, int);
		if (elem_type & ELEMENT_POSITION)
		{
			elem.SemanticName = "POSITION";
		}
		else if (elem_type & ELEMENT_COLOR)
		{
			elem.SemanticName = "COLOR";
		}
		else if (elem_type & ELEMENT_NORMAL)
		{
			elem.SemanticName = "NORMAL";
		}
		else if(elem_type & ELEMENT_UV)
		{
			elem.SemanticName = "TEXCOORD";
		}
		elem.SemanticIndex = 0;

		DXGI_FORMAT formatType = static_cast<DXGI_FORMAT>(va_arg(argument, int));

		elem.Format = formatType;
		elem.InputSlot = 0;
		elem.AlignedByteOffset = GetDXGiByteSize(formatType);
		elem.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elem.InstanceDataStepRate = 0;
	}

	return elements;
}


//--------------------------------------------------------------
/**
	
*/
DXRenderer::DXRenderer()
{
	m_vertexCount = 0;
	m_indexCount = 0;
	m_vtxBuffers = NULL;

	m_rot = 0.0f;
	m_backBuffer = NULL;
}

DXRenderer::~DXRenderer()
{
	Release();
}


bool DXRenderer::Initialize(HWND hwnd)
{
	// Enumerate First Adapter
	IDXGIFactory1* dxGIFactory = NULL;
	if (S_OK != CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&dxGIFactory))
	{
		return false;
	}

	IDXGIAdapter1* adapter;
	if (S_OK != dxGIFactory->EnumAdapters1(0, &adapter))
	{
		return false;
	}
	dxGIFactory->Release();

	/*
		Describe Buffer
	*/
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Width = m_screenWidth;
	bufferDesc.Height = m_screenHeight;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	/*
		Describe SwapChain
	*/
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// allow full-screen switching

	if (S_OK != D3D11CreateDeviceAndSwapChain(
		adapter, 
		D3D_DRIVER_TYPE_UNKNOWN, //D3D_DRIVER_TYPE_HARDWARE, 
		NULL, 
		NULL, //D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT, 
		NULL, 
		NULL,
		D3D11_SDK_VERSION, 
		&swapChainDesc,
		&m_d3dSwapChain, 
		&m_d3dDevice, 
		NULL, 
		&m_d3dDeviceContext))
	{
		return false;
	}

	// initialize Direct2D, DirectWrite
	if (InitDirectWrite(adapter) == false)
	{
		adapter->Release();

		return false;
	}
	
	adapter->Release();

	// get the address of the back buffer
	if (S_OK != m_d3dSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_backBuffer))
	{
		return false;
	}

	// use the back buffer address to create the render target
	if (S_OK != m_d3dDevice->CreateRenderTargetView(m_backBuffer, NULL, &m_renderView))
	{
		return false;
	}
	

	// describe depth / stencil buffer
	D3D11_TEXTURE2D_DESC delthStencilDesc;
	delthStencilDesc.Width = m_screenWidth;
	delthStencilDesc.Height = m_screenHeight;
	delthStencilDesc.MipLevels = 1;
	delthStencilDesc.ArraySize = 1;
	delthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	delthStencilDesc.SampleDesc.Count = 1;
	delthStencilDesc.SampleDesc.Quality = 0;
	delthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	delthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	delthStencilDesc.CPUAccessFlags = 0;
	delthStencilDesc.MiscFlags = 0;

	m_d3dDevice->CreateTexture2D(&delthStencilDesc, NULL, &m_depthStencilBuffer);
	m_d3dDevice->CreateDepthStencilView(m_depthStencilBuffer, NULL, &m_depthStencilView);


	// set the render target as the back buffer
	m_d3dDeviceContext->OMSetRenderTargets(1, &m_renderView, m_depthStencilView);

	// set the viewport
	D3D11_VIEWPORT viewPort;
	ZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));

	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = static_cast<float>(m_screenWidth);
	viewPort.Height = static_cast<float>(m_screenHeight);
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;

	m_d3dDeviceContext->RSSetViewports(1, &viewPort);

	// create the buffer to send to the constant buffer in effect file
	D3D11_BUFFER_DESC cbdesc;
	ZeroMemory(&cbdesc, sizeof(cbdesc));

	cbdesc.Usage = D3D11_USAGE_DEFAULT;
	cbdesc.ByteWidth = sizeof(cbPerObject);
	cbdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbdesc.CPUAccessFlags = 0;
	cbdesc.MiscFlags = 0;

	if (S_OK != m_d3dDevice->CreateBuffer(&cbdesc, NULL, &m_cbPerObjectBuffer))
	{
		return false;
	}

	// create the buffer to send to the cbuffer per frame in effect file
	ZeroMemory(&cbdesc, sizeof(D3D11_BUFFER_DESC));

	cbdesc.Usage = D3D11_USAGE_DEFAULT;
	cbdesc.ByteWidth = sizeof(cbPerFrame);
	cbdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbdesc.CPUAccessFlags = 0;
	cbdesc.MiscFlags = 0;

	if (S_OK != m_d3dDevice->CreateBuffer(&cbdesc, NULL, &m_cbPerFrameBuffer))
	{
		return false;
	}


	// camera info
	m_camPosition = XMVectorSet(0.0f, 3.0f, -8.0f, 0.0f);
	m_camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_camup = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// set the view matrix
	m_view = XMMatrixLookAtLH(m_camPosition, m_camTarget, m_camup);

	// set the projection matrix
	m_projection = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)m_screenWidth/m_screenHeight, 1.0f, 1000.0f);


	// loading texture
	if (S_OK != D3DX11CreateShaderResourceViewFromFile(m_d3dDevice, 
		L"box1.jpg", 
		//L"[click.png",
		NULL, NULL, &m_texture, NULL))
	{
		return false;
	}

	// describe the sample state
	D3D11_SAMPLER_DESC sampdesc;
	ZeroMemory(&sampdesc, sizeof(sampdesc));
	sampdesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampdesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampdesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampdesc.MinLOD = 0;
	sampdesc.MaxLOD = D3D11_FLOAT32_MAX;

	// create the sampler state
	if (S_OK != m_d3dDevice->CreateSamplerState(&sampdesc, &m_texSamplerState))
	{
		return false;
	}

	// @@@ temp cull test code
	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory(&rastDesc, sizeof(rastDesc));
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_NONE;
	if (S_OK != m_d3dDevice->CreateRasterizerState(&rastDesc, &m_noCull))
	{
		return false;
	}


	// define the blending equation
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rendertargetblendDesc;
	ZeroMemory(&rendertargetblendDesc, sizeof(rendertargetblendDesc));

	rendertargetblendDesc.BlendEnable	= true;
	rendertargetblendDesc.SrcBlend		= D3D11_BLEND_SRC_COLOR;
	rendertargetblendDesc.DestBlend		= D3D11_BLEND_INV_SRC_ALPHA;
	rendertargetblendDesc.BlendOp		= D3D11_BLEND_OP_ADD;
	rendertargetblendDesc.SrcBlendAlpha	= D3D11_BLEND_ONE;
	rendertargetblendDesc.DestBlendAlpha= D3D11_BLEND_ZERO;
	rendertargetblendDesc.BlendOpAlpha	= D3D11_BLEND_OP_ADD;
	rendertargetblendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable	= false;
	blendDesc.RenderTarget[0] = rendertargetblendDesc;

	
	if (S_OK != m_d3dDevice->CreateBlendState(&blendDesc, &m_transparency))
	{
		return false;
	}

	// create the Counter Clockwise and Clockwise Culling States
	D3D11_RASTERIZER_DESC rasterizeDesc;
	ZeroMemory(&rasterizeDesc, sizeof(rasterizeDesc));

	rasterizeDesc.FillMode	= D3D11_FILL_SOLID;
	rasterizeDesc.CullMode	= D3D11_CULL_BACK;
	rasterizeDesc.FrontCounterClockwise = TRUE;
	if (S_OK != m_d3dDevice->CreateRasterizerState(&rasterizeDesc,&m_CCWCullMode))
	{
		return false;
	}

	rasterizeDesc.FrontCounterClockwise = FALSE;
	if (S_OK != m_d3dDevice->CreateRasterizerState(&rasterizeDesc, &m_CWCullMode))
	{
		return false;
	}

	//m_cbPerObj.COLOR = XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f);

	InitD2DScreenTexture();

	if (!InitShader())
	{
		return false;
	}

	// create the input layout object
	//m_d3dDevice->CreateInputLayout(, 3, m_VSBuffer->GetBufferPointer(), m_VSBuffer->GetBufferSize(), &m_layout);
	//m_d3dDeviceContext->IASetInputLayout(m_layout);
	
	return true;
}

namespace
{
	struct Vertex	//Overloaded Vertex Structure
	{
		float pos[3];
		float texCoord[2];
		float normal[3];
	};
}

void DXRenderer::InitD2DScreenTexture()
{
	// create vertex buffer	
	Vertex vtxList[] = 
	{
		{-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0, -1.0f, -1.0f},
		{-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0, 1.0f, -1.0f},
		{1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0, 1.0f, -1.0f},
		{1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0, -1.0f, -1.0f},
	};

	unsigned short indices[] =
	{
		0, 1, 2,
		0, 2, 3,
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned short) * 2 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = indices;
	m_d3dDevice->CreateBuffer(&indexBufferDesc, &iinitData, &m_d2dIdxBuffer);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));

	vertexBufferData.pSysMem = vtxList;
	if (S_OK != m_d3dDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_d2dVtxBuffer))
	{
		return;
	}

	m_d3dDevice->CreateShaderResourceView(m_sharedTexture, NULL, &m_d2dTexture);
}

bool DXRenderer::Release()
{
	if (m_d3dSwapChain)
	{
		m_d3dSwapChain->SetFullscreenState(FALSE, NULL);
	}
	
	// close and release all existing COM objects
	SAFE_RELEASE(m_d3dDevice10);
	SAFE_RELEASE(m_keyedMutex10);
	SAFE_RELEASE(m_keyedMutex11);
	SAFE_RELEASE(m_d2dRenderTarget);
	SAFE_RELEASE(m_currentBrush);
	SAFE_RELEASE(m_backBuffer);
	SAFE_RELEASE(m_sharedTexture);
	SAFE_RELEASE(m_dwriteFactory);
	SAFE_RELEASE(m_dwriteTextFormat);
	SAFE_RELEASE(m_d2dTexture);
	SAFE_RELEASE(m_noCull);
	SAFE_RELEASE(m_CWCullMode);
	SAFE_RELEASE(m_CCWCullMode);
	SAFE_RELEASE(m_transparency);
	SAFE_RELEASE(m_wireFrame);
	SAFE_RELEASE(m_cbPerObjectBuffer);
	SAFE_RELEASE(m_depthStencilBuffer);
	SAFE_RELEASE(m_depthStencilView);
	SAFE_RELEASE(m_layout);
	SAFE_RELEASE(m_PSBuffer);
	SAFE_RELEASE(m_VSBuffer);
	SAFE_RELEASE(m_VShader);
	SAFE_RELEASE(m_PShader);
	SAFE_RELEASE(m_idxBuffer);
	SAFE_RELEASE(m_cbPerFrameBuffer);

	if (m_vtxBuffers)
	{
		delete [] m_vtxBuffers;
	}

	for (auto itor = m_vtxBufferList.begin(); itor != m_vtxBufferList.end(); ++itor)
	{
		ID3D11Buffer* buffer = (*itor).vtxBuffer;
		SAFE_RELEASE(buffer);
	}

	SAFE_RELEASE(m_d3dSwapChain);
	SAFE_RELEASE(m_renderView);
	SAFE_RELEASE(m_d3dDevice);
	SAFE_RELEASE(m_d3dDeviceContext);

	return true;
}

bool DXRenderer::InitShader()
{
	// load and compile the two shaders
	m_VSBuffer = NULL;
	m_PSBuffer = NULL;

	//
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	HRESULT result;
	result = CompileShader(L"exam.fx", "VS", "vs_4_0", &m_VSBuffer);
	if (FAILED(result))
	{
		m_VSBuffer->Release();
	}


	result = CompileShader(L"exam.fx", "PS", "ps_4_0", &m_PSBuffer);
	if (FAILED(result))
	{
		m_PSBuffer->Release();
	}

	result = CompileShader(L"exam.fx", "D2D_PS", "ps_4_0", &m_d2dPShaderBuffer);
	if (FAILED(result))
	{
		m_d2dPShader->Release();
	}

	//HRESULT hr = D3DX11CompileFromFile(L"exam.fx", 0, 0, "VS", "vs_4_0", 0, 0, 0, &VS, 0, 0);
	//hr = D3DX11CompileFromFile(L"exam.fx", 0, 0, "PS", "ps_4_0", 0, 0, 0, &PS, 0, 0);

	//
	// create the shader object
	m_d3dDevice->CreateVertexShader(m_VSBuffer->GetBufferPointer(), m_VSBuffer->GetBufferSize(), NULL, &m_VShader);
	m_d3dDevice->CreatePixelShader(m_PSBuffer->GetBufferPointer(), m_PSBuffer->GetBufferSize(), NULL, &m_PShader);

	m_d3dDevice->CreatePixelShader(m_d2dPShaderBuffer->GetBufferPointer(), m_d2dPShaderBuffer->GetBufferSize(), NULL, &m_d2dPShader);


	// Set Vertex and pixel shaders
	m_d3dDeviceContext->VSSetShader(m_VShader, 0, 0);
	m_d3dDeviceContext->PSSetShader(m_PShader, 0, 0);


	// Set Light Param
	m_light.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_light.range = 100.0f;
	m_light.att = XMFLOAT3(0.0f, 0.2f, 0.0f);

	// not using by point light, this param using directional light
	//m_light.dir	= XMFLOAT3(-0.25f, 0.0f, 0.5f);
	m_light.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	return true;
}


bool DXRenderer::InitDirectWrite(IDXGIAdapter1* adapter)
{
	if (S_OK != D3D10CreateDevice1(adapter, D3D10_DRIVER_TYPE_HARDWARE, NULL,
		D3D10_CREATE_DEVICE_DEBUG | D3D10_CREATE_DEVICE_BGRA_SUPPORT,
		D3D10_FEATURE_LEVEL_9_3, D3D10_1_SDK_VERSION, &m_d3dDevice10))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC sharedTexDesc;
	ZeroMemory(&sharedTexDesc, sizeof(sharedTexDesc));

	sharedTexDesc.Width = m_screenWidth;
	sharedTexDesc.Height = m_screenHeight;
	sharedTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sharedTexDesc.MipLevels = 1;
	sharedTexDesc.ArraySize = 1;
	sharedTexDesc.SampleDesc.Count = 1;
	sharedTexDesc.Usage = D3D11_USAGE_DEFAULT;
	sharedTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	sharedTexDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;

	if (S_OK != m_d3dDevice->CreateTexture2D(&sharedTexDesc, NULL, &m_sharedTexture))
	{
		return false;
	}

	// get the keyed mutex for the shared texture (for d3d11)
	if (S_OK != m_sharedTexture->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&m_keyedMutex11))
	{
		return false;
	}

	// get the shared handle needed to open the shared texture d3d10.1
	IDXGIResource* sharedResource10;
	HANDLE shareHandle10;

	if (S_OK != m_sharedTexture->QueryInterface(__uuidof(IDXGIResource), (void**)&sharedResource10))
	{
		return false;
	}

	if (S_OK != sharedResource10->GetSharedHandle(&shareHandle10))
	{
		return false;
	}

	sharedResource10->Release();

	// Open the surface for the shared texture in d3d10.1
	IDXGISurface1* sharedSurface10;
	if (S_OK != m_d3dDevice10->OpenSharedResource(shareHandle10, __uuidof(IDXGISurface1), (void**)&sharedSurface10))
	{
		return false;
	}

	if (S_OK != sharedSurface10->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&m_keyedMutex10))
	{
		return false;
	}

	// create d2d factory
	ID2D1Factory* d2dFactory;
	if (S_OK != D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), (void**)&d2dFactory))
	{
		return false;
	}


	D2D1_RENDER_TARGET_PROPERTIES rendertargetProperties;
	ZeroMemory(&rendertargetProperties, sizeof(rendertargetProperties));

	rendertargetProperties.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;
	rendertargetProperties.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);
	
	if (S_OK != d2dFactory->CreateDxgiSurfaceRenderTarget(sharedSurface10, &rendertargetProperties, &m_d2dRenderTarget))
	{
		return false;
	}

	sharedSurface10->Release();
	d2dFactory->Release();

	// create a solid color brush to draw something with
	if (S_OK != m_d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), &m_currentBrush))
	{
		return false;
	}

	// direct write
	if (S_OK != DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&m_dwriteFactory)))
	{
		return false;
	}

	if (S_OK != m_dwriteFactory->CreateTextFormat(
		L"Script",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		24.0f,
		L"en-us",
		&m_dwriteTextFormat))
	{
		return false;
	}

	if (S_OK != m_dwriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING))
	{
		return false;
	}

	if (S_OK != m_dwriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR))
	{
		return false;
	}

	m_d3dDevice10->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	return true;
}

bool DXRenderer::SetVertexBuffer(unsigned int byteWidth, 
								unsigned int structSize, 
								const void* vtxList,
								int sumProperty)
{
	assert(byteWidth > 0  && structSize > 0);

	// describe vertex buffer
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = byteWidth;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;//D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;


	D3D11_SUBRESOURCE_DATA resource;
	ZeroMemory(&resource, sizeof(D3D11_SUBRESOURCE_DATA));

	resource.pSysMem = vtxList;

	sVtxBufferItem vtxItem;
	ZeroMemory(&vtxItem, sizeof(vtxItem));


	HRESULT result = m_d3dDevice->CreateBuffer(&desc, &resource, &vtxItem.vtxBuffer);
	if (FAILED(result))
	{
		return false;
	}

	vtxItem.stride = structSize;
	vtxItem.vtxCount = byteWidth / structSize;
	m_vtxBufferList.push_back(vtxItem);

	if (m_vtxBuffers)
	{
		delete [] m_vtxBuffers;
	}

	int index = 0;
	m_vtxBuffers = new ID3D11Buffer* [(int)m_vtxBufferList.size()];
	for (auto itor = m_vtxBufferList.begin(); itor != m_vtxBufferList.end(); ++itor)
	{
		m_vtxBuffers[index++] = (*itor).vtxBuffer;
	}

	unsigned int offset = 0;
	m_d3dDeviceContext->IASetVertexBuffers(0, (int)m_vtxBufferList.size(), m_vtxBuffers, &vtxItem.stride, &offset);
	m_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

bool DXRenderer::SetIndexBuffer(unsigned int byteWidth, unsigned int structSize, const void* idxList)
{
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = byteWidth;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = idxList;
	m_d3dDevice->CreateBuffer(&indexBufferDesc, &initData, &m_idxBuffer);

	m_d3dDeviceContext->IASetIndexBuffer(m_idxBuffer, DXGI_FORMAT_R16_UINT, 0);

	m_indexCount = byteWidth / structSize;

	return true;
}

//std::wstring g_text(L"Hello world");
XMMATRIX g_lt;

const float g_deepBlue[4] = { 0.0f, 0.0f, 0.4f, 1.0f };
void DXRenderer::Render()
{
	//static unsigned int offset =  0;
	m_d3dDeviceContext->ClearRenderTargetView(m_renderView, g_deepBlue);

	// Refresh the Depth / Stencil View
	m_d3dDeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	XMVECTOR dir = XMVectorZero();
	dir = XMVector3TransformCoord(dir, g_lt);

	m_light.dir.x = dir.m128_f32[0];
	m_light.dir.y = dir.m128_f32[1];
	m_light.dir.z = dir.m128_f32[2];

	// set light
	m_cbPerFrame.light = m_light;
	m_d3dDeviceContext->UpdateSubresource(m_cbPerFrameBuffer, 0, NULL, &m_cbPerFrame, 0, 0);
	m_d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_cbPerFrameBuffer);

	// reset vertex and pixel shaders
	m_d3dDeviceContext->VSSetShader(m_VShader, 0, 0);
	m_d3dDeviceContext->PSSetShader(m_PShader, 0, 0);


	// set render target
	m_d3dDeviceContext->OMSetRenderTargets(1, &m_renderView, m_depthStencilView);

	// set the default blend state(no blending) for opaque objects
	m_d3dDeviceContext->OMSetBlendState(0, 0, 0xffffffff);

	// set the cubes index buffer
	m_d3dDeviceContext->IASetIndexBuffer(m_idxBuffer, DXGI_FORMAT_R16_UINT, 0);

	// st the cubes vertex buffer
	sVtxBufferItem& item = m_vtxBufferList[0];
	UINT offset = 0;
	m_d3dDeviceContext->IASetVertexBuffers(0, 1, &item.vtxBuffer, &item.stride, &offset);


	// render opaque object
	// enable the default Rasterizer State
	m_d3dDeviceContext->RSSetState(NULL);
	// draw objects that will use backface culling
	// turn off backface culling
	m_d3dDeviceContext->RSSetState(m_noCull);

	// "fine-tune" the blending equation
	float blendFactor[] = {0.75f, 0.75f, 0.75f, 1.0f};


	// set the blend state for tranparent object
	m_d3dDeviceContext->OMSetBlendState(m_transparency, blendFactor, 0xffffffff);

	// @@@ transparency depth ordering @@@
	// Find which trasparent object is further from the camera
	// So This can render the objects in depth order to the render state
	XMVECTOR cubePos = XMVectorZero();
	cubePos = XMVector3TransformCoord(cubePos, m_renderObj[0]);

	float distX = XMVectorGetX(cubePos) - XMVectorGetX(m_camPosition);
	float distY = XMVectorGetY(cubePos) - XMVectorGetY(m_camPosition);
	float distZ = XMVectorGetZ(cubePos) - XMVectorGetZ(m_camPosition);

	float cube1Dist = distX*distX + distY*distY + distZ*distZ;

	// find distance from second cube to camera
	cubePos = XMVectorZero();
	cubePos = XMVector3TransformCoord(cubePos, m_renderObj[1]);

	distX = XMVectorGetX(cubePos) - XMVectorGetX(m_camPosition);
	distY = XMVectorGetY(cubePos) - XMVectorGetY(m_camPosition);
	distZ = XMVectorGetZ(cubePos) - XMVectorGetZ(m_camPosition);

	float cube2Dist = distX*distX + distY*distY + distZ*distZ;

	if (cube1Dist < cube2Dist)
	{
		// switch the order in which the cubes are drawn
		XMMATRIX tempMatrix = m_renderObj[0];
		m_renderObj[0] = m_renderObj[1];
		m_renderObj[1] = tempMatrix;
	}


	// set the World, View, Projection
	m_WVP = m_renderObj[0] * m_view * m_projection;

	m_cbPerObj.World = XMMatrixTranspose(m_renderObj[0]);

	m_cbPerObj.WVP = XMMatrixTranspose(m_WVP);
	m_d3dDeviceContext->UpdateSubresource(m_cbPerObjectBuffer, 0, NULL, &m_cbPerObj, 0, 0);
	m_d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cbPerObjectBuffer);

	m_d3dDeviceContext->PSSetShaderResources(0, 1, &m_texture);
	m_d3dDeviceContext->PSSetSamplers(0, 1, &m_texSamplerState);

	// @@@ counter clockwise culling first because we need the back side of
	// the cube to be rendered first, so the front side can blend with it
	m_d3dDeviceContext->RSSetState(m_CCWCullMode);
	m_d3dDeviceContext->DrawIndexed(m_indexCount, 0, 0);

	m_d3dDeviceContext->RSSetState(m_CWCullMode);
	m_d3dDeviceContext->DrawIndexed(m_indexCount, 0, 0);

	
	// draw Second Obj
	m_WVP = m_renderObj[1] * m_view * m_projection;

	m_cbPerObj.World = XMMatrixTranspose(m_renderObj[1]);

	m_cbPerObj.WVP = XMMatrixTranspose(m_WVP);
	m_d3dDeviceContext->UpdateSubresource(m_cbPerObjectBuffer, 0, NULL, &m_cbPerObj, 0, 0);
	m_d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cbPerObjectBuffer);

	m_d3dDeviceContext->PSSetShaderResources(0, 1, &m_texture);
	m_d3dDeviceContext->PSSetSamplers(0, 1, &m_texSamplerState);

	m_d3dDeviceContext->RSSetState(m_CCWCullMode);
	m_d3dDeviceContext->DrawIndexed(m_indexCount, 0, 0);


	m_d3dDeviceContext->RSSetState(m_CWCullMode);
	m_d3dDeviceContext->DrawIndexed(m_indexCount, 0, 0);

	std::wostringstream ss;
	ss << L"FPS : " << m_fps;
	std::wstring text = ss.str();
	RenderText(text);

	m_d3dSwapChain->Present(0, 0);
}

void DXRenderer::RenderText(const std::wstring text)
{
	// release the d3d 11 device
	m_keyedMutex11->ReleaseSync(0);

	// use d3d10.1 device
	m_keyedMutex10->AcquireSync(0, 5);

	//draw d2d content
	m_d2dRenderTarget->BeginDraw();

	// clear d2d background
	m_d2dRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));

	// create string

	// set the font color
	D2D1_COLOR_F fontColor = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);

	// set the brush color d2d will use the draw with
	m_currentBrush->SetColor(fontColor);

	// create the d2d render area
	D2D1_RECT_F layoutRect = D2D1::RectF(0, 0, (float)m_screenWidth, (float)m_screenHeight);

	// draw the text
	m_d2dRenderTarget->DrawText(
		text.c_str(),
		wcslen(text.c_str()),
		m_dwriteTextFormat,
		layoutRect,
		m_currentBrush);

	m_d2dRenderTarget->EndDraw();

	// release the d3d10.1 device
	m_keyedMutex10->ReleaseSync(1);
	
	// use the d3d11 device
	m_keyedMutex11->AcquireSync(1, 5);


	// use the shader resource representing the direct2d render target
	// to texture  square which is rendered in screen space so it
	// overlays on top of our entire scene. We use alpha blending so
	// that the entire background of the D2D render target is "invisible"
	// And only the stuff we draw with D2D will be visible (the text)

	// set the blend state for D2d redner target texture objects
	m_d3dDeviceContext->OMSetBlendState(m_transparency, NULL, 0xffffffff);

	// set the d2d index buffer
	m_d3dDeviceContext->IASetIndexBuffer(m_d2dIdxBuffer, DXGI_FORMAT_R16_UINT, 0);

	// set the d2d vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset  = 0;
	m_d3dDeviceContext->IASetVertexBuffers(0, 1, &m_d2dVtxBuffer, &stride, &offset);

	m_WVP = XMMatrixIdentity();
	m_cbPerObj.World = XMMatrixTranspose(m_WVP);
	m_cbPerObj.WVP = XMMatrixTranspose(m_WVP);
	m_d3dDeviceContext->UpdateSubresource(m_cbPerObjectBuffer, 0, NULL, &m_cbPerObj, 0, 0);
	m_d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cbPerObjectBuffer);
	m_d3dDeviceContext->PSSetShaderResources(0, 1, &m_d2dTexture);
	m_d3dDeviceContext->PSSetSamplers(0, 1, &m_texSamplerState);

	m_d3dDeviceContext->RSSetState(m_CWCullMode);

	// draw the second cube
	m_d3dDeviceContext->DrawIndexed(6, 0, 0);
}

void DXRenderer::Process(const NtFloat& delta)
{
	m_rot += 1.0f * delta;
	if (m_rot > 6.26f)
	{
		m_rot = 0.0f;
	}

	// rotaion z axis

	//m_light.dir.z += 1.0f * delta;
	//if (m_light.dir.z > 3.14f)
	//{
	//	m_light.dir.z = 0.0f;
	//}

	//XMVECTOR rotY = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//XMMATRIX tpRot = XMMatrixIdentity();
	//XMMATRIX tpTran = XMMatrixIdentity();
	//tpRot = XMMatrixRotationAxis(rotY, m_rot);
	//tpTran = XMMatrixTranslation(-0.25f, 0.0f, 0.5f);

	//g_lt = tpTran * tpRot;


	// cube1
	XMMATRIX& cube1 = m_renderObj[0];
	cube1 = XMMatrixIdentity();

	XMVECTOR rotAxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_rotation = XMMatrixRotationAxis(rotAxis, m_rot);
	m_translation = XMMatrixTranslation(0.0f, 0.0f, 4.0f);

	cube1 = m_translation * m_rotation;

	// cube2
	XMMATRIX& cube2 = m_renderObj[1];
	cube2 = XMMatrixIdentity();

	m_rotation = XMMatrixRotationAxis(rotAxis, -m_rot);
	m_scale = XMMatrixScaling(1.3f, 1.3f, 1.3f);

	cube2 = m_rotation * m_scale;

	XMVECTOR lightVec = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	lightVec = XMVector3TransformCoord(lightVec, cube1);
	m_light.pos.x = XMVectorGetX(lightVec);
	m_light.pos.y = XMVectorGetY(lightVec);
	m_light.pos.z = XMVectorGetZ(lightVec);
}


//
HRESULT CompileShader(LPCWSTR fileName, char* entryPoint, char* shaderVersion, ID3DBlob** blobPtr)
{
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* errBlob = NULL;
	HRESULT result = D3DX11CompileFromFile(fileName, 
		NULL,
		NULL,
		entryPoint,
		shaderVersion,
		shaderFlags,
		0, 
		0, 
		blobPtr,
		&errBlob,
		0);

	if (FAILED(result))
	{
		if (NULL != errBlob)
		{
			OutputDebugString((LPCWSTR)errBlob->GetBufferPointer());
			errBlob->Release();
		}

		return S_FALSE;
	}

	if (NULL != errBlob)
	{
		errBlob->Release();
	}

	return S_OK;
}

}	// namespace _dx11
}	// namespace NT