#include "d3dApp.h"
#include <crtdbg.h>
#include "tchar.h"
#include <iostream>

using namespace std;

struct VertexPosCol
{
	VertexPosCol(float x, float y, float z, D3DCOLOR c)
		: pos(x, y, z), col(c) {}

	D3DXVECTOR3 pos;
	D3DCOLOR col;
};

class ExamDemo : public D3DApp
{
public:
	ExamDemo(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD requestedVP);
	~ExamDemo();

	bool checkDeviceCaps();
	void onLostDevice();
	void onResetDevice();
	void updateScene(float dt);
	void drawScene();

private:	
	RECT clientRect;

	float currRot = 0;

	D3DXMATRIX V;
	D3DXMATRIX P;

	D3DXHANDLE mhTech;
	D3DXHANDLE mhWVP;

	ID3DXEffect* mFx;
	ID3DXBuffer* mErrors;

	IDirect3DVertexBuffer9* mVB;
	IDirect3DIndexBuffer9* mIB;
	IDirect3DVertexDeclaration9* decl;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Standard input/output/error file pointers
	FILE *fpStdIn, *fpStdOut, *fpStdErr;

	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

		if(AllocConsole())
		{
			// Assign the stdin/stdout/stderr streams to the newly created console
			_tfreopen_s(&fpStdIn, _T("CONIN$"), _T("r"), stdin);
			_tfreopen_s(&fpStdOut, _T("CONOUT$"), _T("w"), stdout);
			_tfreopen_s(&fpStdErr, _T("CONOUT$"), _T("w"), stderr);
		}
	#endif

	ExamDemo app(hInstance, "Examen Partiel", D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	gd3dApp = &app;

	return gd3dApp->run();
}

ExamDemo::ExamDemo(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD requestedVP)
: D3DApp(hInstance, winCaption, devType, requestedVP)
{
	HR(D3DXCreateEffectFromFile(gd3dDevice, "Transform.fx", 0, 0, 0, 0, &mFx, &mErrors));
	
	mhWVP = mFx->GetParameterByName(0, "gWVP");

	mhTech = mFx->GetTechniqueByName("Tech0");
	mFx->SetTechnique(mhTech);

	//Décalaration de vertex
	D3DVERTEXELEMENT9 elements[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};
	HR(gd3dDevice->CreateVertexDeclaration(elements, &decl));

	// Création de vertex buffer
	HR(gd3dDevice->CreateVertexBuffer(3 * sizeof(VertexPosCol), 0, 0, D3DPOOL_MANAGED, &mVB, 0));
	VertexPosCol* v;
	HR(mVB->Lock(0, 0, (void**)&v, 0));
	v[0] = VertexPosCol(-0.5f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0));
	v[1] = VertexPosCol(0.5f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0));
	v[2] = VertexPosCol(0.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 255));
	HR(mVB->Unlock());

	// Création d'index buffer
	HR(gd3dDevice->CreateIndexBuffer(3 * sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &mIB, 0));
	WORD* k;
	HR(mIB->Lock(0, 0, (void**)&k, 0));
	k[0] = 0; k[1] = 1; k[2] = 2;
	HR(mIB->Unlock());

	if(!checkDeviceCaps())
	{
		MessageBox(0, "checkDeviceCaps() Failed", 0, 0);
		PostQuitMessage(0);
	}

	GetClientRect(mhMainWnd, &clientRect);
	
	onResetDevice();
}

ExamDemo::~ExamDemo()
{
	ReleaseCOM(mFx);
	ReleaseCOM(mErrors);
	ReleaseCOM(decl);
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}

bool ExamDemo::checkDeviceCaps()
{
	// Nothing to check.
	return true;
}

void ExamDemo::onLostDevice()
{
	mFx->OnLostDevice();
}

void ExamDemo::onResetDevice()
{
	mFx->OnResetDevice();

	// Sets up the camera 1000 units back looking at the origin.
	D3DXVECTOR3 pos(0.0f, 0.0f, -10.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXMatrixLookAtLH(&V, &pos, &target, &up);
	//HR(gd3dDevice->SetTransform(D3DTS_VIEW, &V));

	// The following code defines the volume of space the camera sees.
	RECT R;
	GetClientRect(mhMainWnd, &R);
	float width  = (float)R.right;
	float height = (float)R.bottom;
	D3DXMatrixPerspectiveFovLH(&P, D3DX_PI*0.25f, width/height, 1.0f, 5000.0f);
	//HR(gd3dDevice->SetTransform(D3DTS_PROJECTION, &P));

	// This code sets texture filters, which helps to smooth out distortions
	// when you scale a texture.  
	HR(gd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR));
	HR(gd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR));
	HR(gd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR));

	// This line of code disables Direct3D lighting.
	HR(gd3dDevice->SetRenderState(D3DRS_LIGHTING, false));
	
	// The following code specifies an alpha test and reference value.
	HR(gd3dDevice->SetRenderState(D3DRS_ALPHAREF, 10));
	HR(gd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER));

	// The following code is used to setup alpha blending.
	HR(gd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE));
	HR(gd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1));
	HR(gd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA));
	HR(gd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA));

	// Indicates that we are using 2D texture coordinates.
	HR(gd3dDevice->SetTextureStageState(
		0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2));
}

void ExamDemo::updateScene(float dt)
{
	currRot += 10.0f * dt;
}

void ExamDemo::drawScene()
{
	HR(gd3dDevice->SetStreamSource(0, mVB, 0, sizeof(VertexPosCol)));
	HR(gd3dDevice->SetVertexDeclaration(decl));
	HR(gd3dDevice->SetIndices(mIB));

	D3DXMATRIX world, Rx;
	D3DXMatrixIdentity(&world);
	D3DXMatrixRotationY(&Rx, currRot);


	world = world * Rx * V * P;
	mFx->SetMatrix(mhWVP, &world);

	// Clear the backbuffer and depth buffer.
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x88888888, 1.0f, 0));

	HR(gd3dDevice->BeginScene());
	
	UINT nbPass = 0;
	HR(mFx->Begin(&nbPass, 0));

	for (int i = 0; i < nbPass; i++)
	{
		HR(mFx->BeginPass(i));
		
		HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 3, 0, 3));	

		HR(mFx->EndPass());
	}

	HR(mFx->End());

	HR(gd3dDevice->EndScene());

	// Present the backbuffer.
	HR(gd3dDevice->Present(0, 0, 0, 0));
}