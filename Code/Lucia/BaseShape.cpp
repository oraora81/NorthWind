#include "stdafx.h"
#pragma hdrstop

#include "BaseShape.h"

BaseShape::BaseShape()
    : m_theta(1.5f * nt::NtMath<float>::PI)
    , m_phi(0.25f * nt::NtMath<float>::PI)
    , m_radius(5.0f)
{

}

BaseShape::~BaseShape()
{

}

void BaseShape::Update(float deltaTime)
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