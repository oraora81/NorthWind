
#include "NtCoreLib.h"
#pragma hdrstop

namespace nt { namespace renderer {

eVShaderModel NtShader::ms_vsModel = eVShaderModel::NT_MAX_VSHADER_MODEL;
ePShaderModel NtShader::ms_psModel = ePShaderModel::NT_MAX_PSHADER_MODEL;
eFxShaderModel NtShader::ms_fxModel = eFxShaderModel::NT_MAX_FXSHADER_MODEL;

namespace
{

static ntChar* g_vsShaderModel[eVShaderModel::NT_MAX_VSHADER_MODEL] = 
{
	"vs_1_1",
	"vs_2_0",
	"vs_3_0",
	"vs_4_0",
	"vs_5_0",
};


static ntChar* g_psShaderModel[ePShaderModel::NT_MAX_PSHADER_MODEL] = 
{
	"ps_1_1",
	"ps_2_0",
	"ps_3_0",
	"ps_4_0",
	"ps_5_0",
};

static ntChar* g_fxShaderModel[eFxShaderModel::NT_MAX_FXSHADER_MODEL] = 
{
	"fx_2_0",
	"fx_4_0",
	"fx_4_1",
	"fx_5_0",
};

}

bool NtShader::InitializeFx(const ntWchar* fx)
{
	return true;
}

const ntChar* NtShader::GetVSModel() const
{
	return g_vsShaderModel[NtShader::ms_vsModel];
}

const ntChar* NtShader::GetPSModel() const
{
	return g_psShaderModel[NtShader::ms_psModel];
}

const ntChar* NtShader::GetFXModel() const
{
	return g_fxShaderModel[NtShader::ms_fxModel];
}

void NtShader::SetVShaderModel(eVShaderModel vs)
{
	ms_vsModel = vs;
}

void NtShader::SetPShaderModel(ePShaderModel ps)
{
	ms_psModel = ps;
}


}
}	// namespace nt
