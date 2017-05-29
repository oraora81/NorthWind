
#include "NtCoreLib.h"
#include "NtRenderer.h"

#include "NtD3DRenderer.h"
#include "NtCamera.h"
#include "NtPuppet.h"
#include "NtColorShader.h"
#include "NtTextureShader.h"
#include "NtLightShader.h"
#include "NtLight.h"

#include "NtModelParser.h"
#include "NtAse.h"


namespace nt { namespace renderer {


bool VSYNC_ENABLED = true;

NtRenderer::NtRenderer()
	: m_renderEngine(nullptr)
	, m_camera(nullptr)
	, m_puppet(nullptr)
	, m_colorShader(nullptr)
	, m_textureShader(nullptr)
	, m_lightShader(nullptr)
	, m_light(nullptr)
{

}

NtRenderer::~NtRenderer()
{
	SAFE_DELETE(m_light);
	Release();
	SAFE_DELETE(m_camera);
}


bool NtRenderer::Initialize(HWND hwnd, ntInt width, ntInt height)
{
	m_renderEngine = std::shared_ptr<NtDx11Renderer>(new NtDx11Renderer());
	NtAsserte(m_renderEngine != nullptr);

	bool res = m_renderEngine->Initialize(width, height, hwnd, VSYNC_ENABLED, false, false, 1000.0f, 0.1f);
	if (res == false)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	g_renderer = m_renderEngine;


	// create the camera object
	m_camera = new NtCamera;
	NtAsserte(m_camera != nullptr);

	// set the initial position of the camera
	m_camera->SetPosition(0.0f, 20.0f, -100.0f);

	
	// create the light shader object
	//m_lightShader = new NtLightShader;
	//NtAsserte(m_lightShader != nullptr);

	// init the light shader obj
	//res = m_lightShader->Initialize(m_renderEngine, hwnd, L"light_vs.hlsl", L"light_ps.hlsl");
	//if (false == res)
	//{
	//	MessageBox(hwnd, L"Could not initialize the light shader object", L"Error", MB_OK);
	//	return false;
	//}

	// create the light obj
	//m_light = new NtLight;
	//NtAsserte(m_light != nullptr);

	//m_light->SetDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	//m_light->SetDirection(0.0f, 0.0f, 1.0f);

	// create the color shader object
	m_colorShader = new NtColorShader;
	NtAsserte(m_colorShader != nullptr);

	res = m_colorShader->Initialize(L"color_vs_hlsl", L"color_ps_hlsl");
	if (res == false)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object", L"Error", MB_OK);
		return false;
	}

	// create the texture shader object
	m_textureShader = new NtTextureShader;
	NtAsserte(m_textureShader != nullptr);

	// initialize the texture shader object
	res = m_textureShader->Initialize(L"texture_vs.hlsl", L"texture_ps.hlsl");
	if (false == res)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object", L"Error", MB_OK);
		return false;
	}

	// create the puppet model object
	m_puppet = new NtPuppet;
	NtAsserte(m_puppet != nullptr);

	res = m_puppet->Initialize(L"test_cube.ase");
	if (res == false)
	{
		MessageBox(hwnd, L"Could not initialize the puppet object", L"Error", MB_OK);
		return false;
	}

	//m_puppet->SetLightShader(m_lightShader);
	m_puppet->SetColorShader(m_colorShader);
	m_puppet->SetTextureShader(m_textureShader);

	return true;
}


template <typename MEM_OBJ>
void WholeReleaseObject(MEM_OBJ& obj)
{
	NtAsserte(NtIsPtr<MEM_OBJ>::Result == TRUE);
	if (obj)
	{
		obj->Release();
		SAFE_DELETE(obj);
	}
}

void NtRenderer::Release()
{
	//WholeReleaseObject(m_lightShader);
	WholeReleaseObject(m_textureShader);
	WholeReleaseObject(m_colorShader);
	WholeReleaseObject(m_puppet);
}

bool NtRenderer::Process()
{
	static ntFloat rot = 0.0f;

	// update the rotation variable each frame
	rot += NtMath<ntFloat>::PI * 0.005f;
	if (rot > 360.0f)
	{
		rot = 0.0f;
	}


	if (!Render(rot))
	{
		return false;
	}

	return true;
}

