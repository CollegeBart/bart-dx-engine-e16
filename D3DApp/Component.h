#pragma once
#include <vector>
#include "D3DApp.h"
class Component
{
	friend class Engine;
public:
	Component();
	~Component();
	// Virtuel pure
	virtual void Update() = 0;
	virtual void Draw(ID3DXSprite* spriteBatch) = 0;

private:
	static std::vector<Component*> components;

protected:
	
	IDirect3DTexture9* mTexture;
	D3DXVECTOR3 mCenter;
	D3DXVECTOR3 mPosition;

};

