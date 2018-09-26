#include "NtCoreLib.h"
#pragma hdrstop

#include "NtInputLayout.h"
#include "NtShaderHandler.h"
#include "NtColorShader.h"
#include "NtLightShader.h"
#include "NtUIShader.h"

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

    const D3D11_INPUT_ELEMENT_DESC NtInputLayout::PNULayout[3] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    const D3D11_INPUT_ELEMENT_DESC NtInputLayout::SpriteLayout[3] =
    {
        { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };


    // etc
    // 2 input slot
    const D3D11_INPUT_ELEMENT_DESC NtInputLayout::PCLayoutEachSlot[2] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    const D3D11_INPUT_ELEMENT_DESC NtInputLayout::PCLayout32bitColor[2] =
    {
        { "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    const D3D11_INPUT_ELEMENT_DESC NtInputLayout::PSLayout[2] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

// implement static variable
ID3D11InputLayout* NtInputLayoutHandler::PCInputLayout;
ID3D11InputLayout* NtInputLayoutHandler::PNInputLayout;
ID3D11InputLayout* NtInputLayoutHandler::PNUInputLayout;
ID3D11InputLayout* NtInputLayoutHandler::SpriteInputLayout;
ID3D11InputLayout* NtInputLayoutHandler::PCInputLayoutEachSlot;
ID3D11InputLayout* NtInputLayoutHandler::PCInputLayout32bitColor;
ID3D11InputLayout* NtInputLayoutHandler::PSInputLayout;

bool NtInputLayoutHandler::Initialize()
{
    
    D3DX11_PASS_DESC passDesc;

    auto device = g_renderer->Device();
    NtAsserte(device != nullptr);

    // pos + color
    NtShaderHandler::ColorShader->ColorTech->GetPassByIndex(0)->GetDesc(&passDesc);
    HRF(device->CreateInputLayout(
        NtInputLayout::PCLayout, 
        _countof(NtInputLayout::PCLayout),
        passDesc.pIAInputSignature, 
        passDesc.IAInputSignatureSize, 
        &PCInputLayout));

    // pos + normal
    //NtShaderHandler::LightShader->Light1Tech()->GetPassByIndex(0)->GetDesc(&passDesc);
    //HRF(device->CreateInputLayout(
    //    NtInputLayout::PNLayout, 
    //    2, 
    //    passDesc.pIAInputSignature, 
    //    passDesc.IAInputSignatureSize, 
    //    &PNInputLayout));

    // pos + normal + uv
    NtShaderHandler::LightShader->Light1Tech()->GetPassByIndex(0)->GetDesc(&passDesc);
    HRF(device->CreateInputLayout(NtInputLayout::PNULayout,
        _countof(NtInputLayout::PNULayout),
        passDesc.pIAInputSignature,
        passDesc.IAInputSignatureSize,
        &PNUInputLayout));

    NtShaderHandler::UIShader->UITech()->GetPassByIndex(0)->GetDesc(&passDesc);
    HRF(device->CreateInputLayout(NtInputLayout::SpriteLayout,
        _countof(NtInputLayout::SpriteLayout),
        passDesc.pIAInputSignature,
        passDesc.IAInputSignatureSize,
        &SpriteInputLayout));

    //NtShaderHandler::LightShader->
    
    return true;
}
    
void NtInputLayoutHandler::Release()
{
    SAFE_RELEASE(PCInputLayout);
    //SAFE_RELEASE(PNInputLayout);
    SAFE_RELEASE(PNUInputLayout);
    SAFE_RELEASE(SpriteInputLayout);
}

}
 