bool NtRenderer::Render(ntFloat rot)
{
	XMMATRIX worldMatrix;
	XMMATRIX viewMatrix;
	XMMATRIX projMatrix;

	// clear the buffers to begin the scene
	// r, g, b
	m_renderEngine->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// generate the view matrix based on the camera and d3d objects
	m_camera->Render();

	// get the world, view and proj matrices from the camera and d3d objects
	m_camera->GetViewMatrix(viewMatrix);
	m_renderEngine->GetWorldMatrix(worldMatrix);
	m_renderEngine->GetProjectionMatrix(projMatrix);

	//////////////////////////////////////////////////////////////////////////

	// rotate the world matrix by the rotation value so that the triangle will spin
	//D3DXMatrixRotationZ(&worldMatrix, rot);
	//D3DXMatrixRotationY(&worldMatrix, rot);

	//////////////////////////////////////////////////////////////////////////


	// put the puppet vertex and index buffers on the graphics pipeline to prepare them for drawing
	m_puppet->Render(worldMatrix, viewMatrix, projMatrix, m_light);

		
	//NtPoint p1(-10.0f, 0.0f);
	//NtPoint p2(10.0f, 0.0f);

	//m_colorShader->RenderLine(m_renderEngine, worldMatrix, viewMatrix, projMatrix);

	// present the rendered scene to the screen
	m_renderEngine->EndScene();

	return true;
}

void NtRenderer::Resize(ntInt width, ntInt height)
{
	bool result =  m_renderEngine->Resize(width, height);
	if (result == false)
	{

	}
}


bool NtRenderer::CreateTexture1D(D3D11_TEXTURE1D_DESC* desc,D3D11_SUBRESOURCE_DATA* initData,ID3D11Texture1D** tex)
{
	HRESULT res = m_renderEngine->Device()->CreateTexture1D(desc, initData, tex);
	
	return FAILED(res) ? false : true;
}

bool NtRenderer::CreateTexture2D(D3D11_TEXTURE2D_DESC* desc,D3D11_SUBRESOURCE_DATA* initData,ID3D11Texture2D** tex)
{
	HRESULT res = m_renderEngine->Device()->CreateTexture2D(desc, initData, tex);

	return FAILED(res) ? false : true;
}

bool NtRenderer::CreateTexture3D(D3D11_TEXTURE3D_DESC* desc,D3D11_SUBRESOURCE_DATA* initData,ID3D11Texture3D** tex)
{
	HRESULT res = m_renderEngine->Device()->CreateTexture3D(desc, initData, tex);

	return FAILED(res) ? false : true;
}

int NtRenderer::GetFeatureLevel()
{
	return m_renderEngine->Device()->GetFeatureLevel();
}

std::shared_ptr<NtDx11Renderer>& NtRenderer::GetD3DRenderer()
{
	return m_renderEngine;
}

//bool NtRenderer::CreateBuffer(const D3D11_BUFFER_DESC& desc,D3D11_SUBRESOURCE_DATA* resData,ID3D11Buffer** buffer)
bool NtRenderer::CreateBuffer(NtRenderBufferParam& param)
{
	HRESULT res = m_renderEngine->Device()->CreateBuffer(param.m_desc, param.m_resData, param.m_buffer);

	return FAILED(res) ? false : true;
}


void NtRenderer::SetPrimitiveTopology(ntInt topology)
{
	m_renderEngine->DeviceContext()->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)topology);
}


void NtRenderer::SetVertexBuffers(ntUint startSlot, ntUint numBuffers, NtVertexBuffer** buffer, ntUint* stride, ntUint* offset)
{
	m_renderEngine->DeviceContext()->IASetVertexBuffers(startSlot, numBuffers, buffer, stride, offset);
}

void NtRenderer::SetIndexBuffers(NtIndexBuffer* buffer, NTCOLORFMT format, ntUint offset)
{
	m_renderEngine->DeviceContext()->IASetIndexBuffer(buffer, format, offset);
}


} // namespace renderer

} // namespace nt
