//**************************************************************************//
// Start of an OBJ loader.  By no means an end.  This just creates			//
// triangles.																//
//**************************************************************************//

//--------------------------------------------------------------------------------------
// File: Tutorial07 - Textures and Constant buffers.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//**************************************************************************//
// Constant Buffer Variables.												//
//**************************************************************************//
cbuffer cbNeverChanges : register(b0)
{
	float4 materialColour;
	float4 vecLight;
};



//**************************************************************************//
// Textures and Texture Samplers.  These variables can't (it seems) go in	//
// constant buffers; I think the size of a CB is too restricted.			//
//																			//
// We only have one Texture2D variable here, what does that suggest about	//
// the nature or our mesh, i.e. could it be multi-textured?					//
//**************************************************************************//
Texture2D    txDiffuse : register(t0);
SamplerState samLinear : register(s0);




//**************************************************************************//
// Pixel shader input structure.	The semantics (the things after the		//
// colon) look a little weird.  The semantics are used (so Microsoft tell	//
// us) used by the compiler to link shader inputs and outputs. 				//
//																			//
// For this to work, you must ensure this structure is identical to the		//
// vertex shader's output structure.										//
//**************************************************************************//
struct PS_INPUT
{
	float4 Pos		 : SV_POSITION;
	float3 vecNormal : NORMAL;
	float2 Tex		 : TEXCOORD0;
};




//**************************************************************************//
// Pixel Shader.	This one has no lighting.  You can have many pixel		//
// shaders in one of these files.											//
//**************************************************************************//
float4 main2(PS_INPUT input) : SV_Target
{
	return txDiffuse.Sample(samLinear, input.Tex) * materialColour;
}


//**************************************************************************//
// Pixel Shader.	This one has basic lighting, however the really			//
// important part is the rexture sampler.									//
//**************************************************************************//
float4 main(PS_INPUT input) : SV_Target
{
	//float4 light = saturate( dot( vecLight, input.vecNormal ) );
	float4 light = saturate(dot(vecLight, input.vecNormal) + 0.5f) + 0.2f;
	return txDiffuse.Sample(samLinear, input.Tex) * materialColour * light;
}