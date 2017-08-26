#include "stdafx.h"
#pragma hdrstop

#include "Box.h"
#include "NtColorShader.h"

using namespace nt;

namespace
{
    struct sVertex
    {
        XMFLOAT3 m_position;
    };

    struct sColor
    {
        XMFLOAT4 m_color;
    };
}

Box::Box()
: m_theta(1.5f * NtMath<float>::PI)
, m_phi(0.25f * NtMath<float>::PI)
, m_radius(5.0f)
{
    XMStoreFloat4x4(&m_world, XMMatrixIdentity());
}


Box::~Box()
{

}

void Box::MakeGeometry()
{
    MakeNormal();
}

void Box::MakeNormal()
{
    NtModel::NtPCVertex vertices[] =
    {
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), (const float*)&Colors::White },
        { XMFLOAT3(-1.0f, +1.0f, -1.0f), (const float*)&Colors::Black },
        { XMFLOAT3(+1.0f, +1.0f, -1.0f), (const float*)&Colors::Red },
        { XMFLOAT3(+1.0f, -1.0f, -1.0f), (const float*)&Colors::Green },
        { XMFLOAT3(-1.0f, -1.0f, +1.0f), (const float*)&Colors::Blue },
        { XMFLOAT3(-1.0f, +1.0f, +1.0f), (const float*)&Colors::Yellow },
        { XMFLOAT3(+1.0f, +1.0f, +1.0f), (const float*)&Colors::Cyan },
        { XMFLOAT3(+1.0f, -1.0f, +1.0f), (const float*)&Colors::Magenta },
    };

    ntUint indices[] =
    {
        0, 1, 2,
        0, 2, 3,

        4, 6, 5,
        4, 7, 6,

        4, 5, 1,
        4, 1, 0,

        3, 2, 6,
        3, 6, 7,

        1, 5, 6,
        1, 6, 2,

        4, 0, 3,
        4, 3, 7
    };

    InitializeModelData(vertices, _countof(vertices), indices, _countof(indices), L"../Code/Lucia/simple_fx.fxo");
}

void Box::MakeGeometryTwoVertexBuf()
{
    sVertex vertices[] =
    {
        XMFLOAT3(-1.0f, -1.0f, -1.0f),
        XMFLOAT3(-1.0f, +1.0f, -1.0f),
        XMFLOAT3(+1.0f, +1.0f, -1.0f),
        XMFLOAT3(+1.0f, -1.0f, -1.0f),
        XMFLOAT3(-1.0f, -1.0f, +1.0f),
        XMFLOAT3(-1.0f, +1.0f, +1.0f),
        XMFLOAT3(+1.0f, +1.0f, +1.0f),
        XMFLOAT3(+1.0f, -1.0f, +1.0f),
    };

    sColor colors[] =
    {
        (const float*)&Colors::White,
        (const float*)&Colors::Black,
        (const float*)&Colors::Red,
        (const float*)&Colors::Green,
        (const float*)&Colors::Blue,
        (const float*)&Colors::Yellow,
        (const float*)&Colors::Cyan,
        (const float*)&Colors::Magenta,
    };

    ntUint indices[] =
    {
        0, 1, 2,
        0, 2, 3,

        4, 6, 5,
        4, 7, 6,

        4, 5, 1,
        4, 1, 0,

        3, 2, 6,
        3, 6, 7,

        1, 5, 6,
        1, 6, 2,

        4, 0, 3,
        4, 3, 7
    };

    D3D11_BUFFER_DESC vd1;
    vd1.Usage = D3D11_USAGE_IMMUTABLE;
    vd1.ByteWidth = sizeof(sVertex) * _countof(vertices);
    vd1.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vd1.CPUAccessFlags = 0;
    vd1.MiscFlags = 0;
    vd1.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA vbData;
    vbData.pSysMem = vertices;

    ID3D11Buffer* vb;
    HR(g_renderer->Device()->CreateBuffer(&vd1, &vbData, &vb));

    m_vertexBuffer = vb;
    m_vertexCount = _countof(vertices);

    m_buffers[0] = m_vertexBuffer;

    D3D11_BUFFER_DESC vd2;
    vd2.Usage = D3D11_USAGE_IMMUTABLE;
    vd2.ByteWidth = sizeof(sColor) * _countof(colors);
    vd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vd2.CPUAccessFlags = 0;
    vd2.MiscFlags = 0;
    vd2.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA cData;
    cData.pSysMem = colors;

    ID3D11Buffer* cb;
    HR(g_renderer->Device()->CreateBuffer(&vd2, &cData, &cb));

    m_CB = cb;
    m_colorCount = _countof(colors);
    m_buffers[1] = cb;

    // index buffer
    D3D11_BUFFER_DESC id;
    id.Usage = D3D11_USAGE_IMMUTABLE;
    id.ByteWidth = sizeof(ntUint) * _countof(indices);
    id.BindFlags = D3D11_BIND_INDEX_BUFFER;
    id.CPUAccessFlags = 0;
    id.MiscFlags = 0;
    id.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA ibData;
    ibData.pSysMem = indices;

    ID3D11Buffer* ib = nullptr;
    HR(g_renderer->Device()->CreateBuffer(&id, &ibData, &ib));

    m_indexBuffer = ib;
    m_indexCount = _countof(indices);

    m_colorShader->InitializeFx(L"../Code/Lucia/simple_fx.fxo");
}

void Box::Update(float deltaTime)
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
}

void Box::RenderColor(XMMATRIX& worldViewProj)
{
    RenderNormal(worldViewProj);
}

void Box::RenderNormal(XMMATRIX& worldViewProj)
{
    NtModel::RenderColor(worldViewProj);
}

void Box::RenderTwoVertexBuf(XMMATRIX& worldViewProj)
{
    ntUint strides[] =
    {
        sizeof(sVertex),
        sizeof(sColor)
    };

    ntUint offsets[] = { 0, 0 };

    g_renderInterface->SetPrimitiveTopology(ePrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    g_renderInterface->SetVertexBuffers(0, 2, m_buffers, strides, offsets);

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
        XMMATRIX world = XMLoadFloat4x4(&m_world);
        effectMatrix->SetMatrix(reinterpret_cast<float*>(&(world * viewProj)));

        tech->GetPassByIndex(p)->Apply(0, g_renderer->DeviceContext());
        g_renderer->DeviceContext()->DrawIndexed(m_indexCount, 0, 0);
    }
}