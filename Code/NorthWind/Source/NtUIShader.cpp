#include "NtCoreLib.h"
#pragma hdrstop

#include "NtShader.h"
#include "NtUIShader.h"

namespace nt { namespace renderer {

NtUIShader::NtUIShader()
    : NtShader()
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

    return true;
}

void NtUIShader::Release()
{
    NtShader::Release();
}

} }
