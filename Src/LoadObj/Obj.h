#pragma once

#include "D3DApp.h"
#include "Component.h"
#include "D3DObjMesh.h"
#include "Resource.h"
#include <strsafe.h>

class Obj :
	public Component
{
public:
	Obj(char* const path);
	~Obj();

protected:
	void OnResetDevice() {}
	void OnLostDevice() {}
	HRESULT OnCreateDevice();
	void Update();
	void Draw(ID3DXSprite* spriteBatch, const D3DXMATRIX& view, const D3DXMATRIX& proj);

protected:
	void Render();
	HRESULT DrawTransformedQuad(LPDIRECT3DDEVICE9 pDevice,
		FLOAT x, FLOAT y, FLOAT z,
		FLOAT width, FLOAT height,
		D3DXVECTOR2 uvTopLeft, D3DXVECTOR2 uvTopRight,
		D3DXVECTOR2 uvBottomLeft, D3DXVECTOR2 uvBottomRight,
		D3DCOLOR c1, D3DCOLOR c2, D3DCOLOR c3, D3DCOLOR c4);

	BOOL LoadObjectFile(LPCTSTR sFileName);
	HRESULT InitD3D();


private:
	LPDIRECT3D9 g_pD3D ;
	LPDIRECT3DTEXTURE9 g_pTexture ;
	float						g_fScale ;
	// A simple effect file used for rendering the obj mesh.
	LPD3DXEFFECT g_pEffect ;
	CD3DMesh* g_pD3DMesh ; // The loaded object. Can be NULL.
	FLOAT g_fFOV;
	FLOAT g_fAspect;

	float rotation;

};

