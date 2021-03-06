#include "stdafx.h"
#pragma hdrstop

#include "WaveModel.h"
#include "NtMath.h"
#include "NtGeometryGenerator.h"
#include "NtColorShader.h"
#include "NtLightShader.h"
#include "NtShaderHandler.h"
#include "NtInputLayout.h"
#include "NtSampleShader.h"
#include "NtTexture.h"

using namespace nt;

//#define SAMPLE_LIGHT_SETTING 1

namespace
{
	float GetHeight(float x, float z)
	{
		return 0.3f * (z * NtMathf::Sin(0.1f * x) * NtMathf::Cos(0.1f * z));
	}

    XMFLOAT3 GetHillNormal(float x, float z)
    {
        // n = (-df/dx, 1, -df/dz)
        XMFLOAT3 n(
            -0.03f* z * NtMathf::Cos(0.1f * x) - 0.3f * NtMathf::Cos(0.1f * z),
            1.0f,
            -0.3f* NtMathf::Sin(0.1f * x) + 0.03f * x * NtMathf::Sin(0.1f * z));

        XMVECTOR unitNormal = XMVector3Normalize(XMLoadFloat3(&n));
        XMStoreFloat3(&n, unitNormal);

        return n;
    }

    enum RenderOption
    {
        Lighting = 0,
        Textures = 1,
        TexturesAndFog = 2
    };

    RenderOption RENDER_OPTION = RenderOption::TexturesAndFog;
}

WaveModel::WaveModel()
	: m_waveVB(nullptr)
	, m_waveIB(nullptr)
    , m_boxVB(nullptr)
    , m_boxIB(nullptr)
    , m_grassMapSRV(nullptr)
    , m_waveMapSRV(nullptr)
    , m_boxMapSRV(nullptr)
    , m_gridIndexCount(0)
    , m_boxIndexCount(0)
    , m_lightCount(1)
{
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&m_gridWorld, I);
	XMStoreFloat4x4(&m_wavesWorld, I);
    XMStoreFloat4x4(&m_boxWorld, I);
    XMStoreFloat4x4(&m_cylinderWorld, I);

    XMMATRIX boxScale = XMMatrixScaling(15.0f, 15.0f, 15.0f);
    XMMATRIX boxOffset = XMMatrixTranslation(8.0f, 5.0f, -15.0f);
    XMStoreFloat4x4(&m_boxWorld, boxScale*boxOffset);

    XMMATRIX cylinderOffset = XMMatrixTranslation(8.0f, 5.0f, 15.0f);
    XMStoreFloat4x4(&m_cylinderWorld, boxScale * cylinderOffset);

    XMMATRIX grassTexScale = XMMatrixScaling(5.0f, 5.0f, 0.0f);
    XMStoreFloat4x4(&m_grassTexTransform, grassTexScale);

    //XMMATRIX wavOffset = XMMatrixTranslation(0.0f, -3.0f, 0.0f);
    //XMStoreFloat4x4(&m_wavesWorld, wavOffset);

    m_dirLight[0].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    m_dirLight[0].Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_dirLight[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_dirLight[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

    m_dirLight[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    m_dirLight[1].Diffuse = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
    m_dirLight[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
    m_dirLight[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

    m_dirLight[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    m_dirLight[2].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    m_dirLight[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    m_dirLight[2].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);

    m_landMaterial.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_landMaterial.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_landMaterial.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);

    m_wavMaterial.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_wavMaterial.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
    m_wavMaterial.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 32.0f);

#if 0
#if SAMPLE_LIGHT_SETTING
    m_dirLight.Ambient = XMFLOAT4(0.5f, .0f, .0f, 1.0f);
#else
    m_dirLight.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
#endif
    m_dirLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_dirLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_dirLight.Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

    // 점광원 - 위치는 애니메이션을 위해 매 프레임 Update에서 갱신된다.
#if SAMPLE_LIGHT_SETTING
    m_pointLight.Ambient = XMFLOAT4(.0f, 0.3f, .0f, 1.0f);
    m_pointLight.Diffuse = XMFLOAT4(.0f, 0.7f, .0f, 1.0f);
    m_pointLight.Specular = XMFLOAT4(.0f, 0.7f, .0f, 1.0f);
#else
    m_pointLight.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
    m_pointLight.Diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
    m_pointLight.Specular = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
#endif
    m_pointLight.Att = XMFLOAT3(0.0f, 0.1f, 0.0f);
    m_pointLight.Range = 25.0f;

    // 점적광원 설정 - 위치는 애니메이션을 위해 매 프레임 갱신
    m_spotLight.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    m_spotLight.Diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
    m_spotLight.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_spotLight.Att = XMFLOAT3(1.0f, 0.0f, 0.0f);
    m_spotLight.Spot = 96.0f;
    m_spotLight.Range = 10000.0f;

    m_landMaterial.Ambient = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
    m_landMaterial.Diffuse = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
    m_landMaterial.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);

    m_wavMaterial.Ambient = XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
    m_wavMaterial.Diffuse = XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
    m_wavMaterial.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 96.0f);
#endif

    m_boxMaterial.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_boxMaterial.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_boxMaterial.Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 16.0f);

    m_cylinderMaterial.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_cylinderMaterial.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_cylinderMaterial.Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 16.0f);
}

