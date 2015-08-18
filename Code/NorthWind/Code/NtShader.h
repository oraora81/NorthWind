
#pragma once

namespace NT
{
namespace RENDERER
{

// forward declare
class NtD3DRenderer;

class NtShader
{
public:
	virtual ~NtShader() {}

	virtual bool Initialize(const NtWChar* vs, const NtWChar* ps) = 0;

	const NtChar* GetVSModel();
	const NtChar* GetPSModel();

	static void SetVSShaderModel(eVSShaderModel vs);
	static void SetPSShaderModel(ePSShaderModel ps);

	static eVSShaderModel m_vsModel;
	static ePSShaderModel m_psModel;
};


}
}
