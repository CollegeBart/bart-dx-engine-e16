// HLSL -> High Level Shading Language

//float4x4
//float4
//float3
//float2
//float 
//int
//bool

uniform extern float4x4 gWVP;
uniform extern float gTime;

uniform extern float3 gEye;
uniform extern float4 gFogColor;
uniform extern float gMinDist;
uniform extern float gMaxDist;

// Amplitude
static float a[3] = { 8.f, 0.2f, 1.2f };

// Angular Wave number
static float k[3] = { 0.1f, 0.2f, 0.4f };

// Angular Frequency
static float w[3] = { 1.0f, 2.0f, 4.0f };

// Phase shift
static float p[3] = { 0.0f, 1.0f, 4.0f };

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

	// Vertex Data
	float colLerp: TEXCOORD3;
	float fogLerp: TEXCOORD4;
};

float SumOfRadialSineWave(float x, float z)
{
	// Distance du centre.
	float d = sqrt(x*x + z*z);

	// Sum the waves
	float sum = 0.0f;
	for (int i = 0; i < 2; i++)
	{
		sum += a[i] * sin(k[i] * d - gTime * w[i] + p[i]);
	}
	return sum;
}

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

// Vertex Shader Function
PosColVS WaveColVS(float3 posL: POSITION0, float4 col : COLOR0)
{
	PosColVS vsOut = (PosColVS)0;

	posL.y = SumOfRadialSineWave(posL.x, posL.z);

	vsOut.pos = mul(float4(posL, 1.0f), gWVP);
	vsOut.col = col;
	vsOut.colLerp = posL.y / 8.0f;

	float dist = distance(posL, gEye);
	vsOut.fogLerp = saturate((dist - gMinDist) / (gMaxDist - gMinDist));

	return vsOut;
}

 //Pixel Shader Function
float4 TransformPS() : COLOR0
{
	return float4(0.0f, 0.0f, 0.0f, 0.0f);
}

float4 TransformColPS(float4 col: COLOR0, float colLerp: TEXCOORD3): COLOR0
{
	float4 col1 = float4(1.0f, 0.0f, 0.0f, 1.0f);
	float4 col2 = float4(0.0f, 1.0f, 0.0f, 1.0f);

	return lerp(col1, col2, colLerp);
}

float4 TransformFogPS(PosColVS vsIn) : COLOR0
{
	float4 finalColor = lerp(vsIn.col, gFogColor, vsIn.fogLerp);
	return finalColor;
}

technique WaveColTech
{
	pass P0
	{
		vertexShader = compile vs_2_0 WaveColVS();
		pixelShader = compile ps_2_0 TransformColPS();
	}

	pass P1
	{
		vertexShader = compile vs_2_0 WaveColVS();
		pixelShader = compile ps_2_0 TransformPS();

		FillMode = WireFrame;
	}
};

technique WaveFogColTech
{
	pass P0
	{
		vertexShader = compile vs_2_0 WaveColVS();
		pixelShader = compile ps_2_0 TransformFogPS();
	}

	pass P1
	{
		vertexShader = compile vs_2_0 WaveColVS();
		pixelShader = compile ps_2_0 TransformPS();
	
		FillMode = WireFrame;
	}
};

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
		//pixelShader = compile ps_2_0 TransformColPS();
	}

	pass P1
	{
		vertexShader = compile vs_2_0 TransformColVS();
		pixelShader = compile ps_2_0 TransformPS();

		FillMode = WireFrame;
	}
};
