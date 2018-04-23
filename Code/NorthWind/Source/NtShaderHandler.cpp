#include "NtCoreLib.h"
#pragma hdrstop

#include "NtShaderHandler.h"
#include "NtLightShader.h"
#include "NtColorShader.h"
#include "NtUIShader.h"
#include "NtSampleShader.h"

namespace nt { namespace renderer { 

// static implement
std::unique_ptr<NtLightShader> NtShaderHandler::LightShader;
std::unique_ptr<NtColorShader> NtShaderHandler::ColorShader;
std::unique_ptr<NtUIShader>     NtShaderHandler::UIShader;
std::unique_ptr<NtSampleShader> NtShaderHandler::SampleLightShader;

//
bool NtShaderHandler::Initialize()
{
    LightShader = std::make_unique<NtLightShader>();
    ColorShader = std::make_unique<NtColorShader>();
    UIShader = std::make_unique<NtUIShader>();
    SampleLightShader = std::make_unique<NtSampleShader>();

    const ntWchar* filePath = g_resMgr->GetPath(L"simple_fx.fxo");
    if (ColorShader->InitializeFx(filePath) == false)
    {
        NtAsserte("ColorShader initialize failed" && false);
        return false;
    }

    filePath = g_resMgr->GetPath(L"basic.fxo");
    if (LightShader->InitializeFx(filePath) == false)
    {
        NtAsserte("LightShader initialize failed" && false);
        return false;
    }

    filePath = g_resMgr->GetPath(L"ui.fxo");
    if (UIShader->InitializeFx(filePath) == false)
    {
        NtAsserte("UIShader initialize failed" && false);
        return false;
    }

    filePath = g_resMgr->GetPath(L"light.fxo");
    if (SampleLightShader->InitializeFx(filePath) == false)
    {
        NtAsserte("SampleLightShader initialize failed" && false);
        return false;
    }
    
    return true;
}

void NtShaderHandler::Release()
{

}

} }
