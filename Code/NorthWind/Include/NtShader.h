
#pragma once

namespace nt { namespace renderer {

// forward declare
class NtD3DRenderer;

class NtShader
{
public:
    NtShader();

    virtual ~NtShader();

	virtual bool InitializeFx(const ntWchar* fx);

    virtual void Release();

    ID3DX11EffectVariable* GetEffectVariable(ntChar* effectName);

    const ID3DX11EffectMatrixVariable* GetWorldViewProj();

    const ntChar* GetVSModel() const;

    const ntChar* GetPSModel() const;

    const ntChar* GetFXModel() const;

	static void SetVShaderModel(VShaderModel vs);

	static void SetPShaderModel(PShaderModel ps);

public:
    inline void SetWorldViewProj(CXMMATRIX m)
    {
        m_fxWorldViewProj->SetMatrix(reinterpret_cast<const float*>(&m));
    }

private:
    NtShader(const NtShader& shader);
    NtShader& operator=(const NtShader& shader);

public:
	static VShaderModel ms_vsModel;
	static PShaderModel ms_psModel;
	static FxShaderModel ms_fxModel;

protected:
    ID3D11VertexShader*	m_vertexShader;
    ID3D11PixelShader*	m_pixelShader;
    ID3DX11Effect*      m_fx;
    ID3DX11EffectMatrixVariable* m_fxWorldViewProj;
};


} }
