// HLSL -> High Level Shading Language

//float4x4
//float4
//float3
//float2
//float 
//int
//bool

uniform extern float4x4 gWVP;

struct PosVS
{
			 // SEMANTIQUE
	float4 pos: POSITION0;
};

struct PosColVS
{
	// SEMANTIQUE
	float4 pos: POSITION0;
	float4 col: COLOR0;
};

 //Vertex Shader Function
PosVS TransformVS(float3 posL: POSITION0)
{
	PosVS vsOut = (PosVS)0;

	vsOut.pos = mul(float4(posL, 1.0f), gWVP);

	return vsOut;
}

// Vertex Shader Function
PosColVS TransformColVS(float3 posL: POSITION0, float4 col: COLOR0)
{
	PosColVS vsOut = (PosColVS)0;

	vsOut.pos = mul(float4(posL, 1.0f), gWVP);
	vsOut.col = col;

	return vsOut;
}

 //Pixel Shader Function
float4 TransformPS() : COLOR0
{
	return float4(0.0f, 0.0f, 0.0f, 0.0f);
}

float4 TransformColPS(float4 col: COLOR0): COLOR
{
	return col;
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

technique TransformColTech
{
	pass P0
	{
		vertexShader = compile vs_2_0 TransformColVS();
		pixelShader = compile ps_2_0 TransformColPS();
	}

	pass P1
	{
		vertexShader = compile vs_2_0 TransformColVS();
		pixelShader = compile ps_2_0 TransformPS();

		FillMode = WireFrame;
	}
};
