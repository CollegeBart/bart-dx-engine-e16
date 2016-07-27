uniform extern float4x4 gWVP;

struct VertexFinal
{
	float4 pos: POSITION0;
	float4 col: COLOR0;
};

VertexFinal TransformVS(float3 posL: POSITION0, float4 col: COLOR0)
{
	VertexFinal vf;
	vf.pos = mul(float4(posL, 1.0f), gWVP);
	vf.col = col;
	return vf;
}

float4 TransformPS(float4 col: COLOR0): COLOR0
{
	return col;
}

technique Tech0
{
	pass P0
	{
		vertexShader = compile vs_2_0 TransformVS();
		pixelShader = compile ps_2_0 TransformPS();

		CullMode = none;
	}
}
