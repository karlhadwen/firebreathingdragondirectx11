cbuffer cbChangesEveryFrame : register(b0) {
	matrix MatWorld;
	matrix MatWorldViewProjection;
};

struct VS_INPUT {
	float4 Pos       : POSITION;
	float3 VecNormal : NORMAL;
};

struct VS_OPTPUT {
	float4 Pos		 : SV_POSITION;
	float3 VecNormal : NORMAL;
	float3 texCoord  : TEXCOORD;
};

VS_OPTPUT main(VS_INPUT input) {
	VS_OPTPUT output;

	output.Pos = mul(input.Pos, MatWorldViewProjection);
	output.VecNormal = mul(input.VecNormal, (float3x3)MatWorld);
	output.texCoord = input.Pos.xyz;

	return output;
}

