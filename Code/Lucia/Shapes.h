#pragma once

#include "NtModel.h"

class Shapes : public nt::renderer::NtModel
{
public:
    Shapes();
    virtual ~Shapes();

    virtual void Update(float deltaTime) override;

    void MakeGeometry();

    float& Theta()
    {
        return m_theta;
    }

    float& Phi()
    {
        return m_phi;
    }

    void Phi(float p)
    {
        m_phi = p;
    }

    float Radius()
    {
        return m_radius;
    }

    void Radius(float r)
    {
        m_radius = r;
    }

private:
    float m_theta;
    float m_phi;
    float m_radius;

    int m_boxVertexOffset;
    int m_gridVertexOffset;
    int m_sphereVertexOffset;
    int m_cylinderVertexOffset;

    UINT m_boxIndexOffset;
    UINT m_gridIndexOffset;
    UINT m_sphereIndexOffset;
    UINT m_cylinderIndexOffset;

    UINT m_boxIndexCount;
    UINT m_gridIndexCount;
    UINT m_sphereIndexCount;
    UINT m_cylinderIndexCount;
};
