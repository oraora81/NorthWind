#include "NtCoreLib.h"
#pragma hdrstop

#include "NtShader.h"
#include "NtUIShader.h"

namespace nt { namespace renderer {

NtUIShader::NtUIShader()
    : NtShader()
    , m_uiTech(nullptr)
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
    NtAsserte(m_uiTech != nullptr);

    return true;
}

void NtUIShader::Release()
{
    NtShader::Release();
}

} }
