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

    static ID3D11InputLayout* PCInputLayout;
    static ID3D11InputLayout* PNInputLayout;
    static ID3D11InputLayout* PCInputLayoutEachSlot;
    static ID3D11InputLayout* PCInputLayout32bitColor;
};

}
