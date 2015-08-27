
#pragma once

namespace nt { namespace renderer {

// forward declare
class NtD3DRenderer;

class NtShader
{
public:
	virtual ~NtShader() {}

	virtual bool Initialize(const ntWchar* vs, const ntWchar* ps) = 0;

	const ntChar* GetVSModel();
	const ntChar* GetPSModel();

	static void SetVSShaderModel(eVSShaderModel vs);
	static void SetPSShaderModel(ePSShaderModel ps);

	static eVSShaderModel m_vsModel;
	static ePSShaderModel m_psModel;
};


}
}
