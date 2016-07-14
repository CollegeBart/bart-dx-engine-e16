#include "Obj.h"

Obj::Obj(char* const path) :
	Component(true),
	  rotation(0),g_pD3D(NULL),g_pTexture(NULL)
	, g_fScale(1),g_pEffect(NULL),g_pD3DMesh(NULL)
	, g_fFOV(45.5f), g_fAspect(1.333f), velocity(D3DXVECTOR3(0,0,0))
	, position(D3DXVECTOR3(0,0,0))
{
	//No error management
	HR(InitD3D());
	LoadObjectFile(path);
}

Obj::~Obj()
{
	SAFE_RELEASE(g_pTexture);
	SAFE_DELETE(g_pD3DMesh);
	SAFE_RELEASE(g_pEffect);
	SAFE_RELEASE(g_pD3D);
}

HRESULT Obj::OnCreateDevice()
{
	HRESULT hr;
	// Load the effect from file.
	LPD3DXBUFFER pErrors = NULL;
	hr = D3DXCreateEffectFromFile(gD3DDevice, TEXT("D3DObjViewer.fx"), NULL, NULL,
		0, NULL, &g_pEffect, &pErrors);
	if (FAILED(hr))
	{
		if (pErrors)
		{
			MessageBoxA(gD3DApp->GetMainWindow(), (LPCSTR)pErrors->GetBufferPointer(), "Effect error", MB_OK | MB_ICONSTOP);
			pErrors->Release();
		}
		return hr;
	}

	HR(D3DXCreateTextureFromFile(gD3DDevice, TEXT("Art/texture.png"), &g_pTexture));

	return S_OK;
}

void Obj::Update()
{
	ManageInput();
}

void Obj::Draw(ID3DXSprite * spriteBatch, const D3DXMATRIX & view, const D3DXMATRIX & proj)
{
	Render();
}

void Obj::Render()
{
	gD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0);
	if (FAILED(gD3DDevice->BeginScene()))
		return;
	// Draw the background gradient.
	RECT rClient;
	GetClientRect(gD3DApp->GetMainWindow(), &rClient);
	D3DXCOLOR c1(0.2, 0.2, 0.2, 1);
	D3DXCOLOR c2(0.45, 0.45, 0.45, 1);
	gD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	DrawTransformedQuad(gD3DDevice, -0.5f, -0.5f, 0.f,
		(FLOAT)(rClient.right - rClient.left),
		(FLOAT)(rClient.bottom - rClient.top),
		D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 0),
		D3DXVECTOR2(0, 1), D3DXVECTOR2(1, 1),
		c1, c1, c2, c2);
	gD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	gD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	g_pEffect->SetTexture("texDiffuse", g_pTexture);


	UINT passes = 0;
	if (g_pD3DMesh && SUCCEEDED(g_pEffect->Begin(&passes, 0)))
	{
		g_pEffect->BeginPass(0);

		CD3DMesh& d3dMesh = *g_pD3DMesh;

		// Scale the object based on bounding box.
		D3DXVECTOR3 bbSize = d3dMesh.bbmax - d3dMesh.bbmin;
		D3DXVECTOR3 bbCenter = d3dMesh.bbmax - bbSize / 2;
		FLOAT sizeFactor = MAX(bbSize.z, MAX(bbSize.x, bbSize.y));
		if (sizeFactor > 0)
			sizeFactor = 5 / sizeFactor;
		else
			sizeFactor = 1;
		if (g_fScale < 0.05f)
			g_fScale = 0.05f;

		sizeFactor *= g_fScale;

		D3DXMATRIX mT, mR, mS;
		D3DXMatrixTranslation(&mT, -bbCenter.x+position.x, -bbCenter.y+position.y, -bbCenter.z+position.z);
		D3DXMatrixScaling(&mS, sizeFactor, sizeFactor, sizeFactor);

		D3DXMATRIX mWorld = mT * mS;

		D3DXMatrixRotationY(&mR, rotation);
		mWorld *= mR;

		D3DXMATRIX mWVP, mWI, mWIT, mView, mProj;
		D3DXVECTOR3 vEye(0, 0, -5);
		D3DXMatrixLookAtLH(&mView, &vEye, &(D3DXVECTOR3(0, 0, 0)), &(D3DXVECTOR3(0, 1, 0)));
		D3DXMatrixPerspectiveFovLH(&mProj, g_fFOV, g_fAspect, 0.5f, 100.f);
		mWVP = mWorld * mView * mProj;
		D3DXMatrixInverse(&mWI, NULL, &mWorld);
		D3DXMatrixTranspose(&mWIT, &mWI);

		g_pEffect->SetMatrix("mWorld", &mWorld);
		g_pEffect->SetMatrix("mWVP", &mWVP);
		g_pEffect->SetMatrix("mWIT", &mWIT);

		g_pEffect->SetFloatArray("vEye", &vEye.x, 3);
		g_pEffect->CommitChanges();

		gD3DDevice->SetStreamSource(0, d3dMesh.pVB, 0, d3dMesh.vertexSize);
		gD3DDevice->SetFVF(d3dMesh.FVF);

		gD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, d3dMesh.triCount);

		g_pEffect->EndPass();
		g_pEffect->End();
	}
	gD3DDevice->EndScene();
	gD3DDevice->Present(NULL, NULL, NULL, NULL);

	gD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

