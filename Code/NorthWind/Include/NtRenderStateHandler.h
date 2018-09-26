#pragma once

namespace nt {    namespace renderer {

class NtRenderStateHandler
{
public:
    static bool Initialize(ID3D11Device* device);
    static void Release();

    static bool InitRS(ID3D11Device* device);
    static bool InitBS(ID3D11Device* device);
    static bool InitDS(ID3D11Device* device);
    static bool Etc(ID3D11Device* device);
    static void InitOverdrawColors();

public:

    static ID3D11RasterizerState* RSSolid;
    static ID3D11RasterizerState* RSWireFrame;
    static ID3D11RasterizerState* RSNoCull;
	static ID3D11RasterizerState* RSCullClockwise;
    static ID3D11RasterizerState* RSUI;

    static ID3D11BlendState* BSAlpha2Coverage;
    static ID3D11BlendState* BSTransparent;
	static ID3D11BlendState* BSNoRenderTargetWrite;
    static ID3D11BlendState* BSUI;
    static ID3D11BlendState* BSAdd;

	static ID3D11DepthStencilState* DSMarkMirror;
	static ID3D11DepthStencilState* DSDrawReflection;
	static ID3D11DepthStencilState* DSNoDoubleBlend;
    static ID3D11DepthStencilState* DSNoneDepth;
    static ID3D11DepthStencilState* DSUI;
    static ID3D11DepthStencilState* DSOverdrawTest;

    static ID3D11SamplerState* SSUI;

    static std::tuple<ntUchar, XMVECTORF32> RANGE0;
    static std::tuple<ntUchar, XMVECTORF32> RANGE1;
    static std::tuple<ntUchar, XMVECTORF32> RANGE2;
    static std::tuple<ntUchar, XMVECTORF32> RANGE3;
    static std::tuple<ntUchar, XMVECTORF32> RANGE4;
    static std::tuple<ntUchar, XMVECTORF32> RANGE5;
};

}
}
