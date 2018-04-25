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

private:
    ID3DX11EffectTechnique* m_uiTech;
};

}
}
