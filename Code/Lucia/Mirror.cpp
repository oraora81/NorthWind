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

    RenderOption RENDER_OPTION = RenderOption::Textures;
}

Mirror::Mirror()
    : NtModel()
	, m_skullVB(nullptr)
    , m_skullIB(nullptr)
    , m_floorDiffuseMapSRV(nullptr)
    , m_wallDiffuseMapSRV(nullptr)
    , m_mirrorDiffuseMapSRV(nullptr)
	, m_skullTranslation(0.0f, 1.0f, -5.0f)
    , m_skullIndexCount(0)
{
	m_phi = 0.42f * NtMathf::PI;
	m_radius = 12.0f;

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
    NtModel::Update(deltaTime);

    // switch render mode
    if (GetAsyncKeyState('1') & 0x8000)
    {
        RENDER_OPTION = RenderOption::Lighting;
    }

    if (GetAsyncKeyState('2') & 0x8000)
    {
        RENDER_OPTION = RenderOption::Textures;
    }

    if (GetAsyncKeyState('3') & 0x8000)
    {
        RENDER_OPTION = RenderOption::TexturesAndFog;
    }

	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_skullTranslation.x -= 1.0f*deltaTime;
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_skullTranslation.x += 1.0f*deltaTime;
	}

	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_skullTranslation.y += 1.0f*deltaTime;
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_skullTranslation.y -= 1.0f*deltaTime;
	}

    m_skullTranslation.y = NtMathf::Max(m_skullTranslation.y, 0.0f);

    XMMATRIX skullRot = XMMatrixRotationY(0.5f * NtMathf::PI);
    XMMATRIX skullScale = XMMatrixScaling(0.45f, 0.45f, 0.45f);
    XMMATRIX skullOffset = XMMatrixTranslation(m_skullTranslation.x, m_skullTranslation.y, m_skullTranslation.z);
    XMStoreFloat4x4(&m_skullWorld, skullRot*skullScale*skullOffset);
}

