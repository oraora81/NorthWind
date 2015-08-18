
#include "NtCoreLib.h"
#include "NtD3DRenderer.h"

#include <fstream>

using namespace NT;
using namespace RENDERER;


NtD3DRenderer::NtD3DRenderer()
{
	m_videoCardMemory = 0;
	m_videoCardDescription = 0;
	m_multiSamples = 0;
	m_vsync = false;
	m_windowMode = false;
}

NtD3DRenderer::~NtD3DRenderer()
{
	
}


void NtD3DRenderer::GetWorldMatrix(XMMATRIX& world)
{
	world = m_world;
}


void NtD3DRenderer::GetProjectionMatrix(XMMATRIX& proj)
{
	proj = m_proj;
}


void NtD3DRenderer::GetOrthoMatrix(XMMATRIX& ortho)
{
	ortho = m_ortho;
}

void NtD3DRenderer::GetDeviceCardInfo(NtString& cardName, NtInt& memory)
{
	cardName = m_videoCardDescription;
	memory = m_videoCardMemory;
}

const NtChar* NtD3DRenderer::GetVShaderModel() const
{
	return m_shaderModel[0];
}

const NtChar* NtD3DRenderer::GetPShaderModel() const
{
	return m_shaderModel[1];
}

void NtD3DRenderer::OutputShaderErrorMessage(ID3D10Blob* errMsg, const NtWChar* fileName)
{
	NtChar* msg = reinterpret_cast<NtChar*>(errMsg->GetBufferPointer());

	// get the length of the msg
	NtInt bufferSize = errMsg->GetBufferSize();

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
