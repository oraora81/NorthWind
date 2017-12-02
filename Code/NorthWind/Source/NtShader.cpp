
#include "NtCoreLib.h"
#pragma hdrstop

namespace nt { namespace renderer {

VShaderModel NtShader::ms_vsModel = VShaderModel::NT_MAX_VSHADER_MODEL;
PShaderModel NtShader::ms_psModel = PShaderModel::NT_MAX_PSHADER_MODEL;
FxShaderModel NtShader::ms_fxModel = FxShaderModel::NT_MAX_FXSHADER_MODEL;

namespace
{

static ntChar* g_vsShaderModel[VShaderModel::NT_MAX_VSHADER_MODEL] = 
{
	"vs_1_1",
	"vs_2_0",
	"vs_3_0",
	"vs_4_0",
	"vs_5_0",
};


static ntChar* g_psShaderModel[PShaderModel::NT_MAX_PSHADER_MODEL] = 
{
	"ps_1_1",
	"ps_2_0",
	"ps_3_0",
	"ps_4_0",
	"ps_5_0",
};

static ntChar* g_fxShaderModel[FxShaderModel::NT_MAX_FXSHADER_MODEL] = 
{
	"fx_2_0",
	"fx_4_0",
	"fx_4_1",
	"fx_5_0",
};

}


NtShader::NtShader()
    : m_vertexShader(nullptr)
    , m_pixelShader(nullptr)
    , m_layout(nullptr)
    , m_fx(nullptr)
    , m_tech(nullptr)
    , m_fxWorldViewProj(nullptr)
{

}

NtShader::~NtShader()
{
    Release();
}

bool NtShader::InitializeFx(const ntWchar* fx)
{

}

void NtShader::Release()
{
    SAFE_RELEASE(m_fxWorldViewProj);
    SAFE_RELEASE(m_tech);
    SAFE_RELEASE(m_fx);
    SAFE_RELEASE(m_layout);
    SAFE_RELEASE(m_pixelShader);
    SAFE_RELEASE(m_vertexShader);
}

ID3DX11EffectVariable* NtShader::GetEffectVariable(ntChar* effectName)
{
    return m_fx->GetVariableByName(effectName);
}

void NtShader::SetVShaderModel(VShaderModel vs)
{
	ms_vsModel = vs;
}

void NtShader::SetPShaderModel(PShaderModel ps)
{
	ms_psModel = ps;
}

ID3D11InputLayout* NtShader::GetInputLayout()
{
    NtAsserte(m_layout);

    return m_layout;
}
//
//const ID3DX11EffectTechnique* NtShader::GetEffectTechnique()
//{
//    NtAsserte(m_tech != nullptr);
//
//    return m_tech;
//}

const ID3DX11EffectMatrixVariable* NtShader::GetEffectMatrix()
{
    NtAsserte(m_fxWorldViewProj != nullptr);

    return m_fxWorldViewProj;
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
} }	// namespace nt
