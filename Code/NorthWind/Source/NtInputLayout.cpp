#include "NtCoreLib.h"
#pragma hdrstop

#include "NtInputLayout.h"

namespace nt {

const D3D11_INPUT_ELEMENT_DESC NtInputLayout::PCLayout[2] = 
{
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

const D3D11_INPUT_ELEMENT_DESC NtInputLayout::PNLayout[2] =
{
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

// etc
// 2 input slot
const D3D11_INPUT_ELEMENT_DESC NtInputLayout::PCLayoutEachSlot = 
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC NtInputLayout::PCLayout32bitColor =
{
    { "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};




bool NtInputLayoutHandler::Initialize()
{
    
    D3DX11_PASS_DESC passDesc;

    // pos + color
    NtShaderHandler::ColorShader->ColorTech->GetPassByIndex(0)->GetDesc(&passDesc);
    HRF(g_renderer->Device()->CreateInputLayout(NtInputLayout::PCLayout, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &PCLayout));

    // pos + normal
    NtShaderHandler::LightShader->Light1Tech()->GetPassByIndex(0)->GetDesc(&passDesc);
    HRF(g_renderer->Device()->CreateInputLayout(NtInputLayout::PNLayout, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &PNLayout));

    return true;
}
    
void NtInputLayoutHandler::Release()
{
    Safe_Release(PNLayout);
}

}
 