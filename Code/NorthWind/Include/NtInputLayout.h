#pragma once

namespace nt {

class NtInputLayout
{
public:
    static const D3D11_INPUT_ELEMENT_DESC PCLayout[2];
    static const D3D11_INPUT_ELEMENT_DESC PNLayout[2];

    // ETC
    static const D3D11_INPUT_ELEMENT_DESC PCLayoutEachSlot[2];
    static const D3D11_INPUT_ELEMENT_DESC PCLayout32bitColor[2];
};


class NtInputLayoutHandler
{
public:
    static bool Initialize();
    static void Release();

    static D3D11_INPUT_ELEMENT_DESC* PCLayout;
    static D3D11_INPUT_ELEMENT_DESC* PNLayout;
    static D3D11_INPUT_ELEMENT_DESC* PCLayoutEachSlot;
    static D3D11_INPUT_ELEMENT_DESC* PCLayout32bitColor;
};

}
