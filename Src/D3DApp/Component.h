#pragma once
#include <vector>
#include "D3DApp.h"
class Component
{
	friend class Engine;
public:
	Component();
	~Component();

	virtual void OnLostDevice();
	virtual void OnResetDevice();

	// Virtuel pure
	virtual void Update() = 0;
	virtual void Draw(ID3DXSprite* spriteBatch) = 0;

	D3DXVECTOR3 GetPosition() const
	{
		return mPosition;
	}

	void SetPosition(float x, float y, float z)
	{ 
		mPosition.x = x; 
		mPosition.y = y; 
		mPosition.z = z;
	}

	void SetPosition(D3DXVECTOR3* pos)
	{
		mPosition = *pos;
	}

private:
	static std::vector<Component*> components;

protected:
	
	IDirect3DTexture9* mTexture;
	D3DXVECTOR3 mCenter;
	D3DXVECTOR3 mPosition;
};

