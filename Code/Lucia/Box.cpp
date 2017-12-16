#include "stdafx.h"
#pragma hdrstop

#include "Box.h"
#include "NtColorShader.h"
#include "NtShaderHandler.h"
#include "NtInputLayout.h"

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

    struct sVertexColor
    {
        XMFLOAT3 m_pos;
        XMCOLOR m_color;
    };
}

Box::Box()
{
    XMMATRIX temp = XMMatrixIdentity();
    temp = XMMatrixTranslation(5.0f, 0.0f, 0.0f);
    XMStoreFloat4x4(&m_boxWorld, temp);

    temp = XMMatrixIdentity();
    temp = XMMatrixTranslation(-5.0f, 0.0f, 0.0f);
    XMStoreFloat4x4(&m_pyramidWorld, temp);

    m_boxVertexOffset = 0;
    m_boxIndexOffset = 0;
    m_pyramidVertexOffset = 0;
    m_pyramidIndexOffset = 0;
}


Box::~Box()
{

}

void Box::MakeGeometry()
{
    //MakeNormal();
    MakeColor();
}

void Box::MakeNormal()
{
    std::vector<Vertex::NtPCVertex> vertices;

    Vertex::NtPCVertex box[] =
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

    vertices.assign(std::begin(box), std::end(box));
    m_boxVertexCount = _countof(box);
    m_pyramidVertexOffset = (UINT)vertices.size();

    Vertex::NtPCVertex pyramid[] =
    {
        { XMFLOAT3(-5.0f, -2.0f, -2.0f), (const float*)&Colors::Green },
        { XMFLOAT3(-5.0f, -2.0f, +2.0f), (const float*)&Colors::Green },
        { XMFLOAT3(+5.0f, -2.0f, +2.0f), (const float*)&Colors::Green },
        { XMFLOAT3(+5.0f, -2.0f, -2.0f), (const float*)&Colors::Green },
        { XMFLOAT3(0.0f, 4.0f, 0.0f), (const float*)&Colors::Red },
    };

    vertices.insert(vertices.end(), std::begin(pyramid), std::end(pyramid));
    m_pyramidVertexCount = _countof(pyramid);

    std::vector<UINT> indices;
    ntUint box_indices[] =
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


    indices.assign(std::begin(box_indices), std::end(box_indices));
    m_pyramidIndexOffset = (UINT)indices.size();
    m_boxIndexCount = (UINT)indices.size();

    ntUint pyramid_indices[] =
    {
        // Á¤¸é
        0, 4, 3,
        // ¿·¸é 1
        1, 4, 0,
        // ¿·¸é 2
        2, 4, 1,
        3, 4, 2,

        1, 0, 3,
        1, 3, 2,
    };

    indices.insert(indices.end(), std::begin(pyramid_indices), std::end(pyramid_indices));
    m_pyramidIndexCount = _countof(pyramid_indices);

    Vertex::NtPCVertex* v = &vertices[0];
    UINT* i = &indices[0];

    InitializeModelData(v, sizeof(Vertex::NtPCVertex), (ntInt)vertices.size(), i, (ntInt)indices.size());
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
}

void Box::MakeColor()
{
    std::vector<sVertexColor> vertices;

    sVertexColor box[] =
    {
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), Colors::YellowC },
        { XMFLOAT3(-1.0f, +1.0f, -1.0f), Colors::YellowC },
        { XMFLOAT3(+1.0f, +1.0f, -1.0f), Colors::RedC },
        { XMFLOAT3(+1.0f, -1.0f, -1.0f), Colors::GreenC },
        { XMFLOAT3(-1.0f, -1.0f, +1.0f), Colors::BlueC },
        { XMFLOAT3(-1.0f, +1.0f, +1.0f), Colors::YellowC },
        { XMFLOAT3(+1.0f, +1.0f, +1.0f), Colors::MagentaC },
        { XMFLOAT3(+1.0f, -1.0f, +1.0f), Colors::MagentaC },
    };

    vertices.assign(std::begin(box), std::end(box));
    m_boxVertexCount = _countof(box);

    std::for_each(std::begin(vertices), std::end(vertices), [](sVertexColor& unit) {
        unit.m_color = Colors::ArgbToAbgr(unit.m_color);
    });
    
    std::vector<UINT> indices;
    ntUint box_indices[] =
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

    indices.assign(std::begin(box_indices), std::end(box_indices));
    m_boxIndexCount = (ntInt)indices.size();

    sVertexColor* vtxPtr = &vertices[0];
    UINT* idxPtr = &indices[0];

    InitializeModelData((void*)vtxPtr, sizeof(sVertexColor), (ntInt)vertices.size(), idxPtr, m_boxIndexCount);
}

