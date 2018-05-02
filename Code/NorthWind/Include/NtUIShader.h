#pragma once

namespace nt { namespace renderer {

class NtUIShader : public NtShader
{
public:
    NtUIShader();

    virtual ~NtUIShader() override;

    bool InitializeFx(const ntWchar* fx) override;

    void Release() override;

    inline ID3DX11EffectTechnique* UITech() const
    {
        return m_uiTech;
    }

    inline void SetShaderResourceMap(ID3D11ShaderResourceView* view)
    {
        NtAsserte(view != nullptr);

        m_srMap->SetResource(view);
    }

private:
    ID3DX11EffectTechnique* m_uiTech;
    ID3DX11EffectShaderResourceVariable* m_srMap;
};

}
}
