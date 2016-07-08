// HLSL -> High Level Shading Language

//float4x4
//float4
//float3
//float2
//float 
//int
//bool

uniform extern float4x4 gWVP;

struct VSOut
{
			 // SEMANTIQUE
	float4 pos: POSITION0;
};

// Vertex Shader Function
VSOut TransformVS(float3 posL: POSITION0)
{
	VSOut vsOut = (VSOut)0;

	vsOut.pos = mul(float4(posL, 1.0f), gWVP);

	return vsOut;
}

// Pixel Shader Function
float4 TransformPS(): COLOR0
{
	return float4(0.0f, 0.0f, 0.0f, 0.0f);
}

technique TransformTech
{
	pass P0
	{
		vertexShader = compile vs_2_0 TransformVS();
	}

	pass P1
	{
		vertexShader = compile vs_2_0 TransformVS();
		pixelShader = compile ps_2_0 TransformPS();

		FillMode = WireFrame;
	}

};
