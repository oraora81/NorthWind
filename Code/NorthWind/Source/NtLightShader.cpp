
#include "NtCoreLib.h"
#pragma  hdrstop

#include "NtShader.h"
#include "NtLightShader.h"
#include "NtTexture.h"

namespace nt { namespace renderer {

NtLightShader::NtLightShader()
	: NtShader()
    , m_sampleState(nullptr)
	, m_matrixBuffer(nullptr)
	, m_lightBuffer(nullptr)
{

}


NtLightShader::~NtLightShader()
{
	Release();
}

bool NtLightShader::InitializeFx(const ntWchar* fx)
{
    NtAsserte(fx != nullptr);
    if (NtShader::InitializeFx(fx) == false)
    {
        return false;
    }

    m_light1 = m_fx->GetTechniqueByName("Light1");
    m_light2 = m_fx->GetTechniqueByName("Light2");
    m_light3 = m_fx->GetTechniqueByName("Light3");
    m_fxWorldViewProj = m_fx->GetVariableByName("gWorldViewProj")->AsMatrix();
    m_fxWorld = m_fx->GetVariableByName("gWorld")->AsMatrix();
    m_fxWorldInvTranspose = m_fx->GetVariableByName("gWorldInvTranspose")->AsMatrix();
    m_fxEyePosW = m_fx->GetVariableByName("gEyePosW")->AsVector();
    m_fxDirLight = m_fx->GetVariableByName("gDirLights");
    m_fxMaterial = m_fx->GetVariableByName("gMaterial");

    return true;
}

void NtLightShader::Release()
{
    SAFE_RELEASE(m_light1);
    SAFE_RELEASE(m_light2);
    SAFE_RELEASE(m_light3);
    SAFE_RELEASE(m_fxWorldViewProj);

    NtShader::Release();
}

bool NtLightShader::RenderFx(ntInt indexCount, const XMMATRIX& worldViewProj)
{
    return true;
}

bool NtLightShader::SetShaderParameter(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj, NtTexture* texture, const XMFLOAT3A& lightDir, const XMFLOAT4A& diffuse)
{
	// transpose the matrices to prepare them for the shader
	XMMATRIX w;
	XMMATRIX v;
	XMMATRIX p;

	w = XMMatrixTranspose(world);
	v = XMMatrixTranspose(view);
	p = XMMatrixTranspose(proj);

	// lock the constant buffer so it can be written to
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT res = g_renderer->DeviceContext()->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(res))
	{
		return false;
	}

	// get a pointer to the data in the constant buffer
	NtMatrixBuffer* dataMatrix = nullptr;
	dataMatrix = (NtMatrixBuffer*)mappedResource.pData;

	// copy the matrices into the constant buffer
	dataMatrix->world = w;
	dataMatrix->view = v;
	dataMatrix->proj = p;

	// unlock the constant buffer
	g_renderer->DeviceContext()->Unmap(m_matrixBuffer, 0);

	// set the position of the constant buffer in the vertex shader
	ntUint bufferNum = 0;

	// now set the constant buffer in the vertex shader with the updated values.
	g_renderer->DeviceContext()->VSSetConstantBuffers(bufferNum, 1, &m_matrixBuffer);

	// set shader texture resource in the pixel shader
	ID3D11ShaderResourceView* resourceView = texture->GetTexture();
	g_renderer->DeviceContext()->PSSetShaderResources(0, 1, &resourceView);


	// lock the light constant buffer so it can be written to
	res = g_renderer->DeviceContext()->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(res))
	{
		return false;
	}

	// get a pointer to the data in the constant buffer
	NtLightBuffer* dataLight = nullptr;
	dataLight = (NtLightBuffer*)mappedResource.pData;

	dataLight->diffuseColor = diffuse;
	dataLight->lightDirection = lightDir;
	dataLight->padding = 0.0f;

	// unlock the constant buffer
	g_renderer->DeviceContext()->Unmap(m_lightBuffer, 0);

	// set the position of the light constant buffer in the pixel shader
	bufferNum = 0;

	// finally set the light constant buffer in the pixel shader with the updated values.
	g_renderer->DeviceContext()->PSSetConstantBuffers(bufferNum, 1, &m_lightBuffer);

	return true;
}


void NtLightShader::RenderShader(int indexCount) const
{
	// set the vertex input layout.
	g_renderer->DeviceContext()->IASetInputLayout(m_layout);

	// set the vertex and pixel shaders that will be used to render this triangle.
	g_renderer->DeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	g_renderer->DeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

	// set the sampler state in the pixel shader
	g_renderer->DeviceContext()->PSSetSamplers(0, 1, &m_sampleState);

	// render the triangle
	g_renderer->DeviceContext()->DrawIndexed(indexCount, 0, 0);
}



}	// namespace renderer
}	// namespace nt
