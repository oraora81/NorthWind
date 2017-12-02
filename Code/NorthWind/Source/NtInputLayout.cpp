#include "NtCoreLib.h"
#pragma hdrstop

#include "NtInputLayout.h"

namespace nt {

const D3D11_INPUT_ELEMENT_DESC NtInputLayout::PNLayout[2] =
{
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


bool NtInputLayoutHandler::Initialize()
{


    return true;
}
    
void NtInputLayoutHandler::Release()
{

}

}
 