#pragma once

namespace nt {

class NtInputLayout
{
public:
    static const D3D11_INPUT_ELEMENT_DESC PNLayout[2];
};


class NtInputLayoutHandler
{
public:
    static bool Initialize();
    static void Release();

    static D3D11_INPUT_ELEMENT_DESC* PNLayout;
};

}
