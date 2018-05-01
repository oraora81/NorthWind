#include "NtCoreLib.h"
#pragma hdrstop

#include "NtRenderStateHandler.h"

namespace nt { namespace renderer {

ID3D11RasterizerState* NtRenderStateHandler::RSSolid;
ID3D11RasterizerState* NtRenderStateHandler::RSWireFrame;
ID3D11RasterizerState* NtRenderStateHandler::RSNoCull;
ID3D11BlendState* NtRenderStateHandler::BSAlpha2Coverage;
ID3D11BlendState* NtRenderStateHandler::BSTransparent;

bool NtRenderStateHandler::Initialize(ID3D11Device* device)
{
    NtAsserte(device != nullptr);

    // solid
    D3D11_RASTERIZER_DESC solidDesc;
    Crt::MemSet(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));

    solidDesc.AntialiasedLineEnable = FALSE;
    solidDesc.CullMode = D3D11_CULL_BACK;
    solidDesc.DepthBias = 0;
    solidDesc.DepthBiasClamp = 0.0f;
    solidDesc.DepthClipEnable = TRUE;
    solidDesc.FillMode = D3D11_FILL_SOLID;
    solidDesc.FrontCounterClockwise = FALSE;
    solidDesc.MultisampleEnable = FALSE;
    solidDesc.ScissorEnable = FALSE;
    solidDesc.SlopeScaledDepthBias = 0.0f;

    //
    HRF(device->CreateRasterizerState(&solidDesc, &RSSolid));

    // wireframe
    D3D11_RASTERIZER_DESC wireframeDesc;
    Crt::MemSet(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
    wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
    wireframeDesc.CullMode = D3D11_CULL_BACK;
    wireframeDesc.FrontCounterClockwise = FALSE;
    wireframeDesc.DepthClipEnable = TRUE;

    HRF(device->CreateRasterizerState(&wireframeDesc, &RSWireFrame));
    

    // no cull
    D3D11_RASTERIZER_DESC noCullDesc;
    Crt::MemSet(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
    noCullDesc.FillMode = D3D11_FILL_SOLID;
    noCullDesc.CullMode = D3D11_CULL_NONE;
    noCullDesc.FrontCounterClockwise = FALSE;
    noCullDesc.DepthClipEnable = TRUE;

    HRF(device->CreateRasterizerState(&noCullDesc, &RSNoCull));

    // alpha2coverage
    D3D11_BLEND_DESC alpha2CoverageDesc = { 0, };
    alpha2CoverageDesc.AlphaToCoverageEnable = TRUE;
    alpha2CoverageDesc.IndependentBlendEnable = FALSE;
    alpha2CoverageDesc.RenderTarget[0].BlendEnable = FALSE;
    alpha2CoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    HRF(device->CreateBlendState(&alpha2CoverageDesc, &BSAlpha2Coverage));

    // transparent
    D3D11_BLEND_DESC transparentDesc = { 0, };
    transparentDesc.AlphaToCoverageEnable = FALSE;
    transparentDesc.IndependentBlendEnable = FALSE;

    transparentDesc.RenderTarget[0].BlendEnable = TRUE;
    transparentDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    transparentDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    transparentDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

    transparentDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    transparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    transparentDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    HRF(device->CreateBlendState(&transparentDesc, &BSTransparent));

    return true;
}

void NtRenderStateHandler::Release()
{
    SAFE_RELEASE(RSWireFrame);
    SAFE_RELEASE(RSNoCull);
    SAFE_RELEASE(BSAlpha2Coverage);
    SAFE_RELEASE(BSTransparent);
}

} }
