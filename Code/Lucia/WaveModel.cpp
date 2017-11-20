#include "stdafx.h"
#pragma hdrstop

#include "WaveModel.h"
#include "NtMath.h"
#include "NtGeometryGenerator.h"
#include "NtColorShader.h"

using namespace nt;

namespace
{
	float GetHeight(float x, float z)
	{
		return 0.3f * (z * NtMath<float>::Sin(0.1f * x) * NtMath<float>::Cos(0.1f * z));
	}

    XMFLOAT3 GetHillNormal(float x, float z)
    {
        // n = (-df/dx, 1, -df/dz)
        XMFLOAT3 n(
            -0.03f* z * NtMath<float>::Cos(0.1f * x) - 0.3f * NtMath<float>::Cos(0.1f * z),
            1.0f,
            -0.3f* NtMath<float>::Sin(0.1f * x) + 0.03f * x * NtMath<float>::Sin(0.1f * z));

        XMVECTOR unitNormal = XMVector3Normalize(XMLoadFloat3(&n));
        XMStoreFloat3(&n, unitNormal);

        return n;
    }
}

WaveModel::WaveModel()
	: m_theta(1.5f * NtMath<float>::PI)
	, m_phi(0.25f * NtMath<float>::PI)
	, m_radius(5.0f)
	, m_waveVB(nullptr)
	, m_waveIB(nullptr)
{
    m_eyePosW = XMFLOAT3(0.0f, 0.0f, 0.0f);
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

	float x = m_radius * NtMath<float>::Sin(m_phi) * NtMath<float>::Cos(m_theta);
	float z = m_radius * NtMath<float>::Sin(m_phi) * NtMath<float>::Sin(m_theta);
	float y = m_radius * NtMath<float>::Cos(m_phi);

    m_eyePosW = XMFLOAT3(x, y, z);

	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX v = XMMatrixLookAtLH(pos, target, up);

	g_renderer->SetViewMatrix(v);

	static float timeBase = 0.0f;

	if (g_app->Timer().TotalTime() - timeBase >= 0.25f)
	{
		timeBase += 0.25f;

		DWORD i = 5 + rand() % 190;
		DWORD j = 5 + rand() % 190;

		float r = NtMathf::Rand(1.0f, 2.0f);

		m_waves.Disturb(i, j, r);
	}

	m_waves.Update(deltaTime);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(g_renderer->DeviceContext()->Map(m_waveVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));

	NtLVertex* vertices = reinterpret_cast<NtLVertex*>(mappedData.pData);
	for (UINT i = 0; i < m_waves.VertexCount(); i++)
	{
		vertices[i].position = m_waves[i];
		//vertices[i].color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
        vertices[i].normal = m_waves
	}
	g_renderer->DeviceContext()->Unmap(m_waveVB, 0);
}

void WaveModel::RenderColor(XMMATRIX& worldViewProj)
{
	ntUint stride = sizeof(NtLVertex);
	ntUint offset = 0;

	g_renderInterface->SetPrimitiveTopology(ePrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_renderer->DeviceContext()->IASetInputLayout(m_colorShader->GetInputLayout());

	XMMATRIX view;
	XMMATRIX proj;
	g_renderer->GetViewMatrix(view);
	g_renderer->GetProjectionMatrix(proj);

	XMMATRIX viewProj = view * proj;

	D3DX11_TECHNIQUE_DESC techDesc;

	ID3DX11EffectTechnique* tech = const_cast<ID3DX11EffectTechnique*>(m_colorShader->GetEffectTechnique());
	ID3DX11EffectMatrixVariable* effectMatrix = const_cast<ID3DX11EffectMatrixVariable*>(m_colorShader->GetEffectMatrix());

	tech->GetDesc(&techDesc);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		g_renderInterface->SetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

		g_renderInterface->SetIndexBuffers(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// draw the grid
		XMMATRIX world = XMLoadFloat4x4(&m_gridWorld);
		effectMatrix->SetMatrix(reinterpret_cast<float*>(&(world * viewProj)));
		tech->GetPassByIndex(p)->Apply(0, g_renderer->DeviceContext());
		g_renderer->DeviceContext()->DrawIndexed(m_gridIndexCount, 0, 0);

		// draw the waves
		g_renderInterface->SetVertexBuffers(0, 1, &m_waveVB, &stride, &offset);

		g_renderInterface->SetIndexBuffers(m_waveIB, DXGI_FORMAT_R32_UINT, 0);

		world = XMLoadFloat4x4(&m_wavesWorld);
		effectMatrix->SetMatrix(reinterpret_cast<float*>(&(world * viewProj)));
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

	std::vector<NtModel::NtLVertex> vertices(grid.Vertices.size());
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

	NtModel::NtLVertex* vtxArray = &vertices[0];
	UINT* idxArray = &indices[0];

	InitializeModelData(vtxArray, sizeof(NtLVertex), vertices.size(), idxArray, indices.size(), L"../Code/Lucia/simple_fx.fxo");

	MakeWave();
}

void WaveModel::MakeWave()
{
	m_waves.Init(160, 160, 1.0f, 0.03f, 3.25f, 0.4f);

	m_waveVB = MakeVertexBuffer(nullptr, sizeof(NtLVertex), m_waves.VertexCount(), eBufferUsage::USAGE_DYNAMIC, eCpuAccessFlag::CPU_ACCESS_WRITE);

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
	m_waveIB = MakeIndexBuffer(idxArray, indices.size());
}

