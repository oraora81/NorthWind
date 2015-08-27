
#include "NtCoreLib.h"
#pragma hdrstop

#include "NtD3DRenderer.h"
#include "NtD3DMapping.h"

#ifdef _NT_DX9

namespace nt { namespace renderer {


NtDirectX9Renderer::NtDirectX9Renderer()
	: m_driver(nullptr)
	, m_deviceLost(false)
	, m_systemFont(nullptr)
{

}


NtDirectX9Renderer::~NtDirectX9Renderer()
{

}


bool NtDirectX9Renderer::Initialize(int width, int height, HWND hwnd, bool vsync, bool fullscreen, float screenDepth, float screenNear)
{
	m_hwnd = hwnd;
	m_width = width;
	m_height = height;
	m_vsync = vsync;
	m_windowMode = fullscreen == true ? false : true;
	m_driver = Direct3DCreate9(D3D_SDK_VERSION);

	m_colorFormat = g_colorFormat[eColorFormat::NT_FMT_A8R8G8B8];
	m_depthStencilFormat = g_colorFormat[eColorFormat::NT_D24S8];

	if (false == EnumAdapterModes()
	{
		return false;
	}

	if (false == CreateDevice())
	{
		return false;
	}

	return true;
}


void NtDirectX9Renderer::Release()
{

}


void NtDirectX9Renderer::BeginScene(float r, float g, float b, float a)
{
	m_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
}


void NtDirectX9Renderer::EndScene()
{
	m_device->Present(0, 0, 0, 0);
}


bool NtDirectX9Renderer::EnumAdapterModes()
{
	const NTCOLORFMT allowedColorFormatArray[] = 
	{
		g_colorFormat[eColorFormat::NT_FMT_A8R8G8B8];
	}
	ntUint numAllowedColorFormat = _countof(allowedColorFormatArray);

	std::vector<D3DDISPLAYMODE> displayModeVector;

	ntUint adapterCount = m_driver->GetAdapterCount();
	for (ntUint i = 0; i < adapterCount; ++i)
	{
		D3DADAPTER_IDENTIFIER9 adapterId;
		m_driver->GetAdapterIdentifier(i, 0, &adapterId);

		displayModeVector.clear();
		for( ntUint formatIdx = 0; formatIdx < numAllowedColorFormat; ++formatIdx )
		{
			NTCOLORFMT colorFormat = allowedColorFormatArray[formatIdx];
			ntUint adapterMode = m_driver->GetAdapterModeCount(i, colorFormat);
			for (ntUint mode = 0; mode < adapterMode; ++mode)
			{
				D3DDISPLAYMODE displayMode;
				m_driver->EnumAdapterModes( i, colorFormat, mode, &displayMode );

				if((displayMode.Width < m_width) || (displayMode.Height < m_height))
				{
					continue;
				}

				displayModeVector.push_back( displayMode );
			}
		}

		if (displayModeVector.empty() == false)
		{
			break;
		}
	}

	m_refreshRate =	displayModeVector[0].RefreshRate;

	return true;
}


bool NtDirectX9Renderer::CreateDevice()
{
	D3DPRESENT_PARAMETERS present;

	present.BackBufferWidth = m_width;
	present.BackBufferHeight = m_height;
	present.BackBufferFormat = m_colorFormat;
	present.BackBufferCount = 1;
	present.hDeviceWindow = m_hwnd;
	present.Windowed = m_windowMode;
	present.Flags = 0;
	present.FullScreen_RefreshRateInHz = 0;
	present.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Request depth and stencil buffers.
	present.EnableAutoDepthStencil = TRUE;
	present.AutoDepthStencilFormat = m_depthStencilFormat;

	// Request multisampling.
	present.SwapEffect = D3DSWAPEFFECT_FLIP; 
	present.MultiSampleType = D3DMULTISAMPLE_NONE;
	present.MultiSampleQuality = 0;

	HRESULT res;
	if (m_multiSamples == 2)
	{
		res = m_driver->CheckDeviceMultiSampleType(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			m_colorFormat,
			FALSE,
			D3DMULTISAMPLE_2_SAMPLES,
			NULL);

		NtAssert(res == D3D_OK, "Two multisamples not supported: %s\n",
			DXGetErrorString(res));

		if (res == D3D_OK)
		{
			present.SwapEffect = D3DSWAPEFFECT_DISCARD; 
			present.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
		}
	}
	else if (m_multiSamples == 4)
	{
		res = m_driver->CheckDeviceMultiSampleType(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			m_colorFormat,
			FALSE,
			D3DMULTISAMPLE_4_SAMPLES,
			NULL);

		NtAsserte("Two multisamples not supported: %s\n", DXGetErrorString(res));

		if (res == D3D_OK)
		{
			present.SwapEffect = D3DSWAPEFFECT_DISCARD; 
			present.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
		}
	}

	// create d3d device
	res = m_driver->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE,
		&present,
		&m_device);

	if (D3D_OK != res)
	{
		NtAsserte("Failed to create device: %s\n", DXGetErrorString(res));
		return false;
	}


	// create system font
	if (CreateSystemFont() == false)
	{
		return false;
	}

	// query device capability
	D3DCAPS9 deviceCaps;
	res = m_driver->GetDeviceCaps(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		&deviceCaps);

	if (res != D3D_OK)
	{
		NtAsserte("GetDeviceCaps failed: %s\n", DXGetErrorString(res));
		return false;
	}

	// Set Shader Model
	ntUint vsModel = deviceCaps.VertexShaderVersion;
	if (vsModel < D3DVS_VERSION(1,1))
	{
		NtAsserte("Vertex Shader Model must be at least 1.1.\n");
		return false;
	}

	if (vsModel >= D3DVS_VERSION(3,0))
	{
		NtAsserte("Vertex Shader Model must be at most 3.0f\n");
		vsModel = D3DVS_VERSION(3,0);
	}
	else if (vsModel == D3DVS_VERSION(2,0))
	{
		vsModel = D3DVS_VERSION(2,0);
	}
	else
	{
		vsModel = D3DVS_VERSION(1,1);
	}

	ntUint psModel = deviceCaps.PixelShaderVersion;
	if (psModel < D3DPS_VERSION(1,1))
	{
		NtAsserte("Pixel Shader Model must be at least 1.1.\n");
		return false;
	}

	if (psModel > D3DPS_VERSION(3,0))
	{
		NtAsserte("Pixel Shader Model must be at most 3.0f\n");
		psModel = D3DPS_VERSION(3,0);
	}
	else if (psModel == D3DPS_VERSION(2,0))
	{
		psModel = D3DPS_VERSION(2,0);
	}
	else
	{
		psModel = D3DPS_VERSION(1,1);
	}

	// set vs shader model
	if (vsModel == D3DVS_VERSION(1,1))
	{
		NtShader::SetVSShaderModel(nt::NT_VS_1_1);
	}
	else if (vsModel == D3DVS_VERSION(2,0))
	{
		NtShader::SetVSShaderModel(nt::NT_VS_2_0);
	}
	else
	{
		NtShader::SetVSShaderModel(nt::NT_VS_3_0);
	}

	// set ps shader model
	if (psModel == D3DPS_VERSION(1,1))
	{
		NtShader::SetPSShaderModel(nt::NT_PS_1_1);
	}
	else if (vsModel == D3DVS_VERSION(2,0))
	{
		NtShader::SetVSShaderModel(nt::NT_VS_2_0);
	}
	else
	{
		NtShader::SetVSShaderModel(nt::NT_VS_3_0);
	}


	return true;
}

bool NtDirectX9Renderer::CreateSystemFont()
{
	HRESULT res = D3DXCreateFont(
		m_device,             // pDevice
		18,                   // Height
		0,                    // Width
		FW_REGULAR,           // Weight
		0,                    // MipLevels
		0,                    // Italic
		DEFAULT_CHARSET,      // CharSet
		OUT_DEFAULT_PRECIS,   // OutputPrecision
		ANTIALIASED_QUALITY,  // Quality
		VARIABLE_PITCH,       // PitchAndFamily
		"Arial",              // pFaceName
		&m_systemFont);       // ppFont

	if (res != D3D_OK)
	{
		NtAssert(hr == D3D_OK, "D3DXCreateFont failed: %s\n",
			DXGetErrorString(hr));
		return false;
	}

	return true;
}


}	// namespace renderer
}	// namespace nt

#endif