HRESULT Obj::DrawTransformedQuad(LPDIRECT3DDEVICE9 pDevice,
	FLOAT x, FLOAT y, FLOAT z,
	FLOAT width, FLOAT height,
	D3DXVECTOR2 uvTopLeft, D3DXVECTOR2 uvTopRight,
	D3DXVECTOR2 uvBottomLeft, D3DXVECTOR2 uvBottomRight,
	D3DCOLOR c1, D3DCOLOR c2, D3DCOLOR c3, D3DCOLOR c4)
{
	struct
	{
		float pos[4];
		D3DCOLOR color;
		float uv[2];
	} quad[] =
	{
		x,			y,				z, 1.f,		c1, uvTopLeft.x,		uvTopLeft.y,
		x + width,	y,				z, 1.f,		c2, uvBottomRight.x,	uvTopLeft.y,
		x,			y + height,		z, 1.f,		c3, uvTopLeft.x,		uvBottomRight.y,
		x + width,	y + height,		z, 1.f,		c4, uvBottomRight.x,	uvBottomRight.y,
	};
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0));
	return pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, quad, sizeof(quad[0]));
}
TCHAR g_sObjFileName[MAX_PATH];

BOOL Obj::LoadObjectFile(LPCTSTR sFileName)
{
	BOOL flipTriangles = false;
	BOOL flipUVs = false;

	// Now attempt to load the file.
	TObjMesh objMesh;
	if (0 > LoadObj(sFileName, &objMesh))
	{
		OutputDebugString(TEXT("Failed to load the specified obj file!"));
		return FALSE;
	}

	CD3DMesh* pD3DMesh = new CD3DMesh;
	if (FAILED(pD3DMesh->Create(gD3DDevice, objMesh, flipTriangles, flipUVs)))
	{
		delete pD3DMesh;
		return FALSE;
	}

	StringCchCopy(g_sObjFileName, MAX_PATH, sFileName);

	SAFE_DELETE(g_pD3DMesh);
	g_pD3DMesh = pD3DMesh;

	// Now update the mesh info text.
	TCHAR sMeshInfo[MAX_PATH];
	sMeshInfo[0] = 0;

	StringCchPrintf(sMeshInfo, MAX_PATH,
		TEXT("Triangles: %d. Faces: %d. Verts: %d. Normals: %d. UVs: %d. ")
		TEXT("Materials: %d. Mat groups: %d"),
		objMesh.numTriangles, objMesh.faces.size(), objMesh.vertices.size(),
		objMesh.normals.size(), objMesh.texCoords.size(),
		objMesh.materials.size(), objMesh.matGroups.size());
	return TRUE;
}

HRESULT Obj::InitD3D()
{
	HRESULT hr;
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!g_pD3D)
		return E_FAIL;

	D3DDISPLAYMODE d3ddm;
	g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; // v-sync on - No need to burn the CPU and GPU over nothing.
	d3dpp.hDeviceWindow = gD3DApp->GetMainWindow();

	return OnCreateDevice();
}


void Obj::NormalizeVec3(D3DXVECTOR3* _vector)
{
	if (_vector->x > MOVEMENT)
		_vector->x = MOVEMENT;
	if (_vector->x < -MOVEMENT)
		_vector->x = -MOVEMENT;
			   
	if (_vector->y > MOVEMENT)
		_vector->y = MOVEMENT;
	if (_vector->y < -MOVEMENT)
		_vector->y = -MOVEMENT;
			   
	if (_vector->z > MOVEMENT)
		_vector->z = MOVEMENT;
	if (_vector->z < -MOVEMENT)
		_vector->z = -MOVEMENT;
}

void Obj::ManageInput()
{
	/*					ça bouge par rapport au devant du obj
	WASD for x and y 
	E and Q for z
	Space for rotate
	*/
	if ((gDInput->keyDown(DIK_W)) || (gDInput->keyDown(DIK_S)))
	{
		if (gDInput->keyDown(DIK_S))
		{
			velocity.y -= MOVEMENT;
		}
		else if ((gDInput->keyDown(DIK_W)))
		velocity.y += MOVEMENT;
	}else velocity.y = 0;


	if ((gDInput->keyDown(DIK_A)) || (gDInput->keyDown(DIK_D)))
	{
		if (gDInput->keyDown(DIK_D))
			velocity.x += MOVEMENT;
		else if (gDInput->keyDown(DIK_A))
			velocity.x -= MOVEMENT;
	}else velocity.x = 0;

	if ((gDInput->keyDown(DIK_E)) ||(gDInput->keyDown(DIK_Q)))
	{
		if(gDInput->keyDown(DIK_E))
			velocity.z += MOVEMENT;
		if (gDInput->keyDown(DIK_Q))
			velocity.z -= MOVEMENT;
	}
	else velocity.z = 0;


	if (gDInput->keyDown(DIK_SPACE))
		rotation += MOVEMENT;

	NormalizeVec3(&velocity);
	position += velocity;
}