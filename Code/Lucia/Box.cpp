#include "stdafx.h"
#pragma hdrstop

#include "Box.h"

using namespace nt;

Box::Box()
: m_theta(1.5f * NtMath<float>::PI)
, m_phi(0.25f * NtMath<float>::PI)
, m_radius(5.0f)
{

}


Box::~Box()
{

}

void Box::MakeGeometry()
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
