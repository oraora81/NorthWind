#include "stdafx.h"
#pragma hdrstop

#include "Pyramid.h"

Pyramid::Pyramid()
    : BaseShape()
{

}

Pyramid::~Pyramid()
{

}

void Pyramid::MakeGeometry()
{
    nt::renderer::NtModel::NtPCVertex vertices[] =
    {
        { XMFLOAT3(-5.0f, -2.0f, -2.0f), (const float*)&Colors::Green },
        { XMFLOAT3(-5.0f, -2.0f, +2.0f), (const float*)&Colors::Green },
        { XMFLOAT3(+5.0f, -2.0f, +2.0f), (const float*)&Colors::Green },
        { XMFLOAT3(+5.0f, -2.0f, -2.0f), (const float*)&Colors::Green },
        { XMFLOAT3(0.0f, 4.0f, 0.0f), (const float*)&Colors::Red },
    };

    ntUint indices[] =
    {
        // ����
        0, 4, 3,
        // ���� 1
        1, 4, 0,
        // ���� 2
        2, 4, 1,
        3, 4, 2,
        
        1, 0, 3,
        1, 3, 2,
    };
    
    const ntWchar* filePath = g_resManager->GetPath(L"simple_fx.fxo");
    //InitializeModelData(vertices, _countof(vertices), indices, _countof(indices), L"../Code/Lucia/simple_fx.fxo");
    InitializeModelData(vertices, _countof(vertices), indices, _countof(indices), filePath);
}