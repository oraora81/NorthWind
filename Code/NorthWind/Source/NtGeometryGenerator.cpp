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
    for (ntUint i = 0; i < sliceCount; ++i)
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
