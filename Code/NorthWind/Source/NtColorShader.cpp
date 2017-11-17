

#include "NtCoreLib.h"
#include "NtColorShader.h"

#include "NtD3DRenderer.h"

namespace nt { namespace renderer {


NtColorShader::NtColorShader()
	: NtShader()
	, m_matrixBuffer(nullptr)
{

}

NtColorShader::~NtColorShader()
{

}

bool NtColorShader::Initialize(const ntWchar* vs, const ntWchar* ps)
{
	NtAsserte(vs != nullptr);
	NtAsserte(ps != nullptr);

	// initialize the vertex and pixel shaders;
	bool res = InitializeShader(vs, ps);
	if (false == res)
	{
		return false;
	}

	return true;
}

bool NtColorShader::InitializeFx(const ntWchar* fx)
{
	NtAsserte(fx != nullptr);

	nt::fs::NtFileBuffer fileBuffer(fx);

	HRF(D3DX11CreateEffectFromMemory(fileBuffer.GetData(), fileBuffer.GetBytes(), 0, g_renderer->Device(), &m_fx, nullptr));

	m_tech = m_fx->GetTechniqueByName("ColorTech");

	m_fxWorldViewProj = m_fx->GetVariableByName("gWorldViewProj")->AsMatrix();

    //m_fxTime = m_fx->GetVariableByName("gTime")->AsScalar();

	// now setup the layout of the data that goes into the shader
	// This setup needs to match the NtPCVertex structure in the ModelClass and in the shader.h
	D3D11_INPUT_ELEMENT_DESC elementLayout[2];
	elementLayout[0].SemanticName = "POSITION";
	elementLayout[0].SemanticIndex = 0;
	elementLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elementLayout[0].InputSlot = 0;
	elementLayout[0].AlignedByteOffset = 0;
	elementLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elementLayout[0].InstanceDataStepRate = 0;

	elementLayout[1].SemanticName = "COLOR";
	elementLayout[1].SemanticIndex = 0;
	elementLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elementLayout[1].InputSlot = 0;
	//elementLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	elementLayout[1].AlignedByteOffset = 12;
	elementLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elementLayout[1].InstanceDataStepRate = 0;

	D3DX11_PASS_DESC passDesc;
	m_tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HRF(g_renderer->Device()->CreateInputLayout(elementLayout, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_layout));

	return true;
}

bool NtColorShader::InitializeFxTwoInputElem(const ntWchar* fx)
{
    NtAsserte(fx != nullptr);

    nt::fs::NtFileBuffer fileBuffer(fx);

    HRF(D3DX11CreateEffectFromMemory(fileBuffer.GetData(),
        fileBuffer.GetBytes(),
        0,
        g_renderer->Device(),
        &m_fx, nullptr));

    m_tech = m_fx->GetTechniqueByName("ColorTech");

    m_fxWorldViewProj = m_fx->GetVariableByName("gWorldViewProj")->AsMatrix();

    D3D11_INPUT_ELEMENT_DESC elems[] = 
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    D3DX11_PASS_DESC passDesc;
    m_tech->GetPassByIndex(0)->GetDesc(&passDesc);
    HRF(g_renderer->Device()->CreateInputLayout(elems, 2, passDesc.pIAInputSignature,
        passDesc.IAInputSignatureSize, &m_layout));

    return true;
}

bool NtColorShader::InitializeFx32bitColor(const ntWchar* fx)
{
    NtAsserte(fx != nullptr);

    nt::fs::NtFileBuffer fileBuffer(fx);

    HRF(D3DX11CreateEffectFromMemory(fileBuffer.GetData(),
        fileBuffer.GetBytes(),
        0,
        g_renderer->Device(),
        &m_fx, nullptr));

    m_tech = m_fx->GetTechniqueByName("ColorTech");

    m_fxWorldViewProj = m_fx->GetVariableByName("gWorldViewProj")->AsMatrix();

    D3D11_INPUT_ELEMENT_DESC elems[] = 
    {
        { "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    D3DX11_PASS_DESC passDesc;
    m_tech->GetPassByIndex(0)->GetDesc(&passDesc);
    HRF(g_renderer->Device()->CreateInputLayout(elems, 2, passDesc.pIAInputSignature,
        passDesc.IAInputSignatureSize, &m_layout));

    return true;
}

void NtColorShader::Release()
{
    NtShader::Release();

    SAFE_RELEASE(m_matrixBuffer);
}

bool NtColorShader::Render(ntInt indexCount, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix)
{
	// set the shader parameters that it will use for rendering.
	bool res = SetShaderParameters(worldMatrix, viewMatrix, projMatrix);
	if (false == res)
	{
		return false;
	}

	// now render the prepared buffers with the shader
	RenderShader(indexCount);

	return true;
}

bool NtColorShader::RenderFx(ntInt indexCount, const XMMATRIX& worldViewProj)
{
	g_renderer->DeviceContext()->IASetInputLayout(m_layout);
	
	m_fxWorldViewProj->SetMatrix(reinterpret_cast<const float*>(&worldViewProj));

    //m_fxTime->SetFloat(g_app->Timer().TotalTime());

	D3DX11_TECHNIQUE_DESC techDesc;
	m_tech->GetDesc(&techDesc);
	for (ntUint i = 0; i < techDesc.Passes; i++)
	{
		m_tech->GetPassByIndex(i)->Apply(0, g_renderer->DeviceContext());

		g_renderer->DeviceContext()->DrawIndexed(indexCount, 0, 0);
	}

	return true;
}

bool NtColorShader::InitializeShader(const ntWchar* vs, const ntWchar* ps)
{
	// Initialize the pointers this function will use to null
	ID3D10Blob* errMsg = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;

	ntUint shaderFlag = 0;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlag |= D3D10_SHADER_DEBUG;
	shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#else
	shaderFlag |= D3D10_SHADER_ENABLE_STRICTNESS;
#endif

	// compile the vertex shader code
	HRESULT res = D3DX11CompileFromFile(g_resManager->GetPath(vs), NULL, NULL, "ColorVertexShader", 
		g_renderer->GetVShaderModel(), 
		shaderFlag, 
		0, NULL,
		&vertexShaderBuffer, &errMsg, NULL);
	if (FAILED(res))
	{
		if (errMsg != nullptr)
		{
			g_renderer->OutputShaderErrorMessage(errMsg, vs);
		}
		else
		{
			MessageBox(nullptr, vs, L"Missing Shader File", MB_OK);
		}

		return false;
	}


	// compile the pixel shader code
	ID3D10Blob* pixelShaderBuffer = nullptr;
	res = D3DX11CompileFromFile(g_resManager->GetPath(ps), NULL, NULL, "ColorPixelShader", 
		g_renderer->GetPShaderModel(), 
		shaderFlag, 
		0, 
		NULL,
		&pixelShaderBuffer, &errMsg, NULL);
	if (FAILED(res))
	{
		if (errMsg != nullptr)
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


	// now setup the layout of the data that goes into the shader
	// This setup needs to match the NtPCVertex structure in the ModelClass and in the shader.h
	D3D11_INPUT_ELEMENT_DESC elementLayout[2];
	elementLayout[0].SemanticName = "POSITION";
	elementLayout[0].SemanticIndex = 0;
	elementLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elementLayout[0].InputSlot = 0;
	elementLayout[0].AlignedByteOffset = 0;
	elementLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elementLayout[0].InstanceDataStepRate = 0;

	elementLayout[1].SemanticName = "COLOR";
	elementLayout[1].SemanticIndex = 0;
	elementLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elementLayout[1].InputSlot = 0;
	elementLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	elementLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elementLayout[1].InstanceDataStepRate = 0;

	// get a count of the elements in the layout
	ntInt elementCount = _countof(elementLayout);

	// create the vertex input layout
	res = g_renderer->Device()->CreateInputLayout(elementLayout, elementCount, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(res))
	{
		return false;
	}

	// release the vertex shader buffer and pixel shader buffer since they are no longer needed
	SAFE_RELEASE(vertexShaderBuffer);
	SAFE_RELEASE(pixelShaderBuffer);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader
	D3D11_BUFFER_DESC matrixBufferDesc;
	Crt::MemSet(&matrixBufferDesc, sizeof(matrixBufferDesc));

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(NtMatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class
	res = g_renderer->Device()->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(res))
	{
		return false;
	}


	return true;
}

bool NtColorShader::SetShaderParameters(const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix)
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX proj;

	// Transpose the matrices to prepare them for the shader
	world = XMMatrixTranspose(worldMatrix);
	view = XMMatrixTranspose(viewMatrix);
	proj = XMMatrixTranspose(projMatrix);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	// lock the constant buffer so it can be written 
	HRESULT res = g_renderer->DeviceContext()->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(res))
	{
		return false;
	}

	// get a pointer to the data in the constant buffer
	NtMatrixBufferType* bufferData = (NtMatrixBufferType*)mappedResource.pData;

	// copy the matrices into the constant buffer
	bufferData->world = world;
	bufferData->view = view;
	bufferData->proj = proj;

	// unlock the constant buffer
	g_renderer->DeviceContext()->Unmap(m_matrixBuffer, 0);

	// set the position of the constant buffer in the vertex shader
	ntInt bufferNum = 0;

	// finally set the constant buffer in the vertex shader with the updated values.
	g_renderer->DeviceContext()->VSSetConstantBuffers(bufferNum, 1, &m_matrixBuffer);

	return true;
}


void NtColorShader::RenderShader(int indexCount) const
{
	// set the vertex input layout
	g_renderer->DeviceContext()->IASetInputLayout(m_layout);

	// set the vertex and pixel shaders that will be used to render this triangle
	g_renderer->DeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	g_renderer->DeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

	// render the triangle
	g_renderer->DeviceContext()->DrawIndexed(indexCount, 0, 0);
}

} }