WaveModel::~WaveModel()
{
    SAFE_RELEASE(m_quadVB);
    SAFE_RELEASE(m_quadIB);
	SAFE_RELEASE(m_waveVB);
	SAFE_RELEASE(m_waveIB);
    SAFE_RELEASE(m_boxVB);
    SAFE_RELEASE(m_boxIB);
    SAFE_RELEASE(m_cylinderVB);
    SAFE_RELEASE(m_cylinderIB);
    SAFE_RELEASE(m_grassMapSRV);
    SAFE_RELEASE(m_waveMapSRV);
    SAFE_RELEASE(m_boxMapSRV);
}

void WaveModel::Update(float deltaTime)
{
	NtModel::Update(deltaTime);

	static float timeBase = 0.0f;

    static int k = 0;

	if (g_app->Timer().TotalTime() - timeBase >= 0.25f)
	{
		timeBase += 0.25f;

		DWORD i = 5 + rand() % (m_waves.RowCount() - 10);
		DWORD j = 5 + rand() % (m_waves.ColumnCount() - 10);

		float r = NtMathf::Rand(1.0f, 2.0f);

		m_waves.Disturb(i, j, r);

        m_currBolt = m_bolts[k++];

        if (k >= m_bolts.size())
        {
            k = 0;
        }
	}

	m_waves.Update(deltaTime);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(g_renderer->DeviceContext()->Map(m_waveVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));

	Vertex::PNUVertex* vertices = reinterpret_cast<Vertex::PNUVertex*>(mappedData.pData);
	for (UINT i = 0; i < m_waves.VertexCount(); i++)
	{
		vertices[i].position = m_waves[i];
		//vertices[i].color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
        vertices[i].normal = m_waves.Normal(i);

        // Derive tex-coords in [0,1] from position.
        vertices[i].uv.x = 0.5f + m_waves[i].x / m_waves.Width();
        vertices[i].uv.y = 0.5f - m_waves[i].z / m_waves.Depth();
	}
	g_renderer->DeviceContext()->Unmap(m_waveVB, 0);

    // tile water texture
    XMMATRIX wavesScale = XMMatrixScaling(5.0f, 5.0f, 0.0f);

    // translate texture over time
    m_waterTexOffset.y += 0.05f * deltaTime;
    m_waterTexOffset.x += 0.1f * deltaTime;
    XMMATRIX wavesOffset = XMMatrixTranslation(m_waterTexOffset.x, m_waterTexOffset.y, 0.0f);

    // combine scale and translation
    XMStoreFloat4x4(&m_waterTexTransform, wavesScale * wavesOffset);

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
}

