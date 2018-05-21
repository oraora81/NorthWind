
#include "NtCoreLib.h"
#include "NtRenderer.h"

#include "NtD3DRenderer.h"
#include "NtCamera.h"
#include "NtModel.h"
#include "NtLight.h"
#include "NtModelParser.h"
#include "NtAse.h"
#include "NtD3dUtil.h"
#include "NtColorShader.h"
#include "NtTextureShader.h"
#include "NtLightShader.h"
#include "NtShaderHandler.h"
#include "NtInputLayout.h"

namespace nt { namespace renderer {


bool VSYNC_ENABLED = true;

NtRenderer::NtRenderer()
	: m_renderEngine(nullptr)
	, m_camera(nullptr)
	, m_light(nullptr)
{

}

NtRenderer::~NtRenderer()
{
	SAFE_DELETE(m_light);
	Release();
	SAFE_DELETE(m_camera);
}


bool NtRenderer::Initialize(HWND hwnd, ntInt width, ntInt height, bool fullScreen)
{
	m_renderEngine = std::shared_ptr<NtDx11Renderer>(new NtDx11Renderer());
	NtAsserte(m_renderEngine != nullptr);

	bool res = m_renderEngine->Initialize(width, height, hwnd, VSYNC_ENABLED, fullScreen, false, 1000.0f, 0.1f);
	if (res == false)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

    g_renderer = m_renderEngine;

    // init shaders
    if (NtShaderHandler::Initialize() == false)
    {
        return false;
    }

    // init inputLayouts
    if (NtInputLayoutHandler::Initialize() == false)
    {
        MessageBox(hwnd, L"Input Layout initialize failed", L"Error", MB_OK);
        return false;
    }

	// create the camera object
	//m_camera = new NtCamera;
	//NtAsserte(m_camera != nullptr);

	// set the initial position of the camera
	//m_camera->SetPosition(0.0f, 20.0f, -100.0f);

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

}

bool NtRenderer::Process(const float deltaTime)
{
	auto itor = m_models.begin();
	for (; itor != m_models.end(); ++itor)
	{
		NtModel* model = (*itor);
		model->Update(deltaTime);
	}

	return true;
}

bool NtRenderer::Draw()
{
	m_renderEngine->BeginScene(reinterpret_cast<const float*>(&Colors::LightSteelBlue));

	XMMATRIX tm;
	m_renderEngine->Transform(tm);

	auto itor = m_models.begin();
	for (; itor != m_models.end(); ++itor)
	{
		NtModel* model = (*itor);
		model->Render(tm);
	}

	m_renderEngine->EndScene();

	return true;
}

bool NtRenderer::Resize(ntInt width, ntInt height)
{
	return m_renderEngine->Resize(width, height);
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

int NtRenderer::GetFeatureLevel() const
{
	return m_renderEngine->Device()->GetFeatureLevel();
}

bool NtRenderer::CreateBuffer(NtRenderBufferParam& param)
{
	HRESULT res = m_renderEngine->Device()->CreateBuffer(param.m_desc, param.m_resData, param.m_buffer);

	return FAILED(res) ? false : true;
}

void NtRenderer::SetPrimitiveTopology(ntInt topology)
{
	m_renderEngine->DeviceContext()->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)topology);
}

void NtRenderer::SetRenderState(eRenderState state)
{
	switch (state)
	{
	case eRenderState::NT_RS_SOLID:
        m_renderEngine->DeviceContext()->RSSetState(NtRenderStateHandler::RSSolid);
		break;
	case eRenderState::NT_RS_WIRE_FRAME:
        m_renderEngine->DeviceContext()->RSSetState(NtRenderStateHandler::RSWireFrame);
		break;
	}
}

void NtRenderer::SetVertexBuffers(ntUint startSlot, ntUint numBuffers, NtVertexBuffer** buffer, ntUint* stride, ntUint* offset)
{
	m_renderEngine->DeviceContext()->IASetVertexBuffers(startSlot, numBuffers, buffer, stride, offset);
}

void NtRenderer::SetIndexBuffers(NtIndexBuffer* buffer, NTCOLORFMT format, ntUint offset)
{
	m_renderEngine->DeviceContext()->IASetIndexBuffer(buffer, format, offset);
}

bool NtRenderer::AddModel(NtModel* model)
{
	m_models.push_back(model);

	return true;
}

} }

