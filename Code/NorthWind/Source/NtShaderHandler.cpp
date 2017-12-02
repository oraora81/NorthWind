#include "NtCoreLib.h"
#pragma hdrstop

#include "NtShaderHandler.h"

namespace nt { namespace renderer { 

// static implement
std::unique_ptr<NtLightShader> NtShaderHandler::LightShader;

//
bool NtShaderHandler::Initialize()
{
    LightShader = std::make_unique<NtLightShader>();
}

void NtShaderHandler::Release()
{

}

} }
