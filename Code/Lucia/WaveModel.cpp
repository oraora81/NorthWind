#include "stdafx.h"
#pragma hdrstop

#include "WaveModel.h"
#include "NtMath.h"
#include "NtGeometryGenerator.h"
#include "NtColorShader.h"
#include "NtLightShader.h"
#include "NtShaderHandler.h"
#include "NtInputLayout.h"
#include "NtSampleShader.h"

using namespace nt;

namespace
{
	float GetHeight(float x, float z)
	{
		return 0.3f * (z * NtMathf::Sin(0.1f * x) * NtMathf::Cos(0.1f * z));
	}

    XMFLOAT3 GetHillNormal(float x, float z)
    {
        // n = (-df/dx, 1, -df/dz)
        XMFLOAT3 n(
            -0.03f* z * NtMathf::Cos(0.1f * x) - 0.3f * NtMathf::Cos(0.1f * z),
            1.0f,
            -0.3f* NtMathf::Sin(0.1f * x) + 0.03f * x * NtMathf::Sin(0.1f * z));

        XMVECTOR unitNormal = XMVector3Normalize(XMLoadFloat3(&n));
        XMStoreFloat3(&n, unitNormal);

        return n;
    }
}

WaveModel::WaveModel()
	: m_waveVB(nullptr)
	, m_waveIB(nullptr)
    , m_fxPointLight(nullptr)
    , m_fxSpotLight(nullptr)
    , m_gridIndexCount(0)
    , m_lightCount(1)
{
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&m_gridWorld, I);

	XMStoreFloat4x4(&m_wavesWorld, I);
    XMMATRIX wavOffset = XMMatrixTranslation(0.0f, -3.0f, 0.0f);
    XMStoreFloat4x4(&m_wavesWorld, wavOffset);

    m_dirLight.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    m_dirLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_dirLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_dirLight.Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

    // 점광원 - 위치는 애니메이션을 위해 매 프레임 Update에서 갱신된다.
    m_pointLight.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
    m_pointLight.Diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
    m_pointLight.Specular = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
    m_pointLight.Att = XMFLOAT3(0.0f, 0.1f, 0.0f);
    m_pointLight.Range = 25.0f;

    // 점적광원 설정 - 위치는 애니메이션을 위해 매 프레임 갱신
    m_spotLight.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    m_spotLight.Diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
    m_spotLight.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_spotLight.Att = XMFLOAT3(1.0f, 0.0f, 0.0f);
    m_spotLight.Spot = 96.0f;
    m_spotLight.Range = 10000.0f;
    
    m_landMaterial.Ambient = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
    m_landMaterial.Diffuse = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
    m_landMaterial.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);

    m_wavMaterial.Ambient = XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
    m_wavMaterial.Diffuse = XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
    m_wavMaterial.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 96.0f);
}

WaveModel::~WaveModel()
{
	SAFE_RELEASE(m_waveVB);
	SAFE_RELEASE(m_waveIB);
}

void WaveModel::Update(float deltaTime)
{
	NtModel::Update(deltaTime);

	static float timeBase = 0.0f;

	if (g_app->Timer().TotalTime() - timeBase >= 0.25f)
	{
		timeBase += 0.25f;

		DWORD i = 5 + rand() % (m_waves.RowCount() - 10);
		DWORD j = 5 + rand() % (m_waves.ColumnCount() - 10);

		float r = NtMathf::Rand(1.0f, 2.0f);

		m_waves.Disturb(i, j, r);
	}

	m_waves.Update(deltaTime);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(g_renderer->DeviceContext()->Map(m_waveVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));

	Vertex::NtLVertex* vertices = reinterpret_cast<Vertex::NtLVertex*>(mappedData.pData);
	for (UINT i = 0; i < m_waves.VertexCount(); i++)
	{
		vertices[i].position = m_waves[i];
		//vertices[i].color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
        vertices[i].normal = m_waves.Normal(i);
	}
	g_renderer->DeviceContext()->Unmap(m_waveVB, 0);

    // animate the lights
    // circle light over the land surface
    m_pointLight.Position.x = 70.0f * NtMathf::Cos(0.2f * g_app->Timer().TotalTime());
    m_pointLight.Position.z = 70.0f * NtMathf::Sin(0.2f * g_app->Timer().TotalTime());
    m_pointLight.Position.y = NtMathf::Max(GetHeight(m_pointLight.Position.x, m_pointLight.Position.z), -3.0f) + 10.0f;

    m_spotLight.Position = m_eyePosW;
    XMVECTOR pos = XMVectorSet(m_eyePosW.x, m_eyePosW.y, m_eyePosW.z, 1.0f);
    XMStoreFloat3(&m_spotLight.Direction, XMVector3Normalize(XMVectorZero() - pos));

    /*if (GetAsyncKeyState('0') & 0x8000)
        m_lightCount = 0;

    if (GetAsyncKeyState('1') & 0x8000)
        m_lightCount = 1;

    if (GetAsyncKeyState('2') & 0x8000)
        m_lightCount = 2;

    if (GetAsyncKeyState('3') & 0x8000)
        m_lightCount = 3;*/
}

