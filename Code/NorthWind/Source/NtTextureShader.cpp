
#include "NtCoreLib.h"
#include "NtTextureShader.h"

#include "NtD3DRenderer.h"
#include "NtTexture.h"

namespace nt { namespace renderer {

NtTextureShader::NtTextureShader()
	: NtShader()
	, m_sampleState(nullptr)
{

}


NtTextureShader::~NtTextureShader()
{
	
}

bool NtTextureShader::InitializeFx(const ntWchar* fx)
{
	return true;
}


void NtTextureShader::Release()
{
	NtShader::Release();

	SAFE_RELEASE(m_sampleState);
}

void NtTextureShader::RenderShader(ntInt indexCount)
{
	// set the vertex input layout.
	g_renderer->DeviceContext()->IASetInputLayout(m_layout);

	// set the vertex and pixel shaders that will be used to render this triangle.
	g_renderer->DeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	g_renderer->DeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

	// set the sampler state in the pixel shader.
	g_renderer->DeviceContext()->PSSetSamplers(0, 1, &m_sampleState);

	// render the triangle
	g_renderer->DeviceContext()->DrawIndexed(indexCount, 0, 0);
}


}
}
