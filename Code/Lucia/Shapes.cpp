#include "stdafx.h"
#pragma hdrstop

#include "Shapes.h"
#include "NtMath.h"
#include "NtGeometryGenerator.h"

Shapes::Shapes()
    : m_theta(1.5f * nt::NtMath<float>::PI)
    , m_phi(0.25f * nt::NtMath<float>::PI)
    , m_radius(5.0f)
{
    
}

Shapes::~Shapes()
{

}

void Shapes::Update(float deltaTime)
{
    NtModel::Update(deltaTime);

    float x = m_radius * nt::NtMath<float>::Sin(m_phi) * nt::NtMath<float>::Cos(m_theta);
    float z = m_radius * nt::NtMath<float>::Sin(m_phi) * nt::NtMath<float>::Sin(m_theta);
    float y = m_radius * nt::NtMath<float>::Cos(m_phi);

    XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
    XMVECTOR target = XMVectorZero();
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMMATRIX v = XMMatrixLookAtLH(pos, target, up);

    g_renderer->SetViewMatrix(v);
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

    NtModel::NtPCVertex* vtxArray = &vertices[0];
    UINT* idxArray = &indices[0];

    InitializeModelData(vtxArray, vertices.size(), idxArray, indices.size(), L"../Code/Lucia/simple_fx.fxo");
}
