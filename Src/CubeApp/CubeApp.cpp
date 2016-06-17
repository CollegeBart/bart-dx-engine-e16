#include "CubeApp.h"

CubeApp::CubeApp()
{
}

CubeApp::CubeApp(HINSTANCE hInstance, std::string winCaption)
	:D3DApp(hInstance, winCaption, 800, 600)
{
	// v1____v2
	// |    /|
	// |  /  |
	// |/____|
	// v3    v4
	// [v1, v3, v2, v2, v3, v4]
	HR(gD3DDevice->CreateVertexBuffer(6 * sizeof(D3DXVECTOR3), 0, D3DFVF_XYZW, D3DPOOL_MANAGED, &mVB, 0));

	D3DXVECTOR3* vertices;
	HR(mVB->Lock(0, 0, (void**)&vertices, 0));

	vertices[0] = D3DXVECTOR3(-0.5f, 0.5f, 0.f);
	vertices[1] = D3DXVECTOR3(-0.5f, -0.5f, 0.f);
	vertices[2] = D3DXVECTOR3(0.5f, 0.5f, 0.f);
	vertices[3] = D3DXVECTOR3(0.5f, 0.5f, 0.f);
	vertices[4] = D3DXVECTOR3(-0.5f, -0.5f, 0.f);
	vertices[5] = D3DXVECTOR3(0.5f, -0.5f, 0.f);

	HR(mVB->Unlock());

	HR(gD3DDevice->SetStreamSource(0, mVB, 0, sizeof(D3DXVECTOR3)));
	HR(gD3DDevice->SetFVF(D3DFVF_XYZW));
}

CubeApp::~CubeApp()
{
}

void CubeApp::Draw()
{
	gD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 6);
}
