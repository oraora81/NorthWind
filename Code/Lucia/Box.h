#pragma once

#include "NtModel.h"

class Box : public nt::renderer::NtModel
{
public:
	Box();
	virtual ~Box();

    virtual void Render(XMMATRIX& worldViewProj) override;

	void MakeGeometry();
    
private:
    void MakeNormal();

    void MakeGeometryTwoVertexBuf();

    void MakeColor();

    void RenderNormal(XMMATRIX& worldViewProj);

    void RenderTwoVertexBuf(XMMATRIX& worldViewProj);

    void RenderBoxPyramid(XMMATRIX& worldViewProj);

    void Render32BitColor(XMMATRIX& worldViewProj);

private:
    ID3D11Buffer* m_CB;
    ID3D11Buffer* m_buffers[2];
    int m_colorCount;
    XMFLOAT4X4 m_boxWorld;
    XMFLOAT4X4 m_pyramidWorld;

    UINT m_boxVertexOffset;
    UINT m_boxIndexOffset;
    UINT m_pyramidVertexOffset;
    UINT m_pyramidIndexOffset;
    UINT m_boxVertexCount;
    UINT m_pyramidVertexCount;
    UINT m_boxIndexCount;
    UINT m_pyramidIndexCount;           
};
