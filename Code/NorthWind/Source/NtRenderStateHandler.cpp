#include "NtCoreLib.h"
#pragma hdrstop

#include "NtRenderStateHandler.h"

namespace nt { namespace renderer {


ID3D11RasterizerState* NtRenderStateHandler::RSSolid;
ID3D11RasterizerState* NtRenderStateHandler::RSWireFrame;
ID3D11RasterizerState* NtRenderStateHandler::RSNoCull;
ID3D11RasterizerState* NtRenderStateHandler::RSCullClockwise;
ID3D11RasterizerState* NtRenderStateHandler::RSUI;

ID3D11BlendState* NtRenderStateHandler::BSAlpha2Coverage;
ID3D11BlendState* NtRenderStateHandler::BSTransparent;
ID3D11BlendState* NtRenderStateHandler::BSNoRenderTargetWrite;
ID3D11BlendState* NtRenderStateHandler::BSUI;
ID3D11BlendState* NtRenderStateHandler::BSAdd;

ID3D11DepthStencilState* NtRenderStateHandler::DSMarkMirror;
ID3D11DepthStencilState* NtRenderStateHandler::DSDrawReflection;
ID3D11DepthStencilState* NtRenderStateHandler::DSNoDoubleBlend;
ID3D11DepthStencilState* NtRenderStateHandler::DSNoneDepth;
ID3D11DepthStencilState* NtRenderStateHandler::DSUI;
ID3D11DepthStencilState* NtRenderStateHandler::DSOverdrawTest;

ID3D11SamplerState* NtRenderStateHandler::SSUI;

std::tuple<ntUchar, XMVECTORF32> NtRenderStateHandler::RANGE0;
std::tuple<ntUchar, XMVECTORF32> NtRenderStateHandler::RANGE1;
std::tuple<ntUchar, XMVECTORF32> NtRenderStateHandler::RANGE2;
std::tuple<ntUchar, XMVECTORF32> NtRenderStateHandler::RANGE3;
std::tuple<ntUchar, XMVECTORF32> NtRenderStateHandler::RANGE4;
std::tuple<ntUchar, XMVECTORF32> NtRenderStateHandler::RANGE5;

bool NtRenderStateHandler::Initialize(ID3D11Device* device)
{
    InitRS(device);

    InitBS(device);

    InitDS(device);

    InitOverdrawColors();

    return true;
}

void NtRenderStateHandler::Release()
{
    SAFE_RELEASE(RSSolid);
    SAFE_RELEASE(RSWireFrame);
    SAFE_RELEASE(RSNoCull);
    SAFE_RELEASE(RSCullClockwise);
    SAFE_RELEASE(RSUI);

    SAFE_RELEASE(BSAlpha2Coverage);
    SAFE_RELEASE(BSTransparent);
    SAFE_RELEASE(BSNoRenderTargetWrite);

    SAFE_RELEASE(DSMarkMirror);
    SAFE_RELEASE(DSDrawReflection);
    SAFE_RELEASE(DSNoDoubleBlend);
    SAFE_RELEASE(DSNoneDepth);
    SAFE_RELEASE(DSUI);
    SAFE_RELEASE(DSOverdrawTest);
}

bool NtRenderStateHandler::InitRS(ID3D11Device* device)
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


	// cullclockwise
	// note : define such that we still cull backfaces by making front faces CCW.
	// If we did not cull backfaces, then we have to worry about the Backface
	// property in the D3D11_DEPTH_STENCIL_DESC.
	D3D11_RASTERIZER_DESC cullClockwiseDesc;
	Crt::MemSet(&cullClockwiseDesc, sizeof(D3D11_RASTERIZER_DESC));
	cullClockwiseDesc.FillMode = D3D11_FILL_SOLID;
	cullClockwiseDesc.CullMode = D3D11_CULL_BACK;
	cullClockwiseDesc.FrontCounterClockwise = TRUE;
	cullClockwiseDesc.DepthClipEnable = TRUE;

	HRF(device->CreateRasterizerState(&cullClockwiseDesc, &RSCullClockwise));

	// ui
    D3D11_RASTERIZER_DESC uiDesc;
    Crt::MemSet(&uiDesc, sizeof(D3D11_RASTERIZER_DESC));
    uiDesc.AntialiasedLineEnable = FALSE;
    uiDesc.CullMode = D3D11_CULL_BACK;
    uiDesc.DepthClipEnable = TRUE;
    uiDesc.FillMode = D3D11_FILL_SOLID;
    uiDesc.MultisampleEnable = TRUE;
    
    HRF(device->CreateRasterizerState(&uiDesc, &RSUI));

