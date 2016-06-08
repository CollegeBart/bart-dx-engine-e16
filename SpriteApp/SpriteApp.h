#pragma once
#include "D3DApp.h"

class SpriteApp :
	public D3DApp
{
public:
	SpriteApp();
	SpriteApp(HINSTANCE hInstance, std::string winCaption);
	~SpriteApp();

	void OnLostDevice();
	void OnResetDevice();
	void Update();
	void Draw();

private:
	ID3DXSprite* mSpriteBatch;
	IDirect3DTexture9* mTexture;
	D3DXVECTOR3 mCenter;
	D3DXVECTOR3 mPosition;
};

