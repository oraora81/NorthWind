#include "stdafx.h"
#pragma hdrstop

#include "Point.h"
#include "NtColorShader.h"
#include "NtShaderHandler.h"
#include "NtInputLayout.h"

using namespace nt;

Points::Points()
    : NtModel()
{

}

Points::~Points()
{

}

void Points::Render(XMMATRIX& worldViewProj)
{
	ntUint stride = sizeof(Vertex::PCVertex);
	ntUint offset = 0;

	g_renderInterface->SetPrimitiveTopology(PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	g_renderInterface->SetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	g_renderInterface->SetIndexBuffers(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    auto& colorShader = NtShaderHandler::ColorShader;

	g_renderer->DeviceContext()->IASetInputLayout(NtInputLayoutHandler::PCInputLayout);

	D3DX11_TECHNIQUE_DESC techDesc;

    ID3DX11EffectTechnique* tech = colorShader->ColorTech;
	ID3DX11EffectMatrixVariable* effectMatrix = const_cast<ID3DX11EffectMatrixVariable*>(colorShader->GetWorldViewProj());

	tech->GetDesc(&techDesc);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
        effectMatrix->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
        tech->GetPassByIndex(p)->Apply(0, g_renderer->DeviceContext());
        g_renderer->DeviceContext()->Draw(m_vertexCount, 0);
	}
}

void Points::MakeGeometry()
{
    Vertex::PCVertex vertices[] =
	{
		{ XMFLOAT3(-5.0f, -2.0f, +1.0f), (const float*)&Colors::White },
		{ XMFLOAT3(-4.0f, +2.0f, +1.0f), (const float*)&Colors::Black },
		{ XMFLOAT3(-3.0f, -1.5f, +1.0f), (const float*)&Colors::Red },
		{ XMFLOAT3(-2.0f, 1.5f, +1.0f), (const float*)&Colors::Green },
		{ XMFLOAT3(-1.0f, -1.2f, +1.0f), (const float*)&Colors::Blue },
		{ XMFLOAT3(0.0f, +1.5f, +1.0f), (const float*)&Colors::Yellow },
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), (const float*)&Colors::Cyan },
		{ XMFLOAT3(+2.0f, 3.0f, +1.0f), (const float*)&Colors::Magenta },
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

	InitializeModelData(vertices, sizeof(Vertex::PCVertex), _countof(vertices), indices, _countof(indices));
}