    return true;
}

bool NtRenderStateHandler::InitBS(ID3D11Device* device)
{
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

	// NoRenderTargetWrite
	D3D11_BLEND_DESC noRenderTargetWriteDesc = { 0, };
	noRenderTargetWriteDesc.AlphaToCoverageEnable = FALSE;
	noRenderTargetWriteDesc.IndependentBlendEnable = FALSE;

	noRenderTargetWriteDesc.RenderTarget[0].BlendEnable = FALSE;
	noRenderTargetWriteDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	noRenderTargetWriteDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	noRenderTargetWriteDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	noRenderTargetWriteDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	noRenderTargetWriteDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	noRenderTargetWriteDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	noRenderTargetWriteDesc.RenderTarget[0].RenderTargetWriteMask = 0;

	HRF(device->CreateBlendState(&noRenderTargetWriteDesc, &BSNoRenderTargetWrite));

	// ui
    D3D11_BLEND_DESC uiDesc;
    Crt::MemSet(&uiDesc, sizeof(D3D11_BLEND_DESC));

    uiDesc.RenderTarget[0].BlendEnable = TRUE;
    uiDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    uiDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    uiDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    uiDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    uiDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    uiDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    uiDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

    HRF(device->CreateBlendState(&uiDesc, &BSUI));

    // add
    D3D11_BLEND_DESC addDesc;
    Crt::MemSet(&addDesc, sizeof(D3D11_BLEND));
    addDesc.AlphaToCoverageEnable = FALSE;
    addDesc.IndependentBlendEnable = FALSE;
    addDesc.RenderTarget[0].BlendEnable = TRUE;
    addDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    addDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    addDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    addDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    addDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    addDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    addDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    HRF(device->CreateBlendState(&addDesc, &BSAdd));

    return true;
}

bool NtRenderStateHandler::InitDS(ID3D11Device* device)
{
    {
        D3D11_DEPTH_STENCIL_DESC dsMarkMirror;
        dsMarkMirror.DepthEnable = TRUE;
        dsMarkMirror.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        dsMarkMirror.DepthFunc = D3D11_COMPARISON_LESS;
        dsMarkMirror.StencilEnable = TRUE;
        dsMarkMirror.StencilReadMask = 0xff;
        dsMarkMirror.StencilWriteMask = 0xff;

        dsMarkMirror.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        dsMarkMirror.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        dsMarkMirror.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
        dsMarkMirror.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        // we are not rendering backfacing polygons, so these settings do not matter
        dsMarkMirror.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        dsMarkMirror.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        dsMarkMirror.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
        dsMarkMirror.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        HRF(device->CreateDepthStencilState(&dsMarkMirror, &DSMarkMirror));
    }

    {
        D3D11_DEPTH_STENCIL_DESC drawReflectionDesc;
        drawReflectionDesc.DepthEnable = TRUE;
        drawReflectionDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        drawReflectionDesc.DepthFunc = D3D11_COMPARISON_LESS;
        drawReflectionDesc.StencilEnable = TRUE;
        drawReflectionDesc.StencilReadMask = 0xff;
        drawReflectionDesc.StencilWriteMask = 0xff;

        drawReflectionDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        drawReflectionDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        drawReflectionDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        drawReflectionDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

        // We are not rendering backfacing polygons, so these settins do not matter
        drawReflectionDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        drawReflectionDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        drawReflectionDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        drawReflectionDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

        HRF(device->CreateDepthStencilState(&drawReflectionDesc, &DSDrawReflection));
    }
	
    {
        D3D11_DEPTH_STENCIL_DESC noDoubleBlendDesc;
        noDoubleBlendDesc.DepthEnable = TRUE;
        noDoubleBlendDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        noDoubleBlendDesc.DepthFunc = D3D11_COMPARISON_LESS;
        noDoubleBlendDesc.StencilEnable = TRUE;
        noDoubleBlendDesc.StencilReadMask = 0xff;
        noDoubleBlendDesc.StencilWriteMask = 0xff;

        noDoubleBlendDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        noDoubleBlendDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        noDoubleBlendDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
        noDoubleBlendDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

        // dont care
        noDoubleBlendDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        noDoubleBlendDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        noDoubleBlendDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
        noDoubleBlendDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

        HRF(device->CreateDepthStencilState(&noDoubleBlendDesc, &DSNoDoubleBlend));
    }

    {
        D3D11_DEPTH_STENCIL_DESC noneDepthDesc;
        noneDepthDesc.DepthEnable = TRUE;
        noneDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        noneDepthDesc.DepthFunc = D3D11_COMPARISON_LESS;
        noneDepthDesc.StencilEnable = FALSE;

        HRF(device->CreateDepthStencilState(&noneDepthDesc, &DSNoneDepth));

        D3D11_DEPTH_STENCIL_DESC dsDesc;
        Crt::MemSet(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

        dsDesc.DepthEnable = FALSE;
        dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
        dsDesc.StencilEnable = FALSE;
        dsDesc.StencilReadMask = 0xff;
        dsDesc.StencilWriteMask = 0xff;

        HRF(device->CreateDepthStencilState(&dsDesc, &DSUI));
    }

    {
        D3D11_DEPTH_STENCIL_DESC overdrawDesc;
        overdrawDesc.DepthEnable = TRUE;
        overdrawDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        overdrawDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
        overdrawDesc.StencilEnable = TRUE;
        overdrawDesc.StencilReadMask = 0xff;
        overdrawDesc.StencilWriteMask = 0xff;

        overdrawDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        overdrawDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        overdrawDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
        overdrawDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        
        overdrawDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        overdrawDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        overdrawDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
        overdrawDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        HRF(device->CreateDepthStencilState(&overdrawDesc, &DSOverdrawTest));
    }

    return true;
}

bool NtRenderStateHandler::Etc(ID3D11Device* device)
{
    D3D11_SAMPLER_DESC sDesc;
    Crt::MemSet(&sDesc, sizeof(D3D11_SAMPLER_DESC));

    return true;
}

void NtRenderStateHandler::InitOverdrawColors()
{
    std::get<0>(RANGE0) = 0;
    std::get<0>(RANGE1) = 1;
    std::get<0>(RANGE2) = 2;
    std::get<0>(RANGE3) = 3;
    std::get<0>(RANGE4) = 4;
    std::get<0>(RANGE5) = 5;

    std::get<1>(RANGE0) = Colors::Red;
    std::get<1>(RANGE1) = Colors::Green;
    std::get<1>(RANGE2) = Colors::Blue;
    std::get<1>(RANGE3) = Colors::Cyan;
    std::get<1>(RANGE4) = Colors::Magenta;
    std::get<1>(RANGE5) = Colors::Yellow;
}

} }
