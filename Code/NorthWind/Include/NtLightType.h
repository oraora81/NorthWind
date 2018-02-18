#pragma once

namespace nt { namespace renderer {

struct DirectionalLight
{
    DirectionalLight() { Crt::MemSet(this, sizeof(this)); }

    XMFLOAT4 Ambient;
    XMFLOAT4 Diffuse;
    XMFLOAT4 Specular;
    XMFLOAT3 Direction;
    float Padding;
};

struct PointLight
{
    PointLight() { Crt::MemSet(this, sizeof(this)); }

    XMFLOAT4 Ambient;
    XMFLOAT4 Diffuse;
    XMFLOAT4 Specular;

    // (Position, Range) 형태로 하나의 4차원 벡터에 채워 넣음
    XMFLOAT3 Position;
    float Range;
    // (A0, A1, A2, Pad)형태로 하나의 4차원 벡터에 채운다.
    XMFLOAT3 Att;
    float Pad;
};

struct SpotLight
{
    SpotLight() { Crt::MemSet(this, sizeof(this)); }

    XMFLOAT4 Ambient;
    XMFLOAT4 Diffuse;
    XMFLOAT4 Specular;

    // (Position, Range)형태로 하나의 4차원 벡터에 채운다.
    XMFLOAT3 Position;
    float Range;

    // (Direction, Spot)형태로 하나의 4차원 벡터에 채운다.
    XMFLOAT3 Direction;
    float Spot;

    // Att, Pad 형태로 하나의 4차원 벡터에 채운다.
    XMFLOAT3 Att;
    float Pad;
};

struct Material
{
    Material() { Crt::MemSet(this, sizeof(this)); }

    XMFLOAT4 Ambient;
    XMFLOAT4 Diffuse;
    XMFLOAT4 Specular;  // w = specpower
    XMFLOAT4 Reflect;
};

} }
