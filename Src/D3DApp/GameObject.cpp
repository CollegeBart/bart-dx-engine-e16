#include "GameObject.h"

GameObject::GameObject()
	: Component()
	,
	needImpl(false)
{
	
}

GameObject::GameObject(const char * path)
	:Component()
	, needImpl(true)
{
	//HR(D3DXCreateTextureFromFile(gD3DDevice, path, &mTexture));
	HR(D3DXCreateTextureFromFileEx(gD3DDevice, path, 0, 0, 1, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
		D3DCOLOR_XRGB(255, 255, 255), &info, NULL, &mTexture));
}

GameObject::GameObject(const char * path, float startX, float startY)
	:Component()
	,needImpl(true)
{
	//HR(D3DXCreateTextureFromFile(gD3DDevice, path, &mTexture));
	HR(D3DXCreateTextureFromFileEx(gD3DDevice, path, 0, 0, 1, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
		D3DCOLOR_XRGB(255, 255, 255), &info, NULL, &mTexture));

	mPosition.x = startX;
	mPosition.y = startY;
}

GameObject::~GameObject()
{
}

void GameObject::OnLostDevice()
{
}

void GameObject::OnResetDevice()
{
}

void GameObject::Start()
{
}

void GameObject::Update()
{
	
}

void GameObject::Draw(ID3DXSprite* spriteBatch)
{
	if (needImpl)
	{
		D3DXMATRIX S;
		D3DXMatrixScaling(&S, 1.f, -1.0f, 1.0f);

		HR(spriteBatch->Begin(
			D3DXSPRITE_ALPHABLEND |
			D3DXSPRITE_OBJECTSPACE |
			D3DXSPRITE_DONOTMODIFY_RENDERSTATE));
		{
			HR(spriteBatch->SetTransform(&S));
			HR(spriteBatch->Draw(mTexture, 0,
				&mCenter, &mPosition, D3DCOLOR_XRGB(255, 255, 255)));
		}

		HR(spriteBatch->End());
	}
}