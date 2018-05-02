#include "NtCoreLib.h"
#pragma hdrstop

#include "NtShader.h"
#include "NtUIShader.h"

namespace nt { namespace renderer {

NtUIShader::NtUIShader()
    : NtShader()
    , m_uiTech(nullptr)
    , m_srMap(nullptr)
{
}

NtUIShader::~NtUIShader()
{
    Release();
}

bool NtUIShader::InitializeFx(const ntWchar* fx)
{
    NtAsserte(fx != nullptr);

    if (NtShader::InitializeFx(fx) == false)
    {
        return false;
    }

    m_uiTech = m_fx->GetTechniqueByName("RenderUI");
    m_srMap = m_fx->GetVariableByName("g_Texture")->AsShaderResource();

    NtAsserte(m_uiTech != nullptr);
    NtAsserte(m_srMap != nullptr);

    return true;
}

void NtUIShader::Release()
{
    SAFE_RELEASE(m_srMap);
    SAFE_RELEASE(m_uiTech);

    NtShader::Release();
}

} }
