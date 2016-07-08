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
	void OnResetDevice()	{}
	void OnLostDevice()		{}
	void Update()			{}
	void Draw(ID3DXSprite* spriteBatch);

private:
	IDirect3DVertexBuffer9* mVB;
	IDirect3DIndexBuffer9* mIB;

};

