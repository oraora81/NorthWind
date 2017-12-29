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
    inline void SetWorld(CXMMATRIX m)
    {
        m_fxWorld->SetMatrix(reinterpret_cast<const float*>(&m));
    }

    //inline void SetEyePosW(XMFLOAT3 eyePosW)
    //{

    //}

    //inline void SetDirLight(const DirectionalLight& light);

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
