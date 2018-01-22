#pragma once

#include "NtModel.h"

class Crate : public nt::renderer::NtModel
{
public:
    Crate();

    virtual ~Crate() override;

    virtual void Update(float deltaTime) override;

    virtual void Render(XMMATRIX& worldViewProj) override;

    void MakeGeometry();

private:
    ID3D11ShaderResourceView* m_diffuseMapSRV;
    //ID3D11ShaderResourceView* m_AddSRV;

    nt::DirectionalLight m_dirLights[3];
    nt::Material m_boxMat;

    XMFLOAT4X4 m_boxWorld;
    XMFLOAT4X4 m_texTransform;

    XMFLOAT4X4 m_viewProj;

    UINT m_boxVertexOffset;
    UINT m_boxIndexOffset;
};
