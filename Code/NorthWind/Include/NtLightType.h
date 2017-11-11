#pragma once

namespace nt {

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

        // (Position, Range) ���·� �ϳ��� 4���� ���Ϳ� ä�� ����
        XMFLOAT3 Position;
        float Range;
        // (A0, A1, A2, Pad)���·� �ϳ��� 4���� ���Ϳ� ä���.
        XMFLOAT3 Att;
        float Pad;
    };

    struct SpotLight
    {
        SpotLight() { Crt::MemSet(this, sizeof(this)); }

        XMFLOAT4 Ambient;
        XMFLOAT4 Diffuse;
        XMFLOAT4 Specular;

        // (Position, Range)���·� �ϳ��� 4���� ���Ϳ� ä���.
        XMFLOAT3 Position;
        float Range;

        // (Direction, Spot)���·� �ϳ��� 4���� ���Ϳ� ä���.
        XMFLOAT3 Direction;
        float Spot;

        // Att, Pad ���·� �ϳ��� 4���� ���Ϳ� ä���.
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
}
