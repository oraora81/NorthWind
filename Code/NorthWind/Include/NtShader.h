
#pragma once

namespace nt { namespace renderer {

// forward declare
class NtD3DRenderer;

class NtShader
{
public:
    NtShader();

    virtual ~NtShader();

	virtual bool InitializeFx(const ntWchar* fx) = 0;

    virtual void Release();

	const ntChar* GetVSModel() const;

	const ntChar* GetPSModel() const;

	const ntChar* GetFXModel() const;

    ID3DX11EffectVariable* GetEffectVariable(ntChar* effectName);

    ID3D11InputLayout* GetInputLayout();

    const ID3DX11EffectTechnique* GetEffectTechnique();

    const ID3DX11EffectMatrixVariable* GetEffectMatrix();

	static void SetVShaderModel(VShaderModel vs);

	static void SetPShaderModel(PShaderModel ps);

public:
	static VShaderModel ms_vsModel;
	static PShaderModel ms_psModel;
	static FxShaderModel ms_fxModel;

protected:
    ID3D11VertexShader*	m_vertexShader;
    ID3D11PixelShader*	m_pixelShader;
    ID3D11InputLayout*	m_layout;
    ID3DX11Effect*      m_fx;
    ID3DX11EffectTechnique* m_tech;
    ID3DX11EffectMatrixVariable* m_fxWorldViewProj;
};


} }
