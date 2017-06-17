
#pragma once

namespace nt { namespace renderer {

// forward declare
class NtD3DRenderer;

class NtShader
{
public:
	virtual ~NtShader() {}

	virtual bool Initialize(const ntWchar* vs, const ntWchar* ps) = 0;

	virtual bool InitializeFx(const ntWchar* fx);

	const ntChar* GetVSModel() const;

	const ntChar* GetPSModel() const;

	const ntChar* GetFXModel() const;

	static void SetVShaderModel(eVShaderModel vs);

	static void SetPShaderModel(ePShaderModel ps);

public:
	static eVShaderModel ms_vsModel;
	static ePShaderModel ms_psModel;
	static eFxShaderModel ms_fxModel;
};


}
}
