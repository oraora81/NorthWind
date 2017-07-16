#include "NtCoreLib.h"
#pragma hdrstop

#include "NtGeometryGenerator.h"

namespace nt {	namespace renderer {

void NtGeometryGenerator::CreateBox(ntFloat width, ntFloat height, ntFloat depth, MeshData& meshData)
{
	// create the vertices

	gVertex v[24];

	ntFloat w2 = 0.5f * width;
	ntFloat h2 = 0.5f * height;
	ntFloat d2 = 0.5f * depth;
	
	// Fill in the front face vertex data.
	v[0] = gVertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[1] = gVertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = gVertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[3] = gVertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	v[4] = gVertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[5] = gVertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[6] = gVertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[7] = gVertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the top face vertex data.
	v[8] = gVertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[9] = gVertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[10] = gVertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[11] = gVertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	v[12] = gVertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[13] = gVertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[14] = gVertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[15] = gVertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	v[16] = gVertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[17] = gVertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[18] = gVertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[19] = gVertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	v[20] = gVertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[21] = gVertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[22] = gVertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[23] = gVertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	meshData.Vertices.assign(&v[0], &v[24]);

	// create the indices

	ntUint i[36];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	meshData.Indices.assign(&i[0], &i[36]);
}

void NtGeometryGenerator::CreateSphere(ntFloat radius, ntUint slideCount, ntUint stackCount, MeshData& meshData)
{
	meshData.Vertices.clear();
	meshData.Indices.clear();
}

//void CreateGeosphere(ntFloat radius, ntUint numSubdivision, MeshData& meshData);

//void CreateCylinder(ntFloat bottomRadius, ntFloat topRadius, ntFloat height, ntUint sliceCount, ntUint stackCount, MeshData& meshData);

void NtGeometryGenerator::CreateGrid(ntFloat width, ntFloat depth, ntUint m, ntUint n, MeshData& meshData)
{
	ntUint vertexCount = m * n;
	ntUint faceCount = (m - 1) * (n - 1) * 2;

	// create the vertices
	float halfWidth = 0.5f * width;
	float halfDepth = 0.5f * depth;

	ntFloat dx = width / (n - 1);
	ntFloat dz = depth / (m - 1);

	ntFloat du = 1.0f / (n - 1);
	ntFloat dv = 1.0f / (m - 1);

	meshData.Vertices.resize(vertexCount);
	for (ntUint i = 0; i < m; ++i)
	{
		ntFloat z = halfDepth - (i * dz);
		for (ntUint j = 0; j < n; ++j)
		{
			ntFloat x = -halfWidth + (j * dx);

			gVertex& v = meshData.Vertices[i * n + j];
			v.Position = XMFLOAT3(x, 0.0f, z);
			v.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			v.TangentU = XMFLOAT3(1.0f, 0.0f, 0.0f);

			// stretch texture over grid
			v.TexC.x = j * du;
			v.TexC.y = i * dv;
		}
	}

	// create the indices
	meshData.Indices.resize(faceCount * 3);

	ntUint k = 0;
	for (ntUint i = 0; i < m - 1; ++i)
	{
		for (ntUint j = 0; j < n - 1; ++j)
		{
			meshData.Indices[k] = i * n + j;
			meshData.Indices[k + 1] = i * n + j + 1;
			meshData.Indices[k + 2] = (i + 1) * n + j;

			meshData.Indices[k + 3] = (i + 1) * n + j;
			meshData.Indices[k + 4] = i * n + j + 1;
			meshData.Indices[k + 5] = (i + 1) * n + j + 1;

			k += 6;
		}
	}
}

	//void CreateFullScreenQuad(MeshData& meshData);


}}
