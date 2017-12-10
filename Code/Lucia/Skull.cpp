#include "stdafx.h"
#pragma hdrstop

#include "Skull.h"
#include "NtMath.h"

using namespace nt;

Skull::Skull()
    : m_theta(1.5f * nt::NtMath<float>::PI)
    , m_phi(0.25f * nt::NtMath<float>::PI)
    , m_radius(5.0f)
{

}

Skull::~Skull()
{

}

void Skull::Update(float deltaTime)
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

void Skull::MakeGeometry()
{
	const ntWchar* fileName = L"skull.txt";

	const ntWchar* path = g_resManager->GetPath(fileName);

    std::ifstream fin(path);

    if (!fin)
    {
        //MessageBox(0, L"skull.txt not found.", 0, 0);
        return;
    }

    UINT vcount = 0;
    UINT tcount = 0;
    std::string ignore;

    fin >> ignore >> vcount;
    fin >> ignore >> tcount;
    fin >> ignore >> ignore >> ignore >> ignore;

    float nx, ny, nz;
    XMFLOAT4 black(0.0f, 0.0f, 0.0f, 1.0f);

    std::vector<Vertex::NtPCVertex> vertices(vcount);
    for (UINT i = 0; i < vcount; ++i)
    {
        fin >> vertices[i].position.x >> vertices[i].position.y >> vertices[i].position.z;

        vertices[i].color = black;

        // Normal not used in this demo.
        fin >> nx >> ny >> nz;
    }

    fin >> ignore;
    fin >> ignore;
    fin >> ignore;

    m_skullIndexCount = 3 * tcount;
    std::vector<UINT> indices(m_skullIndexCount);
    for (UINT i = 0; i < tcount; ++i)
    {
        fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
    }

    fin.close();

    Vertex::NtPCVertex* vtxArray = &vertices[0];
	UINT* idxArray = &indices[0];

	InitializeModelData(vtxArray, sizeof(Vertex::NtPCVertex), vertices.size(), idxArray, m_skullIndexCount);
}
