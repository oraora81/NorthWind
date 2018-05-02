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

public:

    static ID3D11RasterizerState* RSSolid;
    static ID3D11RasterizerState* RSWireFrame;
    static ID3D11RasterizerState* RSNoCull;
    static ID3D11RasterizerState* RSUI;

    static ID3D11BlendState* BSAlpha2Coverage;
    static ID3D11BlendState* BSTransparent;
    static ID3D11BlendState* BSUI;

    static ID3D11DepthStencilState* DSUI;

    static ID3D11SamplerState* SSUI;
};

}
}
