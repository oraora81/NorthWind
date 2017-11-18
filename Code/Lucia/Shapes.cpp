#include "stdafx.h"
#pragma hdrstop

#include "Shapes.h"
#include "NtMath.h"
#include "NtGeometryGenerator.h"
#include "NtColorShader.h"

Shapes::Shapes()
	: BaseShape()
{
	const XMMATRIX I = XMMatrixIdentity();

	XMStoreFloat4x4(&m_gridWorld, I);
	XMStoreFloat4x4(&m_boxWorld, I);
	XMStoreFloat4x4(&m_view, I);
	XMStoreFloat4x4(&m_proj, I);

	XMMATRIX boxScale = XMMatrixScaling(2.0f, 1.0f, 2.0f);
	XMMATRIX boxOffset = XMMatrixTranslation(0.0f, 0.5f, 0.0f);
	XMStoreFloat4x4(&m_boxWorld, XMMatrixMultiply(boxScale, boxOffset));

	XMMATRIX centerSphereScale = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	XMMATRIX centerSphereOffset = XMMatrixTranslation(0.0f, 2.0f, 0.0f);
	XMStoreFloat4x4(&m_centerSphere, XMMatrixMultiply(centerSphereScale, centerSphereOffset));

	for (int i = 0; i < 5; i++)
	{
		XMStoreFloat4x4(&m_cylWorld[i * 2 + 0], XMMatrixTranslation(-5.0f, 1.5f, -10.0f + i * 5.0f));
		XMStoreFloat4x4(&m_cylWorld[i * 2 + 1], XMMatrixTranslation(+5.0f, 1.5f, -10.0f + i * 5.0f));

		XMStoreFloat4x4(&m_sphereWorld[i * 2 + 0], XMMatrixTranslation(-5.0f, 3.5f, -10.0f + i * 5.0f));
		XMStoreFloat4x4(&m_sphereWorld[i * 2 + 1], XMMatrixTranslation(+5.0f, 3.5f, -10.0f + i * 5.0f));
	}
}

Shapes::~Shapes()
{

}

