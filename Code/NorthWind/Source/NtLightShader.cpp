
#include "NtCoreLib.h"
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

    nt::fs::NtFileBuffer fileBuffer(fx);

    HRF(D3DX11CreateEffectFromMemory(fileBuffer.GetData(), fileBuffer.GetBytes(), 0, g_renderer->Device(), &m_fx, nullptr));

    m_tech = m_fx->GetTechniqueByName("LightTech");

    m_fxWorldViewProj = m_fx->GetVariableByName("gWorldViewProj")->AsMatrix();
}

void NtLightShader::Release()
{
    NtShader::Release();
}


bool NtLightShader::Render(int indexCount, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj, NtTexture* texture, const XMFLOAT3A& lightDir, const XMFLOAT4A& diffuse)
{
	// set the shader parameters that it will use for rendering
	bool res = SetShaderParameter(world, view, proj, texture, lightDir, diffuse);
	if (false == res)
	{
		return  false;
	}

	// now render the prepared buffers with the shader
	RenderShader(indexCount);

	return true;
}

bool NtLightShader::RenderFx(ntInt indexCount, const XMMATRIX& worldViewProj)
{
    return true;
}


bool NtLightShader::InitializeShader(const ntWchar* vs, const ntWchar* ps)
{
	ID3D10Blob* errMsg = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	
	D3D11_INPUT_ELEMENT_DESC elementLayout[3];
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;

	
	// compile the vertex shader code
	HRESULT res = D3DX11CompileFromFile(g_resManager->GetPath(vs), NULL, NULL, "LightVertexShader", g_renderer->GetVShaderModel(), D3D10_SHADER_ENABLE_STRICTNESS, 0 , NULL, &vertexShaderBuffer, &errMsg, NULL);
	if (FAILED(res))
	{
		// if the shader failed to compile it should have written something to the error message
		if (errMsg)
		{
			g_renderer->OutputShaderErrorMessage(errMsg, vs);
		}
		else
		{
			MessageBox(nullptr, vs, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	res = D3DX11CompileFromFile(g_resManager->GetPath(ps), NULL, NULL, "LightPixelShader", g_renderer->GetPShaderModel(), D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errMsg, NULL);
	if (FAILED(res))
	{
		if (errMsg)
		{
			g_renderer->OutputShaderErrorMessage(errMsg, ps);
		}
		else
		{
			MessageBox(nullptr, ps, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	// create the vertex shader from the buffer
	res = g_renderer->Device()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(res))
	{
		return false;
	}

	// create the pixel shader from the buffer
	res = g_renderer->Device()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(res))
	{
		return false;
	}

	// create the vertex input layout desc
	// this setup needs to match the NtVertexType structure in the Puppet and in the shader
	elementLayout[0].SemanticName = "POSITION";
	elementLayout[0].SemanticIndex = 0;
	elementLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elementLayout[0].InputSlot = 0;
	elementLayout[0].AlignedByteOffset = 0;
	elementLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elementLayout[0].InstanceDataStepRate = 0;

	elementLayout[1].SemanticName = "TEXCOORD";
	elementLayout[1].SemanticIndex = 0;
	elementLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	elementLayout[1].InputSlot = 0;
	elementLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	elementLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elementLayout[1].InstanceDataStepRate = 0;

	elementLayout[2].SemanticName = "NORMAL";
	elementLayout[2].SemanticIndex = 0;
	elementLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elementLayout[2].InputSlot = 0;
	elementLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	elementLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elementLayout[2].InstanceDataStepRate = 0;

	// get a count of the elem in the layout
	int elementCount = _countof(elementLayout);

	// create the vertex input layout
	res = g_renderer->Device()->CreateInputLayout(elementLayout, elementCount, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(res))
	{
		return false;
	}

	// release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	SAFE_RELEASE(vertexShaderBuffer);
	SAFE_RELEASE(pixelShaderBuffer);

	// create a texture sampler state desc
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// create the texture sampler state
	res = g_renderer->Device()->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(res))
	{
		return false;
	}

	// setup the desc of the dynamic matrix constant buffer that is in the vertex shader
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(NtMatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// create the constant buffer pointer so we can access the vertex shader constant buffer from within this class
	res = g_renderer->Device()->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(res))
	{
		return false;
	}

	// setup the desc of the light dynamic constant buffer that is in the pixel shader
	// note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(NtLightBuffer);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// create the constant buffer pointer so we can access the vertex shader constant buffer from within this class
	res = g_renderer->Device()->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if (FAILED(res))
	{
		return false;
	}

	return true;
}

void NtLightShader::ReleaseShader()
{
	SAFE_RELEASE(m_lightBuffer);
	SAFE_RELEASE(m_matrixBuffer);
	SAFE_RELEASE(m_sampleState);
	SAFE_RELEASE(m_layout);
	SAFE_RELEASE(m_pixelShader);
	SAFE_RELEASE(m_vertexShader);
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
