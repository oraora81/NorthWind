
// typedef

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};



// Pixel Shader

//float4 ColorPixelShader(PixelInputType input) : SV_TARGET
//{
//	return input.color;
//}

float4 PS(PixelInputType pin) : SV_TARGET
{
	return pin.color;
}
