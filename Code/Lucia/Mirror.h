#pragma once

#include "NtModel.h"

using namespace nt::renderer;

class Mirror : public nt::renderer::NtModel
{
public:
    Mirror();

    virtual ~Mirror();

    virtual void Update(float deltaTime) override;

    virtual void Render(XMMATRIX& worldViewProj) override;

    void MakeGeometry();

private:
    void BuildRoom();

    void BuildSkull();

private:
    ID3D11Buffer* m_skullVB;
    ID3D11Buffer* m_skullIB;

    ID3D11ShaderResourceView* m_floorDiffuseMapSRV;
    ID3D11ShaderResourceView* m_wallDiffuseMapSRV;
    ID3D11ShaderResourceView* m_mirrorDiffuseMapSRV;

    DirectionalLight m_dirLights[3];
    Material m_roomMat;
    Material m_skullMat;
    Material m_mirrorMat;
    Material m_shadowMat;

    XMFLOAT4X4 m_roomWorld;
    XMFLOAT4X4 m_skullWorld;
};
