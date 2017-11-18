
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

    ID3D11InputLayout* GetInputLayout();

    const ID3DX11EffectTechnique* GetEffectTechnique();

    const ID3DX11EffectMatrixVariable* GetEffectMatrix();

	static void SetVShaderModel(eVShaderModel vs);

	static void SetPShaderModel(ePShaderModel ps);

public:
	static eVShaderModel ms_vsModel;
	static ePShaderModel ms_psModel;
	static eFxShaderModel ms_fxModel;

protected:
    ID3D11VertexShader*	m_vertexShader;
    ID3D11PixelShader*	m_pixelShader;
    ID3D11InputLayout*	m_layout;
    ID3DX11Effect*      m_fx;
    ID3DX11EffectTechnique* m_tech;
    ID3DX11EffectMatrixVariable* m_fxWorldViewProj;
};


} }
