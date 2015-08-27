
#include "NtCoreLib.h"
#include "NtTextureShader.h"

#include "NtD3DRenderer.h"
#include "NtTexture.h"

namespace nt { namespace renderer {

NtTextureShader::NtTextureShader()
	: m_vertexShader(nullptr)
	, m_pixelShader(nullptr)
	, m_layout(nullptr)
	, m_matrixBuffer(nullptr)
	, m_sampleState(nullptr)
{

}


NtTextureShader::~NtTextureShader()
{
	Release();
}

bool NtTextureShader::Initialize(const ntWchar* vs, const ntWchar* ps)
{
	// init the vertex and pixel shader
	bool res = InitializeShader(vs, ps);
	if (false == res)
	{
		return false;
	}

	return true;
}


void NtTextureShader::Release()
{
	ReleaseShader();
}


bool NtTextureShader::Render(ntInt indexCount, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj, ntUint handle)
{
	// set the shader parameter that it will use for rendering
	bool res = SetShaderParameter(world, view, proj, handle);
	if (false == res)
	{
		return false;
	}

	// now render the prepared buffers with the shader
	RenderShader(indexCount);

	return true;
}


bool NtTextureShader::InitializeShader(const ntWchar* vs, const ntWchar* ps)
{
	ID3D10Blob* errMsg = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;

	HRESULT res = D3DX11CompileFromFile(g_resManager->GetWholePath(vs), NULL, NULL, "TextureVertexShader", g_renderer->GetVShaderModel(), D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errMsg, NULL);
	if (FAILED(res))
	{
		// if the shader failed to compile it should have written something to the error msg
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

	
	// compile pixel shader
	res = D3DX11CompileFromFile(g_resManager->GetWholePath(ps), NULL, NULL, "TexturePixelShader", g_renderer->GetPShaderModel(), D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errMsg, NULL);
	if (FAILED(res))
	{
		if (errMsg)
		{
			g_renderer->OutputShaderErrorMessage(errMsg, ps);
		}
		else
		{
			MessageBox(g_app->Handle(), ps, L"Missing Shader File", MB_OK);
		}

		return false;
	}


	// create the vertex shader from the buffer
	
	res = g_renderer->Device()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(res))
	{
		return false;
	}

	res = g_renderer->Device()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(res))
	{
		return false;
	}

	// create the vertex input layout desc
	// this setup needs to match the VertexType structure in the Puppet and in the shader
	D3D11_INPUT_ELEMENT_DESC polyLayout[2];
	polyLayout[0].SemanticName = "POSITION";
	polyLayout[0].SemanticIndex = 0;
	polyLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polyLayout[0].InputSlot = 0;
	polyLayout[0].AlignedByteOffset = 0;
	polyLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polyLayout[0].InstanceDataStepRate = 0;

	polyLayout[1].SemanticName = "TEXCOORD";
	polyLayout[1].SemanticIndex = 0;
	polyLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polyLayout[1].InputSlot = 0;
	polyLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polyLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polyLayout[1].InstanceDataStepRate = 0;

	// get a count of the elements in the layout
	ntInt elementsNum = _countof(polyLayout);

	// create the vertex input layout
	res = g_renderer->Device()->CreateInputLayout(polyLayout, elementsNum, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(res))
	{
		return false;
	}

	SAFE_RELEASE(vertexShaderBuffer);
	SAFE_RELEASE(pixelShaderBuffer);

	// setup the desc of the dynamic matrix constant buffer that is in the vertex shader
	D3D11_BUFFER_DESC matrixBufferDesc;
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

	// create a texture sampler state desc;
	D3D11_SAMPLER_DESC samplerDesc;
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

	return true;
}

void NtTextureShader::ReleaseShader()
{
	SAFE_RELEASE(m_sampleState);
	SAFE_RELEASE(m_matrixBuffer);
	SAFE_RELEASE(m_layout);
	SAFE_RELEASE(m_pixelShader);
	SAFE_RELEASE(m_vertexShader);
}


bool NtTextureShader::SetShaderParameter(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj, ntUint handle)
{
	XMMATRIX w;
	XMMATRIX v;
	XMMATRIX p;

	// transpose the matrices to prepare them for the shader
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
	NtMatrixBuffer* dataPtr = nullptr;
	dataPtr = (NtMatrixBuffer*)mappedResource.pData;

	// copy the matrices into the constant buffer.
	dataPtr->world = w;
	dataPtr->view = v;
	dataPtr->projection = p;

	// unlock the constant buffer
	g_renderer->DeviceContext()->Unmap(m_matrixBuffer, 0);

	// set the position of the constant buffer in the vertex shader
	int bufferNum = 0;
	
	// now set the constant buffer in the vertex shader with the updated values.
	g_renderer->DeviceContext()->VSSetConstantBuffers(bufferNum, 1, &m_matrixBuffer);

	// find texture
	NtTexture* texture = g_resManager->AcquireTexture(handle);
	if (texture == nullptr)
	{
		return false;
	}

	// set shader texture resources in the pixel shader
	ID3D11ShaderResourceView* resourceView =  texture->GetTexture();
	g_renderer->DeviceContext()->PSSetShaderResources(0, 1, &resourceView);

	return true;
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