void WaveModel::Render(XMMATRIX& worldViewProj)
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

    LightShader->SetDirLights(m_dirLight);
    LightShader->SetEyePosW(m_eyePosW);
    LightShader->SetFogColor(Colors::Silver);
    LightShader->SetFogStart(15.0f);
    LightShader->SetFogRange(175.0f);
    
    
    ID3DX11EffectTechnique* tech;
    ID3DX11EffectTechnique* boxTech;
    
    switch (RENDER_OPTION)
    {
    case RenderOption::Lighting:
        boxTech = LightShader->Light3Tech();
        tech = LightShader->Light3Tech();
        break;
    case RenderOption::Textures:
        boxTech = LightShader->Light3TexAlphaClipTech();
        tech = LightShader->Light3TexAlphaClipTech();
        break;
    case RenderOption::TexturesAndFog:
        boxTech = LightShader->Light3TexAlphaClipFogTech();
        tech = LightShader->Light3TexAlphaClipFogTech();
        break;
    }

    D3DX11_TECHNIQUE_DESC techDesc;
	

    // draw the box
    //boxTech->GetDesc(&techDesc);
    //for (UINT p = 0; p < techDesc.Passes; ++p)
    //{
    //    g_renderInterface->SetVertexBuffers(0, 1, &m_boxVB, &stride, &offset);
    //    g_renderInterface->SetIndexBuffers(m_boxIB, DXGI_FORMAT_R32_UINT, 0);

    //    XMMATRIX world = XMLoadFloat4x4(&m_boxWorld);
    //    XMMATRIX worldInvTranspose = NtD3dUtil::InverseTranspose(world);
    //    XMMATRIX goWVP = world * viewProj;

    //    LightShader->SetWorld(world);
    //    LightShader->SetWorldInvTranspose(worldInvTranspose);
    //    LightShader->SetWorldViewProj(goWVP);
    //    LightShader->SetTexTransform(XMMatrixIdentity());
    //    LightShader->SetMaterial(m_boxMaterial);
    //    LightShader->SetDiffuseMap(m_boxMapSRV);

    //    g_renderer->DeviceContext()->RSSetState(NtRenderStateHandler::RSNoCull);
    //    boxTech->GetPassByIndex(p)->Apply(0, g_renderer->DeviceContext());
    //    g_renderer->DeviceContext()->DrawIndexed(m_boxIndexCount, 0, 0);

    //    // restore default render state
    //    g_renderer->DeviceContext()->RSSetState(nullptr);
    //}

    
    tech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
        // set per object constant.
        // draw the grid
        g_renderInterface->SetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
        g_renderInterface->SetIndexBuffers(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

        XMMATRIX world = XMLoadFloat4x4(&m_gridWorld);
        XMMATRIX worldInvTranspose = NtD3dUtil::InverseTranspose(world);
        XMMATRIX goWorldViewProj = world * viewProj;

        LightShader->SetWorld(world);
        LightShader->SetWorldInvTranspose(worldInvTranspose);
        LightShader->SetWorldViewProj(goWorldViewProj);
        LightShader->SetTexTransform(XMLoadFloat4x4(&m_grassTexTransform));
        LightShader->SetMaterial(m_landMaterial);
        LightShader->SetDiffuseMap(m_grassMapSRV);

        tech->GetPassByIndex(p)->Apply(0, g_renderer->DeviceContext());
        g_renderer->DeviceContext()->DrawIndexed(m_gridIndexCount, 0, 0);

		// draw the waves
        g_renderInterface->SetVertexBuffers(0, 1, &m_waveVB, &stride, &offset);
        g_renderInterface->SetIndexBuffers(m_waveIB, DXGI_FORMAT_R32_UINT, 0);

        world = XMLoadFloat4x4(&m_wavesWorld);
        worldInvTranspose = NtD3dUtil::InverseTranspose(world);
        goWorldViewProj = world * viewProj;

        LightShader->SetWorld(world);
        LightShader->SetWorldInvTranspose(worldInvTranspose);
        LightShader->SetWorldViewProj(goWorldViewProj);
        LightShader->SetTexTransform(XMLoadFloat4x4(&m_waterTexTransform));
        LightShader->SetMaterial(m_wavMaterial);
        LightShader->SetDiffuseMap(m_waveMapSRV);
		
        g_renderer->DeviceContext()->OMSetBlendState(NtRenderStateHandler::BSTransparent, blendFactor, 0xffffffff);

		tech->GetPassByIndex(p)->Apply(0, g_renderer->DeviceContext());
		g_renderer->DeviceContext()->DrawIndexed(3 * m_waves.TrisCount(), 0, 0);

        g_renderer->DeviceContext()->OMSetBlendState(nullptr, blendFactor, 0xffffffff);
	}

    // draw the cylinder
    boxTech->GetDesc(&techDesc);
    for (size_t i = 0; i < techDesc.Passes; i++)
    {
        g_renderInterface->SetVertexBuffers(0, 1, &m_cylinderVB, &stride, &offset);
        g_renderInterface->SetIndexBuffers(m_cylinderIB, DXGI_FORMAT_R32_UINT, 0);

        XMMATRIX world = XMLoadFloat4x4(&m_cylinderWorld);
        XMMATRIX worldInvTranspose = NtD3dUtil::InverseTranspose(world);
        XMMATRIX goWVP = world * viewProj;

        LightShader->SetWorld(world);
        LightShader->SetWorldInvTranspose(worldInvTranspose);
        LightShader->SetWorldViewProj(goWVP);
        LightShader->SetTexTransform(XMMatrixIdentity());
        LightShader->SetMaterial(m_cylinderMaterial);
        LightShader->SetDiffuseMap(m_currBolt);

        g_renderer->DeviceContext()->OMSetBlendState(NtRenderStateHandler::BSAdd, blendFactor, 0xff);
        g_renderer->DeviceContext()->OMSetDepthStencilState(NtRenderStateHandler::DSNoneDepth, 0);

        boxTech->GetPassByIndex(i)->Apply(0, g_renderer->DeviceContext());
        g_renderer->DeviceContext()->DrawIndexed(m_cylinderIndexCount, 0, 0);

        //g_renderer->DeviceContext()->RSSetState(nullptr);
        g_renderer->DeviceContext()->OMSetDepthStencilState(nullptr, 0);
        g_renderer->DeviceContext()->OMSetBlendState(nullptr, blendFactor, 0xff);
    }
}

