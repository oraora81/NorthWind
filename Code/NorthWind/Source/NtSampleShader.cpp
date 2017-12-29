#include "NtCoreLib.h"
#pragma hdrstop

#include "NtSampleShader.h"

namespace nt { namespace renderer { 

NtSampleShader::NtSampleShader()
    : NtShader()
    , m_lightTech(nullptr)
    , m_fxWorld(nullptr)
    , m_fxWorldInvTranspose(nullptr)
    , m_fxEyePosW(nullptr)
    , m_fxDirLight(nullptr)
    , m_fxPointLight(nullptr)
    , m_fxSpotLight(nullptr)
    , m_fxMaterial(nullptr)
{

}

NtSampleShader::~NtSampleShader()
{
    Release();
}

bool NtSampleShader::InitializeFx(const ntWchar* fx)
{
    NtAsserte(fx != nullptr);

    if (NtShader::InitializeFx(fx) == false)
    {
        return false;
    }

    m_lightTech = m_fx->GetTechniqueByName("LightTech");
    m_fxWorldViewProj = m_fx->GetVariableByName("gWorldViewProj")->AsMatrix();
    m_fxWorld = m_fx->GetVariableByName("gWorld")->AsMatrix();
    m_fxWorldInvTranspose = m_fx->GetVariableByName("gWorldInvTranspose")->AsMatrix();
    m_fxEyePosW = m_fx->GetVariableByName("gEyePosW")->AsVector();
    m_fxDirLight = m_fx->GetVariableByName("gDirLight");
    m_fxPointLight = m_fx->GetVariableByName("gPointLight");
    m_fxSpotLight = m_fx->GetVariableByName("gSpotLight");
    m_fxMaterial = m_fx->GetVariableByName("gMaterial");

    NtAsserte(m_lightTech != nullptr);
    NtAsserte(m_fxWorldViewProj != nullptr);
    NtAsserte(m_fxWorld != nullptr);
    NtAsserte(m_fxWorldInvTranspose != nullptr);
    NtAsserte(m_fxEyePosW != nullptr);
    NtAsserte(m_fxDirLight != nullptr);
    NtAsserte(m_fxPointLight != nullptr);
    NtAsserte(m_fxSpotLight != nullptr);
    NtAsserte(m_fxMaterial != nullptr);

    return true;
}

void NtSampleShader::Release()
{
    SAFE_RELEASE(m_lightTech);
    SAFE_RELEASE(m_fxWorldViewProj);
    SAFE_RELEASE(m_fxWorld);
    SAFE_RELEASE(m_fxWorldInvTranspose);
    SAFE_RELEASE(m_fxEyePosW);
    SAFE_RELEASE(m_fxDirLight);
    SAFE_RELEASE(m_fxPointLight);
    SAFE_RELEASE(m_fxSpotLight);
    SAFE_RELEASE(m_fxMaterial);
}

}
}