void Box::Render(XMMATRIX& worldViewProj)
{
    //RenderNormal(worldViewProj);
    //RenderBoxPyramid(worldViewProj);
    Render32BitColor(worldViewProj);
}

void Box::RenderNormal(XMMATRIX& worldViewProj)
{
    NtModel::Render(worldViewProj);
}

void Box::RenderTwoVertexBuf(XMMATRIX& worldViewProj)
{
    ntUint strides[] =
    {
        sizeof(sVertex),
        sizeof(sColor)
    };

    ntUint offsets[] = { 0, 0 };

    g_renderInterface->SetPrimitiveTopology(PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    g_renderInterface->SetVertexBuffers(0, 2, m_buffers, strides, offsets);

    g_renderInterface->SetIndexBuffers(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    auto& colorShader = NtShaderHandler::ColorShader;

    g_renderer->DeviceContext()->IASetInputLayout(NtInputLayoutHandler::PCInputLayout);

    XMMATRIX view;
    XMMATRIX proj;

    g_renderer->GetViewMatrix(view);
    g_renderer->GetProjectionMatrix(proj);

    XMMATRIX viewProj = view * proj;

    D3DX11_TECHNIQUE_DESC techDesc;

    ID3DX11EffectTechnique* tech = const_cast<ID3DX11EffectTechnique*>(colorShader->ColorTech);
    ID3DX11EffectMatrixVariable* effectMatrix = const_cast<ID3DX11EffectMatrixVariable*>(colorShader->GetWorldViewProj());

    tech->GetDesc(&techDesc);

    for (UINT p = 0; p < techDesc.Passes; ++p)
    {
        XMMATRIX world = XMLoadFloat4x4(&m_boxWorld);
        effectMatrix->SetMatrix(reinterpret_cast<float*>(&(world * viewProj)));

        tech->GetPassByIndex(p)->Apply(0, g_renderer->DeviceContext());
        g_renderer->DeviceContext()->DrawIndexed(m_indexCount, 0, 0);
    }
}

void Box::RenderBoxPyramid(XMMATRIX& worldViewProj)
{
    UINT stride = sizeof(Vertex::NtPCVertex);
    UINT offset = 0;

    g_renderInterface->SetPrimitiveTopology(PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    g_renderInterface->SetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    g_renderInterface->SetIndexBuffers(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    auto& colorShader = NtShaderHandler::ColorShader;

    g_renderer->DeviceContext()->IASetInputLayout(NtInputLayoutHandler::PCInputLayout);

    XMMATRIX view;
    XMMATRIX proj;

    g_renderer->GetViewMatrix(view);
    g_renderer->GetProjectionMatrix(proj);

    XMMATRIX viewProj = view * proj;

    D3DX11_TECHNIQUE_DESC techDesc;

    ID3DX11EffectTechnique* tech = const_cast<ID3DX11EffectTechnique*>(colorShader->ColorTech);
    ID3DX11EffectMatrixVariable* effectMatrix = const_cast<ID3DX11EffectMatrixVariable*>(colorShader->GetWorldViewProj());

    tech->GetDesc(&techDesc);

    XMMATRIX world;
    for (UINT p = 0; p < techDesc.Passes; ++p)
    {
        world = XMLoadFloat4x4(&m_boxWorld);
        effectMatrix->SetMatrix(reinterpret_cast<float*>(&(world * viewProj)));

        tech->GetPassByIndex(p)->Apply(0, g_renderer->DeviceContext());
        g_renderer->DeviceContext()->DrawIndexed(m_boxIndexCount, 0, 0);

        world = XMLoadFloat4x4(&m_pyramidWorld);
        effectMatrix->SetMatrix(reinterpret_cast<float*>(&(world * viewProj)));

        tech->GetPassByIndex(p)->Apply(0, g_renderer->DeviceContext());
        g_renderer->DeviceContext()->DrawIndexed(m_pyramidIndexCount, m_pyramidIndexOffset, m_pyramidVertexOffset);
    }
}

void Box::Render32BitColor(XMMATRIX& worldViewProj)
{
    ntUint stride = sizeof(sVertexColor);
    ntUint offset = 0;

    g_renderInterface->SetPrimitiveTopology(PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    g_renderInterface->SetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    g_renderInterface->SetIndexBuffers(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    auto& colorShader = NtShaderHandler::ColorShader;

    colorShader->RenderFx(m_indexCount, worldViewProj);
}