void WaveModel::MakeGeometry()
{
	// land
	renderer::NtGeometryGenerator::MeshData grid;
	renderer::NtGeometryGenerator gen;

	gen.CreateGrid(160.0f, 160.0f, 50, 50, grid);

    std::vector<Vertex::PNUVertex> vertices(grid.Vertices.size());

	for (size_t i = 0; i < grid.Vertices.size(); i++)
	{
		XMFLOAT3 p = grid.Vertices[i].Position;

		p.y = GetHeight(p.x, p.z);

		vertices[i].position = p;

		// Color the vertex based on its height.
		//if (p.y < -10.0f)
		//{
		//	// Sandy beach color.
		//	vertices[i].color = XMFLOAT4(1.0f, 0.96f, 0.62f, 1.0f);
		//}
		//else if (p.y < 5.0f)
		//{
		//	// Light yellow-green.
		//	vertices[i].color = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
		//}
		//else if (p.y < 12.0f)
		//{
		//	// Dark yellow-green.
		//	vertices[i].color = XMFLOAT4(0.1f, 0.48f, 0.19f, 1.0f);
		//}
		//else if (p.y < 20.0f)
		//{
		//	// Dark brown.
		//	vertices[i].color = XMFLOAT4(0.45f, 0.39f, 0.34f, 1.0f);
		//}
		//else
		//{
		//	// White snow.
		//	vertices[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		//}

        vertices[i].normal = GetHillNormal(p.x, p.z);
        vertices[i].uv = grid.Vertices[i].TexC;
	}

	std::vector<UINT> indices;
	indices.insert(indices.end(), grid.Indices.begin(), grid.Indices.end());

    m_gridIndexCount = (ntInt)grid.Indices.size();

    Vertex::PNUVertex* vtxArray = &vertices[0];
	UINT* idxArray = &indices[0];

	InitializeModelData(vtxArray, sizeof(Vertex::PNUVertex), (ntInt)vertices.size(), idxArray, (ntInt)indices.size());

	MakeWave();

    g_renderer->CreateShaderResourceView(L"grass.dds", &m_grassMapSRV);

    g_renderer->CreateShaderResourceView(L"water2.dds", &m_waveMapSRV);

    g_renderer->CreateShaderResourceView(L"wirefence.dds", &m_boxMapSRV);
    
    MakeCrate();

    MakeCylinder();

    MakeQuad4Complex();

    std::vector<ntUint> texHandles;
    for (size_t i = 1; i <= 60; i++)
    {
        const ntWchar* file = nt::Crt::MakeString(L"bolt0%02d.bmp", i);

        ntUint handle = g_resMgr->LoadTexture(file);
        if (handle == INVALID_TEXTURE_HANDLE)
        {
            continue;
        }

        texHandles.push_back(handle);
    }

    auto& bolts = m_bolts;
    std::for_each(std::begin(texHandles), std::end(texHandles), [&bolts](const ntUint handle) 
    {
        auto texture = g_resMgr->AcquireTexture(handle);

        bolts.push_back(texture->GetTexture());
    });

    m_currBolt = bolts[0];
}

