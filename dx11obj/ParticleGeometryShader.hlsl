struct GS_INPUT {
	float4 Pos       : SV_POSITION;
	float Size       : SIZE0;
	float Alpha      : ALPHA0;
};

struct GS_OPTPUT {
	float4 Pos       : SV_POSITION;
	float Alpha      : ALPHA0;
	float2 TexCoord  : TEXCOORD0;
};

[maxvertexcount(6)]
void main(
	point GS_INPUT input[1], 
	inout TriangleStream< GS_OPTPUT > output
)
{
	const float halvedSize = input[0].Size;
	float4 projectionPos = input[0].Pos;

	GS_OPTPUT topLeft;
	topLeft.Pos = float4(projectionPos.x - halvedSize, projectionPos.y + halvedSize, projectionPos.zw);
	topLeft.Alpha = input[0].Alpha;
	topLeft.TexCoord = float2(0.0f, 1.0f);

	GS_OPTPUT topRight;
	topRight.Pos = float4(projectionPos.x + halvedSize, projectionPos.y + halvedSize, projectionPos.zw);
	topRight.Alpha = input[0].Alpha;
	topRight.TexCoord = float2(1.0f, 1.0f);

	GS_OPTPUT bottomLeft;
	bottomLeft.Pos = float4(projectionPos.x - halvedSize, projectionPos.y - halvedSize, projectionPos.zw);
	bottomLeft.Alpha = input[0].Alpha;
	bottomLeft.TexCoord = float2(0.0f, 0.0f);

	GS_OPTPUT bottomRight;
	bottomRight.Pos = float4(projectionPos.x + halvedSize, projectionPos.y - halvedSize, projectionPos.zw);
	bottomRight.Alpha = input[0].Alpha;
	bottomRight.TexCoord = float2(1.0f, 0.0f);

	output.Append(topLeft);
	output.Append(topRight);
	output.Append(bottomLeft);

	output.Append(bottomRight);
	output.Append(topRight);
	output.Append(bottomLeft);
}