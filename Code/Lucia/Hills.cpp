#include "stdafx.h"
#pragma hdrstop

#include "Hills.h"
#include "NtGeometryGenerator.h"

using namespace nt;

namespace 
{
	ntFloat GetHeight(ntFloat x, ntFloat z)
	{
		return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
	}
}

Hills::Hills()
{

}

Hills::~Hills()
{

}

void Hills::MakeGeometry()
{
	renderer::NtGeometryGenerator::MeshData grid;
	renderer::NtGeometryGenerator generator;

	generator.CreateGrid(160.0f, 160.0f, 50, 50, grid);

	m_indexCount = grid.Indices.size();

    Vertex::NtPCVertex* vertices = new Vertex::NtPCVertex[grid.Vertices.size()];

	for (int i = 0; i < (int)grid.Vertices.size(); ++i)
	{
		const auto& v = grid.Vertices[i];

		XMFLOAT3 p = v.Position;

		p.y = GetHeight(p.x, p.z);
		
		vertices[i].position = p;

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

	ntUint* indices = new ntUint[grid.Indices.size()];
	for (int i = 0; i < (int)grid.Indices.size(); ++i)
	{
		indices[i] = grid.Indices[i];
	}
	
	InitializeModelData(vertices, sizeof(Vertex::NtPCVertex), grid.Vertices.size(), indices, grid.Indices.size());
}
