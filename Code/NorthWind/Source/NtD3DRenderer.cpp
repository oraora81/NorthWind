
#include "NtCoreLib.h"
#include "NtD3DRenderer.h"

#include <fstream>

namespace nt { namespace renderer {

XMMATRIX NtD3DRenderer::ms_identity = XMMatrixIdentity();


NtD3DRenderer::NtD3DRenderer()
{
	m_videoCardMemory = 0;
	m_videoCardDescription = 0;
	m_multiSamples = 0;
	m_vsync = false;
	m_windowMode = false;

	XMStoreFloat4x4(&m_world, ms_identity);
	XMStoreFloat4x4(&m_view, ms_identity);
	XMStoreFloat4x4(&m_proj, ms_identity);
}

NtD3DRenderer::~NtD3DRenderer()
{
	
}

void NtD3DRenderer::SetWorldMatrix(XMMATRIX& world)
{
	XMStoreFloat4x4(&m_world, world);
}

void NtD3DRenderer::SetViewMatrix(XMMATRIX& view)
{
	XMStoreFloat4x4(&m_view, view);
}

void NtD3DRenderer::SetProjectionMatrix(XMMATRIX& proj)
{
	XMStoreFloat4x4(&m_proj, proj);
}

void NtD3DRenderer::GetWorldMatrix(XMMATRIX& world)
{
	world = XMLoadFloat4x4(&m_world);
}

void NtD3DRenderer::GetViewMatrix(XMMATRIX& view)
{
	view = XMLoadFloat4x4(&m_view);
}

void NtD3DRenderer::GetProjectionMatrix(XMMATRIX& proj)
{
	proj = XMLoadFloat4x4(&m_proj);
}

void NtD3DRenderer::GetViewProjMatrix(XMMATRIX& viewProj)
{
    viewProj = XMLoadFloat4x4(&m_view) * XMLoadFloat4x4(&m_proj);
}

void NtD3DRenderer::Transform(XMMATRIX& tm)
{
	XMMATRIX world = XMLoadFloat4x4(&m_world);
	XMMATRIX view = XMLoadFloat4x4(&m_view);
	XMMATRIX proj = XMLoadFloat4x4(&m_proj);

	tm = world*view*proj;;
}

void NtD3DRenderer::GetOrthoMatrix(XMMATRIX& ortho)
{
	ortho = m_ortho;
}

void NtD3DRenderer::GetDeviceCardInfo(NtString& cardName, ntInt& memory)
{
	cardName = m_videoCardDescription;
	memory = m_videoCardMemory;
}

ntInt NtD3DRenderer::VideoMemory() const
{
	return m_videoCardMemory;
}

void NtD3DRenderer::OutputShaderErrorMessage(ID3D10Blob* errMsg, const ntWchar* fileName)
{
	ntChar* msg = reinterpret_cast<ntChar*>(errMsg->GetBufferPointer());

	// get the length of the msg
	ntSize bufferSize = errMsg->GetBufferSize();

	std::ofstream fout;

	fout.open("shader_err.txt");
	for (int i = 0; i < bufferSize; ++i)
	{
		fout << msg[i];
	}

	fout.close();

	SAFE_RELEASE(errMsg);

	// pop a message up on the screen to notify the user to check the text file for compile err
	MessageBox(nullptr, L"Error compiling shader. Check shader_err.txt for message", fileName, MB_OK);
}

}
}
