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
	void Draw(ID3DXSprite* spriteBatch);

private:

	float currentRotation;

	GameObject* bkg;
	GameObject* canon;
};

