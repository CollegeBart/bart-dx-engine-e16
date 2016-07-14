#pragma once

#include "D3DApp.h"
#include "Component.h"

class CubeApp :
	public Component
{
public:
	CubeApp();
	~CubeApp();

protected:
	void OnResetDevice() {}
	void OnLostDevice() {}
	void Update();
	void Draw(ID3DXSprite* spriteBatch, const D3DXMATRIX& view, const D3DXMATRIX& proj);

private:
	IDirect3DVertexBuffer9* mVB;
	IDirect3DIndexBuffer9* mIB;

	ID3DXEffect* mFX;
	ID3DXBuffer* mErrors;

	D3DXHANDLE mhTech;
	D3DXHANDLE mhWVP;

	float rotation;

	D3DXMATRIX R;
	D3DXMATRIX T;
	D3DXMATRIX S;
};