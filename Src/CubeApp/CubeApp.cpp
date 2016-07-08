#include "CubeApp.h"

CubeApp::CubeApp()
{
	// v1____v2
	// |    /|
	// |  /  |
	// |/____|
	// v3    v4
	// [v1, v3, v2, v2, v3, v4]
	HR(gD3DDevice->CreateVertexBuffer(8 * sizeof(D3DXVECTOR3), 0, D3DFVF_XYZW, D3DPOOL_MANAGED, &mVB, 0));
	HR(gD3DDevice->CreateIndexBuffer(36 * sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &mIB, 0));

	// Vertex Buffer
	D3DXVECTOR3* vertices;
	HR(mVB->Lock(0, 0, (void**)&vertices, 0));

	vertices[0] = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	vertices[1] = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	vertices[2] = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	vertices[3] = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	vertices[4] = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	vertices[5] = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	vertices[6] = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	vertices[7] = D3DXVECTOR3(0.5f, -0.5f, -0.5f);

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

	HR(gD3DDevice->SetStreamSource(0, mVB, 0, sizeof(D3DXVECTOR3)));
	HR(gD3DDevice->SetIndices(mIB));
	HR(gD3DDevice->SetFVF(D3DFVF_XYZW));
}

CubeApp::~CubeApp()
{
}

void CubeApp::Draw(ID3DXSprite* spriteBatch)
{
	//gD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 6);
	HR(gD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12));
}
