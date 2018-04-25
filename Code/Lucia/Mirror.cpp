#include "stdafx.h"
#pragma  hdrstop

#include "Mirror.h"
#include "NtLightShader.h"
#include "NtShaderHandler.h"

using namespace nt;

namespace
{
    enum RenderOption
    {
        Lighting = 0,
        Textures = 1,
        TexturesAndFog = 2
    };

    RenderOption RENDER_OPTION = RenderOption::TexturesAndFog;
}

Mirror::Mirror()
    : m_skullVB(nullptr)
    , m_skullIB(nullptr)
    , m_floorDiffuseMapSRV(nullptr)
    , m_wallDiffuseMapSRV(nullptr)
    , m_mirrorDiffuseMapSRV(nullptr)
{
    XMMATRIX I = XMMatrixIdentity();
    XMStoreFloat4x4(&m_roomWorld, I);
    XMStoreFloat4x4(&m_skullWorld, I);

    m_dirLights[0].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    m_dirLights[0].Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_dirLights[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_dirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

    m_dirLights[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    m_dirLights[1].Diffuse = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
    m_dirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
    m_dirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

    m_dirLights[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    m_dirLights[2].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    m_dirLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    m_dirLights[2].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);

    m_roomMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_roomMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_roomMat.Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 16.0f);

    m_skullMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_skullMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_skullMat.Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 16.0f);

    // Reflected material is transparent so it blends into mirror.
    m_mirrorMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_mirrorMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
    m_mirrorMat.Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 16.0f);

    m_shadowMat.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    m_shadowMat.Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);
    m_shadowMat.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 16.0f);
}

Mirror::~Mirror()
{
    g_renderer->DeviceContext()->ClearState();
}

void Mirror::Update(float deltaTime)
{

}

void Mirror::Render(XMMATRIX& worldViewProj)
{

}


void Mirror::MakeGeometry()
{
    //ntWchar* fileName = g_resMgr->GetPath(L"checkboard.dds");
    g_renderer->CreateShaderResourceView(L"checkboard.dds", &m_floorDiffuseMapSRV);

    g_renderer->CreateShaderResourceView(L"brick01.dds", &m_wallDiffuseMapSRV);

    g_renderer->CreateShaderResourceView(L"ice.dds", &m_floorDiffuseMapSRV);

    BuildRoom();

    BuildSkull();
}


void Mirror::BuildRoom()
{
    // Create and specify geometry.  For this sample we draw a floor
    // and a wall with a mirror on it.  We put the floor, wall, and
    // mirror geometry in one vertex buffer.
    //
    //   |--------------|
    //   |              |
    //   |----|----|----|
    //   |Wall|Mirr|Wall|
    //   |    | or |    |
    //   /--------------/
    //  /   Floor      /
    // /--------------/

    Vertex::PNUVertex v[30] = 
    {
        // Floor: Observe we tile texture coord
        {-3.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 4.0f},
        {-3.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
        { 7.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, 4.0f, 0.0f},

        {-3.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 4.0f},
        { 7.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, 4.0f, 0.0f },
        { 7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 4.0f, 4.0f },

        // Wall : Observe we tile texture coord, and that we
        // leabe a gap in the middle for the mirror
        { -3.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f },
        { -3.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f },
        { -2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 0.0f },

        { -3.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f },
        { -2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 0.0f },
        { -2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 2.0f },

        { 2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f },
        { 2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f },
        { 7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 0.0f },

        { 2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f},
        { 7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 0.0f},
        { 7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 2.0f},

        { -3.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f},
        { -3.5f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f},
        { 7.5f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f, 6.0f, 0.0f },

        { -3.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f},
        { 7.5f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f, 6.0f, 0.0f},
        { 7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 6.0f, 1.0f},

        // Mirror
        {-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f},
        {-2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f},
        {2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f },

        { -2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f},
        { 2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f },
        { 2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f },
    };
}

void Mirror::BuildSkull()
{

}
