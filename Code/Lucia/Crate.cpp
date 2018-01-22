#include "stdafx.h"
#pragma hdrstop

#include "Crate.h"
#include "NtLightShader.h"
#include "NtGeometryGenerator.h"

using namespace nt;
using namespace nt::renderer;

Crate::Crate()
    : m_diffuseMapSRV(nullptr)
    , m_boxVertexOffset(0)
    , m_boxIndexOffset(0)
{
    XMMATRIX I = XMMatrixIdentity();
    XMStoreFloat4x4(&m_boxWorld, I);
    XMStoreFloat4x4(&m_texTransform, I);
    XMStoreFloat4x4(&m_viewProj, I);

    /*XMMATRIX boxOffset = XMMatrixTranslation(0.0f, 0.0f, -4.0f);
    XMStoreFloat4x4(&m_boxWorld, boxOffset);*/

    m_dirLights[0].Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
    m_dirLights[0].Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
    m_dirLights[0].Specular = XMFLOAT4(0.6f, 0.6f, 0.6f, 16.0f);
    m_dirLights[0].Direction = XMFLOAT3(0.707f, -0.707f, 0.0f);

    m_dirLights[1].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    m_dirLights[1].Diffuse = XMFLOAT4(1.4f, 1.4f, 1.4f, 1.0f);
    m_dirLights[1].Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 16.0f);
    m_dirLights[1].Direction = XMFLOAT3(-0.707f, 0.0f, 0.707f);

    m_boxMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_boxMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_boxMat.Specular = XMFLOAT4(0.6f, 0.6f, 0.6f, 16.0f);
}

Crate::~Crate()
{
    //SAFE_RELEASE(m_AddSRV);
    SAFE_RELEASE(m_diffuseMapSRV);
}

void Crate::Update(float deltaTime)
{
    NtModel::Update(deltaTime);

    static float angle = 0.0f;
    static XMMATRIX texTransform;
    // angle
    //m_texTransform
    angle += (deltaTime * 1.0f);
    
    texTransform = XMMatrixRotationZ(angle);
    XMStoreFloat4x4(&m_texTransform, texTransform);

    if (angle >= 360.f)
    {
        angle = 0.0f;
    }
}

void Crate::Render(XMMATRIX& worldViewProj)
{
    ntUint stride = sizeof(Vertex::NtPNUVertex);
    ntUint offset = 0;

    g_renderInterface->SetPrimitiveTopology(PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    g_renderer->DeviceContext()->IASetInputLayout(NtInputLayoutHandler::PNUInputLayout);

    XMMATRIX viewProj = XMLoadFloat4x4(&m_viewProj);
    g_renderer->GetViewProjMatrix(viewProj);

    auto& lightShader = NtShaderHandler::LightShader;

    // set per frame constants
    lightShader->SetDirLights(m_dirLights);
    lightShader->SetEyePosW(m_eyePosW);

    ID3DX11EffectTechnique* activeTech = lightShader->Light2TexTech();

    D3DX11_TECHNIQUE_DESC techDesc;
    activeTech->GetDesc(&techDesc);
    for (UINT p = 0; p < techDesc.Passes; ++p)
    {
        g_renderInterface->SetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &m_boxVertexOffset);
        g_renderInterface->SetIndexBuffers(m_indexBuffer, DXGI_FORMAT_R32_UINT, m_boxIndexOffset);

        XMMATRIX world = XMLoadFloat4x4(&m_boxWorld);
        XMMATRIX worldInvTranspose = NtD3dUtil::InverseTranspose(world);
        XMMATRIX worldViewProj = world * viewProj;

        lightShader->SetWorld(world);
        lightShader->SetWorldInvTranspose(worldInvTranspose);
        lightShader->SetWorldViewProj(worldViewProj);
        lightShader->SetTexTransform(XMLoadFloat4x4(&m_texTransform));
        lightShader->SetMaterial(m_boxMat);
        lightShader->SetDiffuseMap(m_diffuseMapSRV);
        //lightShader->SetBlendMap(m_AddSRV);

        activeTech->GetPassByIndex(p)->Apply(0, g_renderer->DeviceContext());
        g_renderer->DeviceContext()->DrawIndexed(m_indexCount, 0, 0);
    }
}

void Crate::MakeGeometry()
{
    //const ntWchar* filePath = g_resMgr->GetPath(L"woodcrate01.dds");
    const ntWchar* filePath = g_resMgr->GetPath(L"flare.dds");
    NtAsserte(filePath != nullptr);

    HR(D3DX11CreateShaderResourceViewFromFile(
        g_renderer->Device(),
        filePath,
        0, 0,
        &m_diffuseMapSRV, 0));

    /*filePath = g_resMgr->GetPath(L"flarealpha.dds");
    HR(D3DX11CreateShaderResourceViewFromFile(
        g_renderer->Device(),
        filePath,
        0, 0, &m_AddSRV, 0));*/

    // 텍스쳐 압축
    /*filePath = g_resMgr->GetPath(L"darkbrick.bmp");
    D3DX11_IMAGE_LOAD_INFO loadInfo;
    loadInfo.Format = DXGI_FORMAT_BC3_UNORM;

    ID3D11ShaderResourceView* srv;
    HR(D3DX11CreateShaderResourceViewFromFile(
        g_renderer->Device(),
        filePath,
        &loadInfo,
        0,
        &srv, 0));

    ID3D11Texture2D* tex;
    srv->GetResource((ID3D11Resource**)&tex);

    D3D11_TEXTURE2D_DESC texDesc;
    tex->GetDesc(&texDesc);

    SAFE_RELEASE(srv);*/

    NtGeometryGenerator::MeshData box;

    NtGeometryGenerator geoGen;
    geoGen.CreateBox(1.0f, 1.0f, 1.0f, box);

    m_boxVertexOffset = 0;
    m_boxIndexOffset = 0;

    UINT totalVertexCount = box.Vertices.size();
    UINT totalIndexCount = box.Indices.size();

    std::vector<Vertex::NtPNUVertex> vertices(totalVertexCount);

    for (size_t i = 0; i < totalVertexCount; ++i)
    {
        vertices[i].position = box.Vertices[i].Position;
        vertices[i].normal = box.Vertices[i].Normal;
        vertices[i].uv = box.Vertices[i].TexC;
    }

    InitializeModelData(
        &vertices[0],
        sizeof(nt::Vertex::NtPNUVertex),
        (int)vertices.size(),
        &box.Indices[0],
        (int)totalIndexCount);
}
