#pragma once

namespace nt { namespace renderer { 

class NtSampleShader : public NtShader
{
    struct NtMatrixBuffer
    {
        XMMATRIX world;
        XMMATRIX view;
        XMMATRIX proj;
    };

public:
    NtSampleShader();

    virtual ~NtSampleShader() override;

    bool InitializeFx(const ntWchar* fx) override;

    void Release() override;

public:
    inline ID3DX11EffectTechnique* LightTech() const
    {
        return m_lightTech;
    }
    inline void SetWorld(CXMMATRIX m)
    {
        m_fxWorld->SetMatrix(reinterpret_cast<const float*>(&m));
    }
    inline void SetWorldInvTranspose(CXMMATRIX m)
    {
        m_fxWorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&m));
    }
    inline void SetEyePosW(XMFLOAT3 eyePosW)
    {
        m_fxEyePosW->SetRawValue(&eyePosW, 0, sizeof(XMFLOAT3));
    }
    inline void SetDirLight(const DirectionalLight* light)
    {
        m_fxDirLight->SetRawValue(light, 0, sizeof(DirectionalLight));
    }
    inline void SetPointLight(const PointLight* light)
    {
        m_fxPointLight->SetRawValue(light, 0, sizeof(PointLight));
    }
    inline void SetSpotLight(const SpotLight* light)
    {
        m_fxSpotLight->SetRawValue(light, 0, sizeof(SpotLight));
    }
    inline void SetMaterial(const Material& mat)
    {
        m_fxMaterial->SetRawValue(&mat, 0, sizeof(Material));
    }
    inline const ID3DX11EffectMatrixVariable* FxWorld() const
    {
        return m_fxWorld;
    }
    inline const ID3DX11EffectMatrixVariable* FxWorldInvTranspose() const
    {
        return m_fxWorldInvTranspose;
    }

private:
    ID3DX11EffectTechnique* m_lightTech;
    ID3DX11EffectMatrixVariable* m_fxWorld;
    ID3DX11EffectMatrixVariable* m_fxWorldInvTranspose;
    ID3DX11EffectVectorVariable* m_fxEyePosW;
    ID3DX11EffectVariable* m_fxDirLight;
    ID3DX11EffectVariable* m_fxPointLight;
    ID3DX11EffectVariable* m_fxSpotLight;
    ID3DX11EffectVariable* m_fxMaterial;
};

} }