void WaveModel::Render(XMMATRIX& worldViewProj)
{
	ntUint stride = sizeof(Vertex::NtLVertex);
	ntUint offset = 0;

	g_renderInterface->SetPrimitiveTopology(PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    g_renderer->DeviceContext()->IASetInputLayout(NtInputLayoutHandler::PNInputLayout);

	XMMATRIX view;
	XMMATRIX proj;
	g_renderer->GetViewMatrix(view);
	g_renderer->GetProjectionMatrix(proj);

	XMMATRIX viewProj = view * proj;

    auto& LightShader = NtShaderHandler::SampleLightShader;

    LightShader->SetDirLight(&m_dirLight);
    LightShader->SetPointLight(&m_pointLight);
    LightShader->SetSpotLight(&m_spotLight);
    LightShader->SetEyePosW(m_eyePosW);

    D3DX11_TECHNIQUE_DESC techDesc;
    ID3DX11EffectTechnique* tech = LightShader->LightTech();
    /*switch (m_lightCount)
    {
    case 1:
        tech = LightShader->Light1Tech();
        break;
    case 2:
        tech = LightShader->Light2Tech();
        break;
    case 3:
        tech = LightShader->Light3Tech();
        break;
    }*/
    

	tech->GetDesc(&techDesc);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		g_renderInterface->SetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
		g_renderInterface->SetIndexBuffers(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

        // set per object constant.
        // draw the grid
        XMMATRIX world = XMLoadFloat4x4(&m_gridWorld);
        XMMATRIX worldInvTranspose = NtD3dUtil::InverseTranspose(world);
        XMMATRIX goWorldViewProj = world * viewProj;

        LightShader->SetWorld(world);
        LightShader->SetWorldInvTranspose(worldInvTranspose);
        LightShader->SetWorldViewProj(goWorldViewProj);
        LightShader->SetMaterial(m_landMaterial);

		tech->GetPassByIndex(p)->Apply(0, g_renderer->DeviceContext());
		g_renderer->DeviceContext()->DrawIndexed(m_gridIndexCount, 0, 0);

		// draw the waves
		g_renderInterface->SetVertexBuffers(0, 1, &m_waveVB, &stride, &offset);
		g_renderInterface->SetIndexBuffers(m_waveIB, DXGI_FORMAT_R32_UINT, 0);

		world = XMLoadFloat4x4(&m_wavesWorld);
        worldInvTranspose = NtD3dUtil::InverseTranspose(world);
        goWorldViewProj = world * viewProj;

        LightShader->SetWorld(world);
        LightShader->SetWorldInvTranspose(worldInvTranspose);
        LightShader->SetWorldViewProj(goWorldViewProj);
        LightShader->SetMaterial(m_wavMaterial);

		tech->GetPassByIndex(p)->Apply(0, g_renderer->DeviceContext());
		g_renderer->DeviceContext()->DrawIndexed(3 * m_waves.TrisCount(), 0, 0);
	}
}

void WaveModel::MakeGeometry()
{
	// land
	renderer::NtGeometryGenerator::MeshData grid;
	renderer::NtGeometryGenerator gen;

	gen.CreateGrid(160.0f, 160.0f, 50, 50, grid);

	std::vector<Vertex::NtLVertex> vertices(grid.Vertices.size());
	for (size_t i = 0; i < grid.Vertices.size(); i++)
	{
		XMFLOAT3 p = grid.Vertices[i].Position;

		p.y = GetHeight(p.x, p.z);

		vertices[i].position = p;

		// Color the vertex based on its height.
		//if (p.y < -10.0f)
		//{
		//	// Sandy beach color.
		//	vertices[i].color = XMFLOAT4(1.0f, 0.96f, 0.62f, 1.0f);
		//}
		//else if (p.y < 5.0f)
		//{
		//	// Light yellow-green.
		//	vertices[i].color = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
		//}
		//else if (p.y < 12.0f)
		//{
		//	// Dark yellow-green.
		//	vertices[i].color = XMFLOAT4(0.1f, 0.48f, 0.19f, 1.0f);
		//}
		//else if (p.y < 20.0f)
		//{
		//	// Dark brown.
		//	vertices[i].color = XMFLOAT4(0.45f, 0.39f, 0.34f, 1.0f);
		//}
		//else
		//{
		//	// White snow.
		//	vertices[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		//}

        vertices[i].normal = GetHillNormal(p.x, p.z);
	}

	std::vector<UINT> indices;
	indices.insert(indices.end(), grid.Indices.begin(), grid.Indices.end());
    m_gridIndexCount = (ntInt)grid.Indices.size();

    Vertex::NtLVertex* vtxArray = &vertices[0];
	UINT* idxArray = &indices[0];

	InitializeModelData(vtxArray, sizeof(Vertex::NtLVertex), (ntInt)vertices.size(), idxArray, (ntInt)indices.size());

	MakeWave();
}

void WaveModel::MakeWave()
{
	m_waves.Init(160, 160, 1.0f, 0.03f, 3.25f, 0.4f);

	m_waveVB = MakeVertexBuffer(nullptr, sizeof(Vertex::NtLVertex), m_waves.VertexCount(), BufferUsage::USAGE_DYNAMIC, eCpuAccessFlag::CPU_ACCESS_WRITE);

	std::vector<UINT> indices(3 * m_waves.TrisCount());

	// iterate over each quad
	UINT m = m_waves.RowCount();
	UINT n = m_waves.ColumnCount();

	int k = 0;
	for (UINT i = 0; i < m - 1; i++)
	{
		for (DWORD j = 0; j < n - 1; j++)
		{
			indices[k] = i * n + j;
			indices[k+1] = i * n + j + 1;
			indices[k+2] = (i + 1) * n + j;

			indices[k + 3] = (i + 1) * n + j;
			indices[k + 4] = i * n + j + 1;
			indices[k + 5] = (i + 1) * n + j + 1;

			// next quad
			k += 6;
		}
	}

	UINT* idxArray = &indices[0];
	m_waveIB = MakeIndexBuffer(idxArray, (ntInt)indices.size());
}