void Shapes::RenderColor(XMMATRIX& worldViewProj)
{
	ntUint stride = sizeof(NtPCVertex);
	ntUint offset = 0;

	g_renderInterface->SetPrimitiveTopology(ePrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_renderInterface->SetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	g_renderInterface->SetIndexBuffers(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

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
		//// draw the grid
		XMMATRIX world = XMLoadFloat4x4(&m_gridWorld);
		effectMatrix->SetMatrix(reinterpret_cast<float*>(&(world * viewProj)));
		tech->GetPassByIndex(p)->Apply(0, g_renderer->DeviceContext());
		g_renderer->DeviceContext()->DrawIndexed(m_gridIndexCount, m_gridIndexOffset, m_gridVertexOffset);

		// draw the box
		world = XMLoadFloat4x4(&m_boxWorld);
		effectMatrix->SetMatrix(reinterpret_cast<float*>(&(world * viewProj)));
		tech->GetPassByIndex(p)->Apply(0, g_renderer->DeviceContext());
		g_renderer->DeviceContext()->DrawIndexed(m_boxIndexCount, m_boxIndexOffset, m_boxVertexOffset);

		// draw center sphere
		world = XMLoadFloat4x4(&m_centerSphere);
		effectMatrix->SetMatrix(reinterpret_cast<float*>(&(world * viewProj)));
		tech->GetPassByIndex(p)->Apply(0, g_renderer->DeviceContext());
		g_renderer->DeviceContext()->DrawIndexed(m_sphereIndexCount, m_sphereIndexOffset, m_sphereVertexOffset);

		// draw the cylinder
		for (int i = 0; i < 10; i++)
		{
			world = XMLoadFloat4x4(&m_cylWorld[i]);
			effectMatrix->SetMatrix(reinterpret_cast<float*>(&(world * viewProj)));
			tech->GetPassByIndex(p)->Apply(0, g_renderer->DeviceContext());
			g_renderer->DeviceContext()->DrawIndexed(m_cylinderIndexCount, m_cylinderIndexOffset, m_cylinderVertexOffset);
		}

		// draw the sphere
		for (int i = 0; i < 10; i++)
		{
			world = XMLoadFloat4x4(&m_sphereWorld[i]);
			effectMatrix->SetMatrix(reinterpret_cast<float*>(&(world * viewProj)));
			tech->GetPassByIndex(p)->Apply(0, g_renderer->DeviceContext());
			g_renderer->DeviceContext()->DrawIndexed(m_sphereIndexCount, m_sphereIndexOffset, m_sphereVertexOffset);
		}
	}
}

void Shapes::MakeGeometry()
{
	nt::renderer::NtGeometryGenerator::MeshData box;
	nt::renderer::NtGeometryGenerator::MeshData grid;
	nt::renderer::NtGeometryGenerator::MeshData sphere;
	nt::renderer::NtGeometryGenerator::MeshData cylinder;

	nt::renderer::NtGeometryGenerator gen;

	gen.CreateBox(1.0f, 1.0f, 1.0f, box);
	gen.CreateGrid(20.0f, 30.0f, 60, 40, grid);
	gen.CreateSphere(0.5f, 20, 20, sphere);
	gen.CreateCylinder(0.5f, 0.3f, 3.0f, 20, 20, cylinder);
	//gen.CreateGeosphere(0.5f, 2, sphere);

	// cache the vertex offsets to each object in the concatenated vertex buffer
	m_boxVertexOffset = 0;
	m_gridVertexOffset = box.Vertices.size();
	m_sphereVertexOffset = m_gridVertexOffset + grid.Vertices.size();
	m_cylinderVertexOffset = m_sphereVertexOffset + sphere.Vertices.size();

	// cache the index count of each object
	m_boxIndexCount = box.Indices.size();
	m_gridIndexCount = grid.Indices.size();
	m_sphereIndexCount = sphere.Indices.size();
	m_cylinderIndexCount = cylinder.Indices.size();

	// cache the index count of each object in the concatenated index buffer
	m_boxIndexOffset = 0;
	m_gridIndexOffset = m_boxIndexCount;
	m_sphereIndexOffset = m_gridIndexOffset + m_gridIndexCount;
	m_cylinderIndexOffset = m_sphereIndexOffset + m_sphereIndexCount;

	UINT totalVertexCount =
		box.Vertices.size() +
		grid.Vertices.size() +
		sphere.Vertices.size() +
		cylinder.Vertices.size();

	UINT totalIndexCount =
		m_boxIndexCount +
		m_gridIndexCount +
		m_sphereIndexCount +
		m_cylinderIndexCount;

	// extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer

	std::vector<NtModel::NtPCVertex> vertices(totalVertexCount);

	XMFLOAT4 black(0.0f, 0.0f, 0.0f, 1.0f);

	UINT k = 0;
	for (size_t i = 0; i < box.Vertices.size(); i++, ++k)
	{
		vertices[k].position = box.Vertices[i].Position;
		vertices[k].color = black;
	}

	for (size_t i = 0; i < grid.Vertices.size(); i++, ++k)
	{
		vertices[k].position = grid.Vertices[i].Position;
		vertices[k].color = black;
	}

	for (size_t i = 0; i < sphere.Vertices.size(); i++, ++k)
	{
		vertices[k].position = sphere.Vertices[i].Position;
		vertices[k].color = black;
	}

	for (size_t i = 0; i < cylinder.Vertices.size(); i++, ++k)
	{
		vertices[k].position = cylinder.Vertices[i].Position;
		vertices[k].color = black;
	}


	// Pack the indices of all the meshes into one index buffer
	std::vector<UINT> indices;
	indices.insert(indices.end(), box.Indices.begin(), box.Indices.end());
	indices.insert(indices.end(), grid.Indices.begin(), grid.Indices.end());
	indices.insert(indices.end(), sphere.Indices.begin(), sphere.Indices.end());
	indices.insert(indices.end(), cylinder.Indices.begin(), cylinder.Indices.end());

	NtPCVertex* vtxArray = &vertices[0];
	UINT* idxArray = &indices[0];

	InitializeModelData(vtxArray, sizeof(NtPCVertex), vertices.size(), idxArray, indices.size(), L"../Code/Lucia/simple_fx.fxo");
}
