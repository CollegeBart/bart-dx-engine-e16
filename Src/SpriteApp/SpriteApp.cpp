#include "SpriteApp.h"

SpriteApp::SpriteApp()
	: mSpriteBatch(nullptr)
	, mTexture(nullptr)
	, mCenter(0.f, 0.f, 0.f)
	, mPosition(0.f, 0.f, 0.f)
{

}

SpriteApp::SpriteApp(HINSTANCE hInstance, std::string winCaption)
	: D3DApp(hInstance, winCaption)
	, mSpriteBatch(nullptr)
	, mTexture(nullptr)
	, mCenter(0.f, 0.f, 0.f)
	, mPosition(-392.f, -281.f, 0.f)
{
	HR(D3DXCreateSprite(gD3DDevice, &mSpriteBatch));

	// TODO - D3DXCreateTextureFromFileEx
	HR(D3DXCreateTextureFromFile(gD3DDevice, "hey-girl-sloth.jpg", &mTexture));
}

SpriteApp::~SpriteApp()
{

}

void SpriteApp::OnLostDevice()
{
	HR(mSpriteBatch->OnLostDevice());
}

void SpriteApp::OnResetDevice()
{
	HR(mSpriteBatch->OnResetDevice());
}

void SpriteApp::Update()
{
	if (gDInput->keyDown(DIK_SPACE))
	{
		mPosition.x += 100.0f * gTimer->GetDeltaTime();
	}

	
}

void SpriteApp::Draw()
{
	D3DXMATRIX S;
	D3DXMatrixScaling(&S, 1.f, -1.0f, 1.0f);

	HR(mSpriteBatch->Begin(
		D3DXSPRITE_ALPHABLEND |
		D3DXSPRITE_OBJECTSPACE |
		D3DXSPRITE_DONOTMODIFY_RENDERSTATE));
	{
		HR(mSpriteBatch->SetTransform(&S));
		HR(mSpriteBatch->Draw(mTexture, 0,
			&mCenter, &mPosition, D3DCOLOR_XRGB(255, 255, 255)));
	}

	HR(mSpriteBatch->End());
}
