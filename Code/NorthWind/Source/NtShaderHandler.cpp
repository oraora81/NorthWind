#include "NtCoreLib.h"
#pragma hdrstop

#include "NtShaderHandler.h"
#include "NtLightShader.h"
#include "NtColorShader.h"

namespace nt { namespace renderer { 

// static implement
std::unique_ptr<NtLightShader> NtShaderHandler::LightShader;
std::unique_ptr<NtColorShader> NtShaderHandler::ColorShader;

//
bool NtShaderHandler::Initialize()
{
    LightShader = std::make_unique<NtLightShader>();
    ColorShader = std::make_unique<NtColorShader>();

    const ntWchar* filePath = g_resMgr->GetPath(L"simple_fx.fxo");
    if (ColorShader->InitializeFx(filePath) == false)
    {
        return false;
    }

    filePath = g_resMgr->GetPath(L"basic.fxo");
    if (LightShader->InitializeFx(filePath) == false)
    {
        return false;
    }

    return true;
}

void NtShaderHandler::Release()
{

}

} }