void Mirror::Render(XMMATRIX& worldViewProj)
{
    ntUint stride = sizeof(Vertex::PNUVertex);
    ntUint offset = 0;

    g_renderInterface->SetPrimitiveTopology(PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    g_renderer->DeviceContext()->IASetInputLayout(NtInputLayoutHandler::PNUInputLayout);

    float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

    XMMATRIX view;
    XMMATRIX proj;
    g_renderer->GetViewMatrix(view);
    g_renderer->GetProjectionMatrix(proj);

    XMMATRIX viewProj = view * proj;

    auto& LightShader = NtShaderHandler::LightShader;

    LightShader->SetDirLights(m_dirLights);
    LightShader->SetEyePosW(m_eyePosW);
    LightShader->SetFogColor(Colors::LightSteelBlue);
    LightShader->SetFogStart(2.0f);
    LightShader->SetFogRange(40.0f);


    ID3DX11EffectTechnique* tech;
    ID3DX11EffectTechnique* skullTech;

    switch (RENDER_OPTION)
    {
    case RenderOption::Lighting:
         tech = LightShader->Light3Tech();
         skullTech = LightShader->Light3Tech();
        break;
    case RenderOption::Textures:
         tech = LightShader->Light3TexTech();
         skullTech = LightShader->Light3Tech();
        break;
    case RenderOption::TexturesAndFog:
        tech = LightShader->Light3TexFogTech();
		skullTech = LightShader->Light3FogTech();
        break;
    }

    D3DX11_TECHNIQUE_DESC techDesc;

    // draw floor and wall to the back buffer as normal
    tech->GetDesc(&techDesc);
    for (ntUint i = 0; i < techDesc.Passes; i++)
    {
        ID3DX11EffectPass* pass = tech->GetPassByIndex(i);

        g_renderer->DeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

        // set per object constants
        XMMATRIX world = XMLoadFloat4x4(&m_roomWorld);
        XMMATRIX worldInvTranspose = NtD3dUtil::InverseTranspose(world);
        XMMATRIX goWVP = world * viewProj;

        LightShader->SetWorld(world);
        LightShader->SetWorldInvTranspose(worldInvTranspose);
        LightShader->SetWorldViewProj(goWVP);
        LightShader->SetTexTransform(XMMatrixIdentity());
        LightShader->SetMaterial(m_roomMat);

        // floor
        LightShader->SetDiffuseMap(m_floorDiffuseMapSRV);
        pass->Apply(0, g_renderer->DeviceContext());
        g_renderer->DeviceContext()->Draw(6, 0);

        // Wall
        LightShader->SetDiffuseMap(m_wallDiffuseMapSRV);
        pass->Apply(0, g_renderer->DeviceContext());
        g_renderer->DeviceContext()->Draw(18, 6);
    }

    // draw the skull to the back buffer as normal
    skullTech->GetDesc(&techDesc);
    for (ntUint i = 0; i < techDesc.Passes; i++)
    {
        ID3DX11EffectPass* pass = skullTech->GetPassByIndex(i);

        g_renderer->DeviceContext()->IASetVertexBuffers(0, 1, &m_skullVB, &stride, &offset);
        g_renderer->DeviceContext()->IASetIndexBuffer(m_skullIB, DXGI_FORMAT_R32_UINT, 0);

        XMMATRIX world = XMLoadFloat4x4(&m_skullWorld);
        XMMATRIX worldInvTranspose = NtD3dUtil::InverseTranspose(world);
        XMMATRIX goWVP = world * viewProj;

        LightShader->SetWorld(world);
        LightShader->SetWorldInvTranspose(worldInvTranspose);
        LightShader->SetWorldViewProj(goWVP);
        LightShader->SetMaterial(m_skullMat);

        pass->Apply(0, g_renderer->DeviceContext());
        g_renderer->DeviceContext()->DrawIndexed(m_skullIndexCount, 0, 0);
    }

	// draw the mirror to stencil buffer only
	tech->GetDesc(&techDesc);
	for (ntUint i = 0; i < techDesc.Passes; i++)
	{
		ID3DX11EffectPass* pass = tech->GetPassByIndex(i);

		g_renderer->DeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

		// set per object constants
		XMMATRIX world = XMLoadFloat4x4(&m_roomWorld);
		XMMATRIX worldInvTranspose = NtD3dUtil::InverseTranspose(world);
		XMMATRIX goWVP = world * viewProj;

		LightShader->SetWorld(world);
		LightShader->SetWorldInvTranspose(worldInvTranspose);
		LightShader->SetWorldViewProj(goWVP);
		LightShader->SetTexTransform(XMMatrixIdentity());

		// do not write to render target
		g_renderer->DeviceContext()->OMSetBlendState(NtRenderStateHandler::BSNoRenderTargetWrite, blendFactor, 0xffffffff);

		// render visible mirror pixels to stencil buffer
		// do not write mirror depth to depth buffer at this point, otherwise it will occlude the reflection
		g_renderer->DeviceContext()->OMSetDepthStencilState(NtRenderStateHandler::DSMarkMirror, 1);

		pass->Apply(0, g_renderer->DeviceContext());

        // draw mirror
		g_renderer->DeviceContext()->Draw(6, 24);

		// restore states
		g_renderer->DeviceContext()->OMSetDepthStencilState(0, 0);
		g_renderer->DeviceContext()->OMSetBlendState(0, blendFactor, 0xffffffff);
	}

	// draw the skull reflection.
	skullTech->GetDesc(&techDesc);
	for (ntUint i = 0; i < techDesc.Passes; i++)
	{
		ID3DX11EffectPass* pass = skullTech->GetPassByIndex(i);

		g_renderer->DeviceContext()->IASetVertexBuffers(0, 1, &m_skullVB, &stride, &offset);
		g_renderer->DeviceContext()->IASetIndexBuffer(m_skullIB, DXGI_FORMAT_R32_UINT, 0);

		// xy plane
		XMVECTOR mirrorPlane = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		XMMATRIX R = XMMatrixReflect(mirrorPlane);

		XMMATRIX world = XMLoadFloat4x4(&m_skullWorld) * R;
		XMMATRIX worldInvTranspose = NtD3dUtil::InverseTranspose(world);
		XMMATRIX goWVP = world * viewProj;

		LightShader->SetWorld(world);
		LightShader->SetWorldInvTranspose(worldInvTranspose);
		LightShader->SetWorldViewProj(goWVP);
		LightShader->SetMaterial(m_skullMat);
		
		// cache the old light directions, and reflect the light directions
		XMFLOAT3 oldLightDirections[3];
		for (size_t i = 0; i < 3; i++)
		{
			oldLightDirections[i] = m_dirLights[i].Direction;

			XMVECTOR lightDir = XMLoadFloat3(&m_dirLights[i].Direction);
			XMVECTOR reflectedLightDir = XMVector3TransformNormal(lightDir, R);
			XMStoreFloat3(&m_dirLights[i].Direction, reflectedLightDir);
		}

		LightShader->SetDirLights(m_dirLights);

        ID3D11RasterizerState* tempState;
        ID3D11DepthStencilState* tempDS;
        g_renderer->DeviceContext()->OMGetDepthStencilState(&tempDS, 0);
        g_renderer->DeviceContext()->RSGetState(&tempState);

		// cull clockwise triangles for reflections
		g_renderer->DeviceContext()->RSSetState(NtRenderStateHandler::RSCullClockwise);

		// only draw reflection into visible mirrow pixels as marked by the stencil buffer.
		g_renderer->DeviceContext()->OMSetDepthStencilState(NtRenderStateHandler::DSDrawReflection, 1);
		pass->Apply(0, g_renderer->DeviceContext());
		g_renderer->DeviceContext()->DrawIndexed(m_skullIndexCount, 0, 0);

		// restore default state
		g_renderer->DeviceContext()->RSSetState(tempState);
		g_renderer->DeviceContext()->OMSetDepthStencilState(tempDS, 0);

		for (size_t i = 0; i < 3; i++)
		{
			m_dirLights[i].Direction = oldLightDirections[i];
		}

		LightShader->SetDirLights(m_dirLights);
	}

	// draw the mirror to the back buffer as usual but with transparency
	// blending so the reflection shows through
	tech->GetDesc(&techDesc);
	for (ntUint i = 0; i < techDesc.Passes; i++)
	{
		ID3DX11EffectPass* pass = tech->GetPassByIndex(i);

		g_renderer->DeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

		// 
		XMMATRIX world = XMLoadFloat4x4(&m_roomWorld);
		XMMATRIX worldInvTranspose = NtD3dUtil::InverseTranspose(world);
		XMMATRIX goWVP = world * viewProj;

		LightShader->SetWorld(world);
		LightShader->SetWorldInvTranspose(worldInvTranspose);
		LightShader->SetWorldViewProj(goWVP);
		LightShader->SetTexTransform(XMMatrixIdentity());
		LightShader->SetMaterial(m_mirrorMat);
		LightShader->SetDiffuseMap(m_mirrorDiffuseMapSRV);

		// mirror
		g_renderer->DeviceContext()->OMSetBlendState(NtRenderStateHandler::BSTransparent, blendFactor, 0xffffffff);
		pass->Apply(0, g_renderer->DeviceContext());
		g_renderer->DeviceContext()->Draw(6, 24);
	}

	// draw the skull shadow
	skullTech->GetDesc(&techDesc);
	for (ntUint i = 0; i < techDesc.Passes; i++)
	{
		ID3DX11EffectPass* pass = skullTech->GetPassByIndex(i);

		g_renderer->DeviceContext()->IASetVertexBuffers(0, 1, &m_skullVB, &stride, &offset);
		g_renderer->DeviceContext()->IASetIndexBuffer(m_skullIB, DXGI_FORMAT_R32_UINT, 0);

		// xz plane
		XMVECTOR shadowPlane = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMVECTOR toMainLight = -XMLoadFloat3(&m_dirLights[0].Direction);
		XMMATRIX S = XMMatrixShadow(shadowPlane, toMainLight);
		XMMATRIX shadowOffsetY = XMMatrixTranslation(0.0f, 0.001f, 0.0f);

		// set per object constants
		XMMATRIX world = XMLoadFloat4x4(&m_skullWorld) * S * shadowOffsetY;
		XMMATRIX worldInvTranspose = NtD3dUtil::InverseTranspose(world);
		XMMATRIX goWVP = world * viewProj;

		LightShader->SetWorld(world);
		LightShader->SetWorldInvTranspose(worldInvTranspose);
		LightShader->SetWorldViewProj(goWVP);
		LightShader->SetMaterial(m_skullMat);

		g_renderer->DeviceContext()->OMSetDepthStencilState(NtRenderStateHandler::DSNoDoubleBlend, 0);
		pass->Apply(0, g_renderer->DeviceContext());
		g_renderer->DeviceContext()->DrawIndexed(m_skullIndexCount, 0, 0);

		// restore
		g_renderer->DeviceContext()->OMSetBlendState(0, blendFactor, 0xffffffff);
		g_renderer->DeviceContext()->OMSetDepthStencilState(0, 0);
	}
}


void Mirror::MakeGeometry()
{
    //ntWchar* fileName = g_resMgr->GetPath(L"checkboard.dds");
    g_renderer->CreateShaderResourceView(L"checkboard.dds", &m_floorDiffuseMapSRV);

    g_renderer->CreateShaderResourceView(L"brick01.dds", &m_wallDiffuseMapSRV);

    g_renderer->CreateShaderResourceView(L"ice.dds", &m_mirrorDiffuseMapSRV);

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

    D3D11_BUFFER_DESC desc;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.ByteWidth = sizeof(Vertex::PNUVertex) * 30;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = v;

    HR(g_renderer->Device()->CreateBuffer(&desc, &initData, &m_vertexBuffer));
}

void Mirror::BuildSkull()
{
    const ntWchar* fileName = L"skull.txt";

    const ntWchar* path = g_resMgr->GetPath(fileName);

    std::ifstream fin(path);

    NtAsserte(fin.is_open() && "File Load failed : skull.txt");

    UINT vcount = 0;
    UINT tcount = 0;
    std::string ignore;

    fin >> ignore >> vcount;
    fin >> ignore >> tcount;
    fin >> ignore >> ignore >> ignore >> ignore;

    std::vector<Vertex::PNUVertex> vertices(vcount);
    for (UINT i = 0; i < vcount; ++i)
    {
        fin >> vertices[i].position.x >> vertices[i].position.y >> vertices[i].position.z;

        fin >> vertices[i].normal.x >> vertices[i].normal.y >> vertices[i].normal.z;
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

    Vertex::PNUVertex* vtxArray = &vertices[0];
    UINT* idxArray = &indices[0];

    m_skullVB = MakeVertexBuffer(vtxArray, 
                    sizeof(Vertex::PNUVertex), 
                    vertices.size(), 
                    BufferUsage::USAGE_IMMUTABLE, 
                    eCpuAccessFlag::CPU_ACCESS_NONE);

    m_skullIB = MakeIndexBuffer(idxArray, indices.size());
}
