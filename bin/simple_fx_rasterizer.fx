
// globals

cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
};

RasterizerState WF
{
	FillMode = Wireframe;
	CullMode = Front;
	FrontCounterClockWise = false;
};

struct VertexInputType
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PixelInputType VS(VertexInputType vin)
{
	PixelInputType vout;

	// 동차 절단 공간으로 변환
	vout.position = mul(float4(vin.position, 1.0f), gWorldViewProj);

	// 정점 색상을 그대로 픽셸 셰이더에 전달
	vout.color = vin.color;

	return vout;
}

float4 PS(PixelInputType pin) : SV_Target
{
	return pin.color;
}

technique11 ColorTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
		SetRasterizerState(WF);
	}
};

// Debug
// fxc /Fc /Od /Zi /T fx_5_0 /Fo ...fxo ...
// Release
// fxc /T fx_5_0 /Fo ...fxo ...