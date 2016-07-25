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
	virtual void Draw(ID3DXSprite* spriteBatch, const D3DXMATRIX& view, const D3DXMATRIX& proj) = 0;

	D3DXVECTOR3 GetPosition() const
	{
		return mPosition;
	}

	void SetPosition(float x, float y, float z)
	{ 
		mPosition.x = x; 
		mPosition.y = y; 
		mPosition.z = z;

		D3DXMatrixTranslation(&T, x, y, z);
	}

	void SetPosition(const D3DXVECTOR3& pos)
	{
		SetPosition(pos.x, pos.y, pos.z);
	}

	D3DXVECTOR3 GetScale() const
	{
		return mScale;
	}

	void SetScale(float x, float y, float z)
	{
		mScale.x = x;
		mScale.y = y;
		mScale.z = z;

		D3DXMatrixScaling(&S, x, -y, z);
	}

	void SetScale(const D3DXVECTOR3& scale)
	{
		SetScale(scale.x, scale.y, scale.z);
	}

	void SetCenter(float x, float y, float z)
	{
		mCenter.x = x;
		mCenter.y = y;
		mCenter.z = z;
	}

	void SetCenter(D3DXVECTOR3* center)
	{
		mCenter = *center;
	}

	void SetRotationZ(float angle)
	{
		D3DXMatrixRotationZ(&R, angle);
	}

private:
	static std::vector<Component*> components;

protected:
	IDirect3DTexture9* mTexture;
	D3DXVECTOR3 mCenter;
	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mScale;

	D3DXMATRIX R;
	D3DXMATRIX T;
	D3DXMATRIX S;
	D3DXMATRIX resultant;
};

