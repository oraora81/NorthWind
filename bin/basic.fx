#include "lighthelper.fx"

cbuffer cbPerFrame
{
    DirectionalLight gDirLights[3];
    float3 gEyePosW;

    float gFogStart;
    float gFogRange;
    float gFogColor;
};

cbuffer cbPerObject
{
    float4x4 gWorld;
    float4x4 gWorldInvTranspose;
    float4x4 gWorldViewProj;
    float4x4 gTexTransform;
    Material gMaterial;
};

Texture2D gDiffuseMap;

//SamplerState samTriLinear
//{
//    Filter = MIN_MAG_MIP_LINEAR;
//    AddressU = WRAP;
//    AddressV = WRAP;
//};

//SamplerState samTriLinear
//{
//    Filter = MIN_MAG_MIP_LINEAR;
//    AddressU = BORDER;
//    AddressV = BORDER;
//    BorderColor = float4(0.0f, 0.0f, 1.0f, 1.0f);
//};
//
//SamplerState samTriLinear
//{
//    Filter = MIN_MAG_MIP_LINEAR;
//    AddressU = CLAMP;
//    AddressV = CLAMP;
//};
//
//SamplerState samTriLinear
//{
//    Filter = MIN_MAG_MIP_LINEAR;
//    AddressU = MIRROR;
//    AddressV = MIRROR;
//};


SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;
    AddressU = WRAP;
    AddressV = WRAP;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float2 Tex : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    // Transform to world space space.
    vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
    vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);

    // Transform to homogeneous clip space
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

    // Output vertex attributes for interpolation across triangle
    vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

    return vout;
}

float4 PS(VertexOut pin, uniform int gLightCount, uniform bool gUseTexture) : SV_Target
{
    // Interpolating  normal can unnormalize it, so normalize it
    pin.NormalW = normalize(pin.NormalW);
    
    // The toEye vector is used in lighting.
    float3 toEye = gEyePosW - pin.PosW;

    // Cache the distance to the eye from this surface point
    float distToEye = length(toEye);

    // Normalize
    toEye /= distToEye;

    // Default to multiplicative identity
    float4 texColor = float4(1, 1, 1, 1);
    if (gUseTexture)
    {
        // Sample texture
        texColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
        //texColor = gDiffuseMap.Sample(samTriLinear, pin.Tex);
    }

    //
    // Lighting
    //

    float4 litColor = texColor;
    if (gLightCount > 0)
    {
        // Start with a sum of zero
        float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

        // Sum the light contribution from each light source
        [unroll]
        for (int i = 0; i < gLightCount; i++)
        {
            float4 A, D, S;
            ComputeDirectionalLight(gMaterial, gDirLights[i], pin.NormalW, toEye, A, D, S);

            ambient += A;
            diffuse += D;
            spec += S;
        }

        litColor = texColor * (ambient + diffuse) + spec;
    }

    // Common to take alpha from diffuse material and texture
    litColor.a = gMaterial.Diffuse.a * texColor.a;

    return litColor;
}

technique11 Light1
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(1, false)));
    }
};

technique11 Light2
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(2, false)));
    }
};

technique11 Light3
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(3, false)));
    }
};

technique11 Light0Tex
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(0, true)));
    }
};


technique11 Light1Tex
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(1, true)));
    }
};

technique11 Light2Tex
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(2, true)));
    }
};

technique11 Light3Tex
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(3, true)));
    }
};

// Debug
// fxc /Fc /Od /Zi /T fx_5_0 /Fo ...fxo ...
// Release
// fxc /T fx_5_0 /Fo ...fxo ...