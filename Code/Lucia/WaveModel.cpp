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
}

WaveModel::WaveModel()
	: m_theta(1.5f * NtMath<float>::PI)
	, m_phi(0.25f * NtMath<float>::PI)
	, m_radius(5.0f)
	, m_waveVB(nullptr)
	, m_waveIB(nullptr)
{
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&m_gridWorld, I);
	XMStoreFloat4x4(&m_wavesWorld, I);
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

	NtPCVertex* vertices = reinterpret_cast<NtPCVertex*>(mappedData.pData);
	for (UINT i = 0; i < m_waves.VertexCount(); i++)
	{
		vertices[i].position = m_waves[i];
		vertices[i].color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	g_renderer->DeviceContext()->Unmap(m_waveVB, 0);
}

void WaveModel::RenderColor(XMMATRIX& worldViewProj)
{
	ntUint stride = sizeof(NtPCVertex);
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

	std::vector<NtModel::NtPCVertex> vertices(grid.Vertices.size());
	for (size_t i = 0; i < grid.Vertices.size(); i++)
	{
		XMFLOAT3 p = grid.Vertices[i].Position;

		p.y = GetHeight(p.x, p.z);

		vertices[i].position = p;

		// Color the vertex based on its height.
		if (p.y < -10.0f)
		{
			// Sandy beach color.
			vertices[i].color = XMFLOAT4(1.0f, 0.96f, 0.62f, 1.0f);
		}
		else if (p.y < 5.0f)
		{
			// Light yellow-green.
			vertices[i].color = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
		}
		else if (p.y < 12.0f)
		{
			// Dark yellow-green.
			vertices[i].color = XMFLOAT4(0.1f, 0.48f, 0.19f, 1.0f);
		}
		else if (p.y < 20.0f)
		{
			// Dark brown.
			vertices[i].color = XMFLOAT4(0.45f, 0.39f, 0.34f, 1.0f);
		}
		else
		{
			// White snow.
			vertices[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	std::vector<UINT> indices;
	indices.insert(indices.end(), grid.Indices.begin(), grid.Indices.end());

	NtModel::NtPCVertex* vtxArray = &vertices[0];
	UINT* idxArray = &indices[0];

	InitializeModelData(vtxArray, vertices.size(), idxArray, indices.size(), L"../Code/Lucia/simple_fx.fxo");

	MakeWave();
}

void WaveModel::MakeWave()
{
	m_waves.Init(200, 200, 0.8f, 0.03f, 3.25f, 0.4f);

	m_waveVB = MakeVertexBuffer(nullptr, m_waves.VertexCount(), eBufferUsage::USAGE_DYNAMIC, eCpuAccessFlag::CPU_ACCESS_WRITE);

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
