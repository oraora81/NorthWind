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

	// 상단에서 하단으로 쌓아 내려오면서 정점들의 상태를 계산한다.
	
	//
	gVertex topVertex(0.0f, radius, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	gVertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	meshData.Vertices.push_back(topVertex);

	ntFloat phiStep = NtMath<ntFloat>::PI / stackCount;
	ntFloat thetaStep = 2.0f * NtMath<ntFloat>::PI / slideCount;

	for (ntUint i = 1; i <= stackCount - 1; ++i)
	{
		ntFloat phi = i * phiStep;

		for (ntUint j = 0; j <= slideCount; ++j)
		{
			float theta = j * thetaStep;

			gVertex v;

			// spherical to cartesian
			v.Position.x = radius * NtMathf::Sin(phi) * NtMath<ntFloat>::Cos(theta);
			v.Position.y = radius * NtMathf::Cos(phi);
			v.Position.z = radius * NtMathf::Sin(phi) * NtMath<ntFloat>::Sin(theta);

			// Partial derivative of P with respect to theta
			v.TangentU.x = -radius * NtMath<ntFloat>::Sin(phi) * NtMath<ntFloat>::Sin(theta);
			v.TangentU.y = 0.0f;
			v.TangentU.z = radius * NtMath<ntFloat>::Sin(phi) * NtMath<ntFloat>::Cos(theta);

			XMVECTOR T = XMLoadFloat3(&v.TangentU);
			XMStoreFloat3(&v.TangentU, XMVector4Normalize(T));

			XMVECTOR p = XMLoadFloat3(&v.Position);
			XMStoreFloat3(&v.Normal, XMVector3Normalize(p));

			v.TexC.x = theta / XM_2PI;
			v.TexC.y = phi / XM_PI;

			meshData.Vertices.push_back(v);
		}
	}

	meshData.Vertices.push_back(bottomVertex);

	// compute indices for top stack. the top stack was written first fo the vertex buffer
	// and connects the top pole to the first ring
	for (ntUint i = 1; i <= slideCount; ++i)
	{
		meshData.Indices.push_back(0);
		meshData.Indices.push_back(i + 1);
		meshData.Indices.push_back(i);
	}

	// offset the indics to the index of the first vertex int the first ring.
	// This is just skipping the top pole vertex
	ntUint baseIndex = 1;
	ntUint ringVertexCount = slideCount + 1;
	for (ntUint i = 0; i < slideCount - 2; ++i)
	{
		for (ntUint j = 0; j < slideCount; j++)
		{
			meshData.Indices.push_back(baseIndex + i * ringVertexCount + j);
			meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	// Compute indices for bottom stack. the bottom stack was written last to the vertex buffer
	// and connect the bottom pole to the bottom ring.

	// south pole vertex was added last
	ntUint southPoleIndex = (ntUint)meshData.Vertices.size() - 1;

	// offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	// offset the indices to the index of the first vertex in the last ring.
	for (ntUint i = 0; i < slideCount; ++i)
	{
		meshData.Indices.push_back(southPoleIndex);
		meshData.Indices.push_back(baseIndex + i);
		meshData.Indices.push_back(baseIndex + i + 1);
	}

}

void NtGeometryGenerator::CreateGeosphere(ntFloat radius, ntUint numSubdivision, MeshData& meshData)
{
	// put a cap on the number of subdivisions.
    numSubdivision = NtMath<ntUint>::Min(numSubdivision, 5u);

	// approximate a sphere by tessellating an icosahedron

	const ntFloat X = 0.525731f;
	const ntFloat Z = 0.850651f;

	XMFLOAT3 pos[12] = 
	{
		XMFLOAT3(-X, 0.0f, Z),  XMFLOAT3(X, 0.0f, Z),
		XMFLOAT3(-X, 0.0f, -Z), XMFLOAT3(X, 0.0f, -Z),
		XMFLOAT3(0.0f, Z, X),   XMFLOAT3(0.0f, Z, -X),
		XMFLOAT3(0.0f, -Z, X),  XMFLOAT3(0.0f, -Z, -X),
		XMFLOAT3(Z, X, 0.0f),   XMFLOAT3(-Z, X, 0.0f),
		XMFLOAT3(Z, -X, 0.0f),  XMFLOAT3(-Z, -X, 0.0f)
	};

	DWORD k[60] =
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};

	meshData.Vertices.resize(12);
	meshData.Indices.resize(60);

	for (ntUint i = 0; i < 12; i++)
	{
		meshData.Vertices[i].Position = pos[i];
	}

	for (ntUint i = 0; i < 60; i++)
	{
		meshData.Indices[i] = k[i];
	}

	for (ntUint i = 0; i < numSubdivision; i++)
	{
		SubDivide(meshData);
	}

	// Project vertices onto sphere and scale.
	for (ntUint i = 0; i < meshData.Vertices.size(); ++i)
	{
		// Project onto unit sphere.
		XMVECTOR n = XMVector3Normalize(XMLoadFloat3(&meshData.Vertices[i].Position));

		// Project onto sphere
		// 주어진 반지름으로 비례시킨다.
		XMVECTOR p = radius * n;

		XMStoreFloat3(&meshData.Vertices[i].Position, p);
		XMStoreFloat3(&meshData.Vertices[i].Normal, n);

		// Derive texture coordinates from spherical coordinates.
		ntFloat theta = NtMathf::Angle(
			meshData.Vertices[i].Position.x,
			meshData.Vertices[i].Position.z);

		ntFloat phi = NtMathf::ACos(meshData.Vertices[i].Position.y / radius);

		meshData.Vertices[i].TexC.x = theta / XM_2PI;
		meshData.Vertices[i].TexC.y = phi / XM_PI;

		// Partial derivative of P with respect to theta
		// 세타에 대한 P의 편미분을 구한다.
		meshData.Vertices[i].TangentU.x = -radius * NtMathf::Sin(phi) * NtMathf::Sin(theta);
		meshData.Vertices[i].TangentU.y = 0.0f;
		meshData.Vertices[i].TangentU.z = +radius * NtMathf::Sin(phi) * NtMathf::Cos(theta);

		XMVECTOR T = XMLoadFloat3(&meshData.Vertices[i].TangentU);
		XMStoreFloat3(&meshData.Vertices[i].TangentU, XMVector3Normalize(T));
	}
}

void NtGeometryGenerator::SubDivide(MeshData& meshData)
{
	// Save a copy of the input geometry
	MeshData dup = meshData;

	meshData.Vertices.resize(0);
	meshData.Indices.resize(0);

	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	ntUint numTris = (ntUint)dup.Indices.size() / 3;
	for (ntUint i = 0; i < numTris; ++i)
	{
		gVertex v0 = dup.Vertices[dup.Indices[i * 3 + 0]];
		gVertex v1 = dup.Vertices[dup.Indices[i * 3 + 1]];
		gVertex v2 = dup.Vertices[dup.Indices[i * 3 + 2]];

		// Generate the midpoints
		gVertex m0, m1, m2;

		// For subdivision, we just care about the position component.
		// we derive the other vertex components in CreateGeoshpere

		m0.Position = XMFLOAT3(
			0.5f * (v0.Position.x + v1.Position.x),
			0.5f * (v0.Position.y + v1.Position.y),
			0.5f * (v0.Position.z + v1.Position.z));

		m1.Position = XMFLOAT3(
			0.5f * (v1.Position.x + v2.Position.x),
			0.5f * (v1.Position.y + v2.Position.y),
			0.5f * (v1.Position.z + v2.Position.z));

		m2.Position = XMFLOAT3(
			0.5f * (v0.Position.x + v2.Position.x),
			0.5f * (v0.Position.y + v2.Position.y),
			0.5f * (v0.Position.z + v2.Position.z));

		meshData.Vertices.push_back(v0);    // 0
		meshData.Vertices.push_back(v1);    // 1
		meshData.Vertices.push_back(v2);    // 2
		meshData.Vertices.push_back(m0);    // 3
		meshData.Vertices.push_back(m1);    // 4
		meshData.Vertices.push_back(m2);    // 5

		meshData.Indices.push_back(i * 6 + 0);
		meshData.Indices.push_back(i * 6 + 3);
		meshData.Indices.push_back(i * 6 + 5);

		meshData.Indices.push_back(i * 6 + 3);
		meshData.Indices.push_back(i * 6 + 4);
		meshData.Indices.push_back(i * 6 + 5);

		meshData.Indices.push_back(i * 6 + 5);
		meshData.Indices.push_back(i * 6 + 4);
		meshData.Indices.push_back(i * 6 + 2);

		meshData.Indices.push_back(i * 6 + 3);
		meshData.Indices.push_back(i * 6 + 1);
		meshData.Indices.push_back(i * 6 + 4);
	}
}


void NtGeometryGenerator::CreateCylinder(ntFloat bottomRadius, ntFloat topRadius, ntFloat height, ntUint sliceCount, ntUint stackCount, MeshData& meshData)
{
	meshData.Vertices.clear();
	meshData.Indices.clear();

	// create the Dummies
	float stackHeight = height / stackCount;

	// 한 단계 위의 더미로 올라갈 때마다 반지름 변화량을 구한다.
	float radiusStep = (topRadius - bottomRadius) / stackCount;

	ntUint ringCount = stackCount + 1;

	// 최하단 고리에서 최상단 고리까지 훑어가며 각 고리의 정점을 계산
	for (ntUint i = 0; i < ringCount; ++i)
	{
		ntFloat y = -0.5f * height + i * stackHeight;
		ntFloat r = bottomRadius + i * radiusStep;

		// 현재 고리의 정점들
		ntFloat dTheta = 2.0f * NtMath<ntFloat>::PI / sliceCount;
		for (ntUint j = 0; j <= sliceCount; ++j)
		{
			gVertex vertex;

			ntFloat c = NtMath<ntFloat>::Cos(j * dTheta);
			ntFloat s = NtMath<ntFloat>::Sin(j * dTheta);

			vertex.Position = XMFLOAT3(r * c, y, r * s);

			vertex.TexC.x = (ntFloat)j / sliceCount;
			vertex.TexC.y = 1.0f - (ntFloat)i / stackCount;

			// 원기둥을 다음과 같이 매개변수화 할 수 있다.
			// 매개변수 v가 새로 도입되었는데, 이 매개변수는 정점 좌표 v와 
			// 동일한 방향으로 나아간다. (종접선[bitangent]이 v 정점 좌표와 동일한 방향으로
			// 진행하도록 하기 위해
			//   y(v) = h - hv for v in [0,1].
			//   r(v) = r1 + (r0-r1)v
			//
			//   x(t, v) = r(v)*cos(t)
			//   y(t, v) = h - hv
			//   z(t, v) = r(v)*sin(t)
			// 
			//  dx/dt = -r(v)*sin(t)
			//  dy/dt = 0
			//  dz/dt = +r(v)*cos(t)
			//
			//  dx/dv = (r0-r1)*cos(t)
			//  dy/dv = -h
			//  dz/dv = (r0-r1)*sin(t)

			// TangentU는 단위 길이이다.
			vertex.TangentU = XMFLOAT3(-s, 0.0f, c);

			ntFloat dr = bottomRadius - topRadius;
			XMFLOAT3 bitangent(dr * c, -height, dr * s);

			XMVECTOR T = XMLoadFloat3(&vertex.TangentU);
			XMVECTOR B = XMLoadFloat3(&bitangent);
			XMVECTOR N = XMVector3Normalize(XMVector3Cross(T, B));
			XMStoreFloat3(&vertex.Normal, N);

			meshData.Vertices.push_back(vertex);
		}
	}

	// @@@ 각 고리의 첫 정점과 마지막 정점이 위치가 중복되지만 텍스쳐 좌표는
	// 중복되지 않음을 주의해야 한다. 원기둥에 텍스쳐를 제대로 입히려면 이렇게 해야 한다.
	ntUint ringVertexCount = sliceCount + 1;

	// 각 스택의 인덱스들을 계산한다.
	for (ntUint i = 0; i < stackCount; ++i)
	{
		for (ntUint j = 0; j < sliceCount; ++j)
		{
			meshData.Indices.push_back(i * ringVertexCount + j);
			meshData.Indices.push_back((i + 1) * ringVertexCount + j);
			meshData.Indices.push_back((i + 1) * ringVertexCount + j + 1);

			meshData.Indices.push_back(i * ringVertexCount + j);
			meshData.Indices.push_back((i + 1) * ringVertexCount + j + 1);
			meshData.Indices.push_back(i * ringVertexCount + j + 1);
		}
	}

	BuildCylinderTopCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
	BuildCylinderBottomCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
}

void NtGeometryGenerator::BuildCylinderTopCap(ntFloat bottomRaius, ntFloat topRadius, ntFloat height, ntUint sliceCount, ntUint stackCount, MeshData& meshData)
{
	ntUint baseIndex = (ntUint)meshData.Vertices.size();

	ntFloat y = 0.5f * height;
	ntFloat dTheta = 2.0f * NtMath<ntFloat>::PI / sliceCount;

	// 고리 정점들을 복제해서 마개 정점들을 만든다.
	// 텍스쳐 좌표와 법선이 다르므로 중복이 필요하다.
	for (ntUint i = 0; i <= sliceCount; ++i)
	{
		ntFloat x = topRadius * NtMath<ntFloat>::Cos(i * dTheta);
		ntFloat z = topRadius * NtMath<ntFloat>::Sin(i * dTheta);

		// 윗면 마개의 텍스쳐 좌표 면적이 밑면에 비례하도록
		// 텍스쳐 좌표를 높이에 따라 적절히 축소한다.
		ntFloat u = x / height + 0.5f;
		ntFloat v = z / height + 0.5f;

		meshData.Vertices.push_back(gVertex(x, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
	}

	// 마개의 정가운데 
	meshData.Vertices.push_back(gVertex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));

	// 정가운데 정점의 인덱스
	ntUint centerIndex = (ntUint)meshData.Vertices.size() - 1;
	for (ntUint i = 0; i < sliceCount; ++i)
	{
		meshData.Indices.push_back(centerIndex);
		meshData.Indices.push_back(baseIndex + i + 1);
		meshData.Indices.push_back(baseIndex + i);
	}
}

void NtGeometryGenerator::BuildCylinderBottomCap(ntFloat bottomRaius, ntFloat topRadius, ntFloat height, ntUint sliceCount, ntUint stackCount, MeshData& meshData)
{
	// 하단 마개 만들기
	ntUint baseIndex = (ntUint)meshData.Vertices.size();
	ntFloat y = -0.5f * height;

	// 고리 정점 만들기
	ntFloat dTheta = 2.0f * NtMath<ntFloat>::PI / sliceCount;
	for (ntUint i = 0; i <= sliceCount; ++i)
	{
		ntFloat x = bottomRaius * NtMath<ntFloat>::Cos(i * dTheta);
		ntFloat z = bottomRaius * NtMath<ntFloat>::Sin(i * dTheta);

		ntFloat u = x / height + 0.5f;
		ntFloat v = z / height + 0.5f;

		meshData.Vertices.push_back(gVertex(x, y, z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
	}

	// 가운데 정점
	meshData.Vertices.push_back(gVertex(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));

	//
	ntUint centerIndex = (ntUint)meshData.Vertices.size() - 1;
	for (ntUint i = 0; i < sliceCount; ++i)
	{
		meshData.Indices.push_back(centerIndex);
		meshData.Indices.push_back(baseIndex + i);
		meshData.Indices.push_back(baseIndex + i + 1);
	}
}

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
