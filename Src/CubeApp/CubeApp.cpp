#include "CubeApp.h"

CubeApp::CubeApp()
	: rotation(0)
{
	HR(D3DXCreateEffectFromFile(gD3DDevice, "Transform.fx", 0, 0, D3DXSHADER_DEBUG, 0, &mFX, &mErrors));
	if (mErrors)
	{
		MessageBox(0, (char*)mErrors->GetBufferPointer(), 0, 0);
	}

	mhTech = mFX->GetTechniqueByName("TransformTech");
	mhWVP = mFX->GetParameterByName(0, "gWVP");

	HR(mFX->SetTechnique(mhTech));

	// v1____v2
	// |    /|
	// |  /  |
	// |/____|
	// v3    v4
	// [v1, v3, v2, v2, v3, v4]

	//HR(gD3DDevice->CreateVertexBuffer(8 * sizeof(VertexPos), 0, D3DFVF_XYZW, D3DPOOL_MANAGED, &mVB, 0));
	HR(gD3DDevice->CreateVertexBuffer(8 * sizeof(VertexPosCol), 0, 0, D3DPOOL_MANAGED, &mVB, 0));
	HR(gD3DDevice->CreateIndexBuffer(36 * sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &mIB, 0));

	// Vertex Buffer
	VertexPosCol* vertices;
	HR(mVB->Lock(0, 0, (void**)&vertices, 0));

	vertices[0] = VertexPosCol(-0.5f, 0.5f, 0.5f, D3DCOLOR_XRGB(255,0,0));
	vertices[1] = VertexPosCol(-0.5f, -0.5f, 0.5f, D3DCOLOR_XRGB(0, 255, 0));
	vertices[2] = VertexPosCol(0.5f, 0.5f, 0.5f, D3DCOLOR_XRGB(0, 0, 255));
	vertices[3] = VertexPosCol(0.5f, -0.5f, 0.5f, D3DCOLOR_XRGB(255, 255, 0));
	vertices[4] = VertexPosCol(-0.5f, 0.5f, -0.5f, D3DCOLOR_XRGB(255, 0, 255));
	vertices[5] = VertexPosCol(-0.5f, -0.5f, -0.5f, D3DCOLOR_XRGB(0, 255, 255));
	vertices[6] = VertexPosCol(0.5f, 0.5f, -0.5f, D3DCOLOR_XRGB(255, 255, 255));
	vertices[7] = VertexPosCol(0.5f, -0.5f, -0.5f, D3DCOLOR_XRGB(0, 0, 0));

	//vertices[0] = VertexPos(-0.5f, 0.5f, 0.5f);
	//vertices[1] = VertexPos(-0.5f, -0.5f, 0.5f);
	//vertices[2] = VertexPos(0.5f, 0.5f, 0.5f);
	//vertices[3] = VertexPos(0.5f, -0.5f, 0.5f);
	//vertices[4] = VertexPos(-0.5f, 0.5f, -0.5f);
	//vertices[5] = VertexPos(-0.5f, -0.5f, -0.5f);
	//vertices[6] = VertexPos(0.5f, 0.5f, -0.5f);
	//vertices[7] = VertexPos(0.5f, -0.5f, -0.5f);

	HR(mVB->Unlock());

	// Index Buffer
	WORD* k;
	HR(mIB->Lock(0, 0, (void**)&k, 0));

	// Front
	k[0] = 0; k[1] = 2; k[2] = 1;
	k[3] = 1; k[4] = 2; k[5] = 3;

	// Back;
	k[6] = 4; k[7] = 6; k[8] = 5;
	k[9] = 5; k[10] = 6; k[11] = 7;

	// Left;
	k[12] = 0; k[13] = 4; k[14] = 5;
	k[15] = 5; k[16] = 1; k[17] = 0;

	// Right;
	k[18] = 2; k[19] = 3; k[20] = 7;
	k[21] = 7; k[22] = 6; k[23] = 2;

	// Top
	k[24] = 0; k[25] = 2; k[26] = 6;
	k[27] = 6; k[28] = 4; k[29] = 0;

	// Bottom;
	k[30] = 1; k[31] = 5; k[32] = 7;
	k[33] = 7; k[34] = 1; k[35] = 3;

	HR(mIB->Unlock());

	HR(gD3DDevice->SetStreamSource(0, mVB, 0, sizeof(VertexPosCol)));
	HR(gD3DDevice->SetIndices(mIB));
	HR(gD3DDevice->SetVertexDeclaration(VertexPosCol::decl));
	//HR(gD3DDevice->SetFVF(D3DFVF_XYZW));
}

CubeApp::~CubeApp()
{
}

void CubeApp::Update()
{
	rotation += 1.0f * gTimer->GetDeltaTime();
}

void CubeApp::Draw(ID3DXSprite* spriteBatch, const D3DXMATRIX& view, const D3DXMATRIX& proj)
{
	D3DXMatrixTranslation(&T, -2.0f, -2.0f, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&R, rotation, rotation, 0.0f);
	D3DXMatrixScaling(&S, 2.0f, 2.0f, 2.0f); 
	D3DXMATRIX WVP = S * R * T *view * proj;

	HR(mFX->SetMatrix(mhWVP, &WVP));

	UINT numPasses = 0;
	mFX->Begin(&numPasses, 0);

	for (int i = 0; i < numPasses; i++)
	{
		mFX->BeginPass(i);

		//gD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 6);
		HR(gD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12));

		mFX->EndPass();
	}

	mFX->End();
}
