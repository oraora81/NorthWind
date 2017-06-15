
// vertex shader file


// globals

cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
};


// typedef

struct VertexInputType
{
	float3 position : POSITION;
	float4 color : COLOR;
};

//
struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};


// Vertex Shader
PixelInputType VS(VertexInputType  vin)
{
	PixelInputType pout;

	pout.position = mul(float4(vin.position, 1.0f), gWorldViewProj);

	pout.color = vin.color;

	return pout;
}

//PixelInputType ColorVertexShader(VertexInputType input)
//{
//	PixelInputType output;
//
//	// change the position vector to be 4units for proper matrix calculation.
//	input.position.w = 1.0f;
//
//	// calculate the position of the vertex against the world, view, and proj matrix
//	output.position = mul(input.position, world);
//	output.position = mul(output.position, view);
//	output.position = mul(output.position, proj);
//
//	// store the input color for the pixel shader to use
//	output.color = input.color;
//
//	return output;
//}
