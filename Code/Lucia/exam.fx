
struct Light
{
	float3 dir;
	float3 pos;
	float range;
	float3 att;
	float4 ambient;
	float4 diffuse;
};

cbuffer cbPerFrame
{
	Light light;
};

cbuffer cbPerObject
{
	float4x4 WVP;
	float4x4 World;
};

Texture2D objTexture;
SamplerState objSamplerState;

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 WorldPos : POSITION;
	float2 TexCoord : TEXCOORD;
	float3 normal : NORMAL;
};

VS_OUTPUT VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
	VS_OUTPUT output;
	
	output.Pos = mul(inPos, WVP);
	output.WorldPos = mul(inPos, World);
	output.normal = mul(normal, World);
	output.TexCoord = inTexCoord;
	
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
{
	input.normal = normalize(input.normal);

	float4 diffuse = objTexture.Sample(objSamplerState, input.TexCoord);
	//clip(diffuse.a - 0.1);

	float3 finalColor = float3(0.0f, 0.0f, 0.0f);
	
	// create the vector between lignht position and pixels position
	float3 lightToPixelVec = light.pos - input.WorldPos;

	// Find the distance between the light pos and pixel pos
	float d = length(lightToPixelVec);

	// create the ambient light
	float3 finalAmbient = diffuse * light.ambient;

	// if pixel is too far, return pixel color with ambient light
	if (d > light.range )
	{
		return float4(finalAmbient, diffuse.a);
	}

	// Turn lightToPixelVec into a unit length vector describing
	// the pixels direction from the lights position
	lightToPixelVec /= d;

	// calculate how much light the pixel gets by the angle
	// int which the light strikes the pixels surface
	float howMuchLight = dot(lightToPixelVec, input.normal);

	// if light striking the front side of the pixel
	if (howMuchLight > 0.0f )
	{
		// add light to the finalColor of the pixel
		finalColor += howMuchLight * diffuse * light.diffuse;

		// calculate light's falloff factor
		finalColor /= light.att[0] + (light.att[1] * d) + (light.att[2] * (d * d));
	}

	// make sure the values are between 0 and 1, and add the ambient
	finalColor = saturate(finalColor + finalAmbient);

	return float4(finalColor, diffuse.a);
}

float4 D2D_PS(VS_OUTPUT input) : SV_TARGET
{
	float4 diffuse = objTexture.Sample(objSamplerState, input.TexCoord);

	return diffuse;
}
