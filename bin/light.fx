#include "lighthelper.fx"

cbuffer cbPerFrame
{
    DirectionalLight gDirLight;
    PointLight gPointLight;
    SpotLight gSpotLight;
    float3 gEyePosW;
};

cbuffer cbPerObject
{
    float4x4 gWorld;
    float4x4 gWorldInvTranspose;
    float4x4 gWorldViewProj;
    Material gMaterial;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    // ����� ��ȯ
    vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
    vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);

    // ���� ���� �������� ��ȯ
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    // ���� ������ ������ �� �̻� �������Ͱ� �ƴ� �� �����Ƿ� �ٽ� ����ȭ�Ѵ�.
    pin.NormalW = normalize(pin.NormalW);

    float3 toEyeW = normalize(gEyePosW - pin.PosW);

    // ���е��� ���� 0�� ���� �Ӽ���� �����Ѵ�.
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // �� ������ �⿩�� ���� ���Ѵ�.
    float4 A, D, S;

    ComputeDirectionalLight(gMaterial, gDirLight, pin.NormalW, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

    ComputePointLight(gMaterial, gPointLight, pin.PosW, pin.NormalW, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

    ComputeSpotLight(gMaterial, gSpotLight, pin.PosW, pin.NormalW, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

    float4 litColor = ambient + diffuse + spec;

    // �л걤 ������ ���Ŀ� �ؽ����� ������ ���� ��ü���� ���� ������ ����Ѵ�.
    litColor.a = gMaterial.Diffuse.a;

    return litColor;
}

technique11 LightTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
};

// Debug
// fxc /Fc /Od /Zi /T fx_5_0 /Fo ...fxo ...
// Release
// fxc /T fx_5_0 /Fo ...fxo ...
