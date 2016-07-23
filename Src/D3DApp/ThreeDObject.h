#pragma once
#include "D3DApp.h"
#include "Component.h"
#include "D3DObjMesh.h"
#include "Engine.h"
#include "Resource.h"
#include <strsafe.h>
#include "DirectInput.h"

class ThreeDObject :
	public Component
{
public:
	ThreeDObject(char* const _path, char* const _txPath,char* const _shader);
	ThreeDObject(char* const _path, char* const _txPath, char* const _shader,float _speed);
	~ThreeDObject();

	void Update();
	virtual void CreateBody(const btVector3& pos, float mass, btCollisionShape* shape);
	virtual void CreateBody(const btVector3& pos, float mass, btCollisionShape* shape, short group, short mask);
	void Draw(ID3DXSprite* spriteBatch, const D3DXMATRIX& view, const D3DXMATRIX& proj);
	
	D3DXVECTOR3 GetPosition()const { return objPosition; }
	void SetPosition(float _x, float _y, float _z);
	void SetPosition(D3DXVECTOR3 _position);
	
	D3DXVECTOR3 GetRotation() const { return objRotation; }
	void SetRotation(float _yaw, float _pitch, float _roll);
	void SetRotation(D3DXVECTOR3 _rotation);
	

	float GetScale() const { return g_fScale; }
	void SetScale(float _scale);


protected:
	void OnResetDevice() {}
	void OnLostDevice() {}
	HRESULT OnCreateDevice();
	
	HRESULT DrawTransformedQuad(LPDIRECT3DDEVICE9 pDevice,
		FLOAT x, FLOAT y, FLOAT z,
		FLOAT width, FLOAT height,
		D3DXVECTOR2 uvTopLeft, D3DXVECTOR2 uvTopRight,
		D3DXVECTOR2 uvBottomLeft, D3DXVECTOR2 uvBottomRight,
		D3DCOLOR c1, D3DCOLOR c2, D3DCOLOR c3, D3DCOLOR c4);

	BOOL LoadObjectFile(LPCTSTR sFileName);
	HRESULT InitD3D();
	D3DXVECTOR3 objPosition;
	D3DXVECTOR3 objRotation;

private:
	LPDIRECT3D9 g_pD3D;
	LPDIRECT3DTEXTURE9 g_pTexture;
	float						g_fScale;
	// A simple effect file used for rendering the obj mesh.
	LPD3DXEFFECT g_pEffect;
	CD3DMesh* g_pD3DMesh; // The loaded object. Can be NULL.
	FLOAT g_fFOV;
	FLOAT g_fAspect;

	float rotation;
	D3DXVECTOR3 bbCenter;

	btTransform transform;
	btRigidBody* body;
	char* const texturePath;
	char* const shaderName;
	float speed;
};

