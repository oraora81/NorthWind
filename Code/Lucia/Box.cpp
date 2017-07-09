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

	g_renderer->ViewMatrix(v);
}
