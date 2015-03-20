
cbuffer cbChangeOnResize : register(b0)
{
	matrix Projection;
};

cbuffer cbChangesEveryFrame : register(b1)
{
	matrix MatWorld;
	matrix MatWorldViewProjection;
};

struct VS_INPUT
{
	float4 Pos       : POSITION;
	float3 VecNormal : NORMAL;
	float2 Tex       : TEXCOORD0;
};

struct VS_OPTPUT
{
	float4 Pos		 : SV_POSITION;
	float3 VecNormal : NORMAL;
	float2 Tex		 : TEXCOORD0;
};

VS_OPTPUT main(VS_INPUT input)
{
	VS_OPTPUT output;

	output.Pos = mul(input.Pos, MatWorldViewProjection);
	output.VecNormal = mul(input.VecNormal, (float3x3)MatWorld);
	output.Tex = input.Tex;

	return output;
}
