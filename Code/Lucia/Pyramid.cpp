#include "stdafx.h"
#pragma hdrstop

#include "Pyramid.h"

using namespace nt;

Pyramid::Pyramid()
    : NtModel()
{

}

Pyramid::~Pyramid()
{

}

void Pyramid::MakeGeometry()
{
    Vertex::PCVertex vertices[] =
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
    
    InitializeModelData(vertices, sizeof(Vertex::PCVertex), _countof(vertices), indices, _countof(indices));
}