void WaveModel::MakeWave()
{
	m_waves.Init(160, 160, 1.0f, 0.03f, 3.25f, 0.4f);

	m_waveVB = MakeVertexBuffer(nullptr, sizeof(Vertex::PNUVertex), m_waves.VertexCount(), BufferUsage::USAGE_DYNAMIC, CpuAccessFlag::CPU_ACCESS_WRITE);

	std::vector<UINT> indices(3 * m_waves.TrisCount());

	// iterate over each quad
	UINT m = m_waves.RowCount();
	UINT n = m_waves.ColumnCount();

	int k = 0;
	for (UINT i = 0; i < m - 1; i++)
	{
		for (DWORD j = 0; j < n - 1; j++)
		{
			indices[k] = i * n + j;
			indices[k+1] = i * n + j + 1;
			indices[k+2] = (i + 1) * n + j;

			indices[k + 3] = (i + 1) * n + j;
			indices[k + 4] = i * n + j + 1;
			indices[k + 5] = (i + 1) * n + j + 1;

			// next quad
			k += 6;
		}
	}

	UINT* idxArray = &indices[0];
	m_waveIB = MakeIndexBuffer(idxArray, (ntInt)indices.size());
}

void WaveModel::MakeCrate()
{
    NtGeometryGenerator::MeshData box;

    NtGeometryGenerator geoGen;
    geoGen.CreateBox(1.0f, 1.0f, 1.0f, box);

    std::vector<Vertex::PNUVertex> vertices(box.Vertices.size());

    for (size_t i = 0; i < box.Vertices.size(); i++)
    {
        vertices[i].position = box.Vertices[i].Position;
        vertices[i].normal = box.Vertices[i].Normal;
        vertices[i].uv = box.Vertices[i].TexC;
    }
    
    std::vector<UINT> indices;
    indices.insert(std::end(indices), std::begin(box.Indices), std::end(box.Indices));
    m_boxIndexCount = (ntInt)box.Indices.size();

    Vertex::PNUVertex* vtxArray = &vertices[0];
    UINT* idxArray = &indices[0];

    m_boxVB = MakeVertexBuffer(vtxArray, sizeof(Vertex::PNUVertex), (ntInt)vertices.size(), BufferUsage::USAGE_IMMUTABLE, CpuAccessFlag::CPU_ACCESS_NONE);

    m_boxIB = MakeIndexBuffer(idxArray, (ntInt)indices.size());
}

void WaveModel::MakeCylinder()
{
    renderer::NtGeometryGenerator::MeshData cylinder;
    NtGeometryGenerator gen;

    gen.CreateCylinder(0.5f, 0.3f, 3.0f, 20, 20, cylinder);

    std::vector<Vertex::PNUVertex> vertices(cylinder.Vertices.size());

    for (size_t i = 0; i < vertices.size(); ++i)
    {
        vertices[i].position = cylinder.Vertices[i].Position;
        vertices[i].normal = cylinder.Vertices[i].Normal;
        vertices[i].uv = cylinder.Vertices[i].TexC;
    }

    std::vector<ntUint> indices;
    indices.insert(std::end(indices), std::begin(cylinder.Indices), std::end(cylinder.Indices));
    m_cylinderIndexCount = cylinder.Indices.size();

    Vertex::PNUVertex* vtxArray = &vertices[0];
    ntUint* idxArray = &indices[0];

    m_cylinderVB = MakeVertexBuffer(vtxArray, sizeof(Vertex::PNUVertex), (ntInt)vertices.size(), BufferUsage::USAGE_IMMUTABLE, CpuAccessFlag::CPU_ACCESS_NONE);

    m_cylinderIB = MakeIndexBuffer(idxArray, (ntInt)indices.size());
}

void WaveModel::MakeQuad4Complex()
{
    renderer::NtGeometryGenerator::MeshDataClr quad;
    NtGeometryGenerator gen;

    gen.CreateFullScreenQuad(quad, Colors::_Green);

    std::vector<Vertex::PCVertex> vertices(quad.Vertices.size());

    for (size_t i = 0; i < vertices.size(); ++i)
    {
        vertices[i].position = quad.Vertices[i].Position;
        Crt::MemCpy(&vertices[i].color, &quad.Vertices[i].Color, sizeof(quad.Vertices[i].Color));
    }

    std::vector<ntUint> indices;
    indices.insert(std::end(indices), std::begin(quad.Indices), std::end(quad.Indices));

    Vertex::PCVertex* vtxArray = &vertices[0];
    ntUint* idxArray = &indices[0];

    m_quadVB = MakeVertexBuffer(vtxArray, sizeof(Vertex::PCVertex), (ntInt)vertices.size(), BufferUsage::USAGE_IMMUTABLE, CpuAccessFlag::CPU_ACCESS_NONE);
    m_quadIB = MakeIndexBuffer(idxArray, (ntInt)indices.size());
}
