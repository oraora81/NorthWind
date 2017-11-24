#pragma once

#include "BaseShape.h"

class Shapes : public BaseShape
{
public:
    Shapes();
    virtual ~Shapes();

    virtual void Render(XMMATRIX& worldViewProj) override;

    void MakeGeometry();

private:
    XMFLOAT4X4 m_gridWorld;
    XMFLOAT4X4 m_boxWorld;
    XMFLOAT4X4 m_sphereWorld[10];
    XMFLOAT4X4 m_cylWorld[10];
    XMFLOAT4X4 m_centerSphere;
    XMFLOAT4X4 m_view;
    XMFLOAT4X4 m_proj;

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
