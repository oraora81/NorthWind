
#include "NtCoreLib.h"
#pragma hdrstop

namespace NT
{
namespace RENDERER
{

eVSShaderModel NtShader::m_vsModel = eVSShaderModel::NT_MAX_VSSHADER_MODEL;
ePSShaderModel NtShader::m_psModel = ePSShaderModel::NT_MAX_PSSHADER_MODEL;

namespace
{

static NtChar* g_vsShaderModel[eVSShaderModel::NT_MAX_VSSHADER_MODEL] = 
{
	"vs_1_1",
	"vs_2_0",
	"vs_3_0",
	"vs_4_0",
	"vs_5_0",
};


static NtChar* g_psShaderModel[ePSShaderModel::NT_MAX_PSSHADER_MODEL] = 
{
	"ps_1_1",
	"ps_2_0",
	"ps_3_0",
	"ps_4_0",
	"ps_5_0",
};


}


const NtChar* NtShader::GetVSModel()
{
	return g_vsShaderModel[NtShader::m_vsModel];
}

const NtChar* NtShader::GetPSModel()
{
	return g_psShaderModel[NtShader::m_psModel];
}


void NtShader::SetVSShaderModel(eVSShaderModel vs)
{
	m_vsModel = vs;
}

void NtShader::SetPSShaderModel(ePSShaderModel ps)
{
	m_psModel = ps;
}


}
}	// namespace NT
