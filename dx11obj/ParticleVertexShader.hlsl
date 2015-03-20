struct VS_INPUT {
	float4 Pos       : POSITION0;
	float Size       : SIZE0;
	float Alpha      : ALPHA0;
};

struct VS_OPTPUT {
	float4 Pos       : SV_POSITION;
	float Size       : SIZE0;
	float Alpha      : ALPHA0;
};

cbuffer cbChangesEveryFrame : register(b0) {
	matrix MatWorldViewProjection;
};

VS_OPTPUT main(VS_INPUT input) {
	VS_OPTPUT output;

	output.Pos = mul(input.Pos, MatWorldViewProjection);
	output.Size = input.Size;
	output.Alpha = input.Alpha;

	return output;
}

