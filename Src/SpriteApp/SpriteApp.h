#pragma once

#include "D3DApp.h"
#include "GameObject.h"

class SpriteApp :
	public Component
{
public:
	SpriteApp();
	~SpriteApp();

	void OnLostDevice();
	void OnResetDevice();
	void Update();
	void Draw(ID3DXSprite* spriteBatch, const D3DXMATRIX& view, const D3DXMATRIX& proj);

private:

	float currentRotation;

	GameObject* bkg;
	GameObject* canon;
};

