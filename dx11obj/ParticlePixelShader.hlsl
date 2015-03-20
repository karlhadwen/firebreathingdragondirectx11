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
// Textures and Texture Samplers.  These variables can't (it seems) go in	//
// constant buffers; I think the size of a CB is too restricted.			//
//																			//
// We only have one Texture2D variable here, what does that suggest about	//
// the nature or our mesh, i.e. could it be multi-textured?					//
//**************************************************************************//
Texture2D    txParticle : register(t0);
SamplerState samLinear : register(s0);




//**************************************************************************//
// Pixel shader input structure.	The semantics (the things after the		//
// colon) look a little weird.  The semantics are used (so Microsoft tell	//
// us) used by the compiler to link shader inputs and outputs. 				//
//																			//
// For this to work, you must ensure this structure is identical to the		//
// vertex shader's output structure.										//
//**************************************************************************//
struct PS_INPUT {
	float4 Pos       : SV_POSITION;
	float Alpha      : ALPHA0;
	float2 TexCoord  : TEXCOORD0;
};








//**************************************************************************//
// Pixel Shader.	This one has basic lighting, however the really			//
// important part is the rexture sampler.									//
//**************************************************************************//
float4 main(PS_INPUT input) : SV_Target {
	//return float4(input.texCoord, 1.0f);
	const float4 color = txParticle.Sample(samLinear, input.TexCoord);
	return float4(color.xyz, input.Alpha);
}
