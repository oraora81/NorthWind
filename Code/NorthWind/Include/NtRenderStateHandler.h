#pragma once

namespace nt {    namespace renderer {

class NtRenderStateHandler
{
public:
    static bool Initialize(ID3D11Device* device);
    static void Release();

public:

    static ID3D11RasterizerState* RSSolid;
    static ID3D11RasterizerState* RSWireFrame;
    static ID3D11RasterizerState* RSNoCull;

    static ID3D11BlendState* BSAlpha2Coverage;
    static ID3D11BlendState* BSTransparent;
};

}
}
