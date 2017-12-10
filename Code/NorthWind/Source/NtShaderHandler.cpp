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

    if (ColorShader->InitializeFx(L"../bin/simple_fx.fxo") == false)
    {
        return false;
    }

    if (LightShader->InitializeFx(L"../bin/light.fxo") == false)
    {
        return false;
    }

    return true;
}

void NtShaderHandler::Release()
{

}

} }
