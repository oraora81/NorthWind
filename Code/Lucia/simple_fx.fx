
// globals

cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
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

	// ���� ���� �������� ��ȯ
	vout.position = mul(float4(vin.position, 1.0f), gWorldViewProj);

	// ���� ������ �״�� �ȼ� ���̴��� ����
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
	}
};
