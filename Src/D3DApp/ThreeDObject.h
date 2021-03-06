#pragma once
#include "D3DApp.h"
#include "Component.h"
#include "D3DObjMesh.h"
#include "Engine.h"
#include "Resource.h"
#include <strsafe.h>
#include "DirectInput.h"
#include <math.h>

class ThreeDObject :
	public Component
{
public:
	ThreeDObject(char* const _path, char* const _txPath,char* const _shader);
	ThreeDObject(char* const _path, char* const _txPath, char* const _shader,float _speed);
	~ThreeDObject();

	void Update();
	virtual void CreateBody();
	void Draw(ID3DXSprite* spriteBatch, const D3DXMATRIX& view, const D3DXMATRIX& proj);
	
	D3DXVECTOR3 GetPosition()const { return objPosition; }

	void ApplyPosition(float _x, float _y, float _z);
	void ApplyPosition(D3DXVECTOR3 _position);
	
	D3DXVECTOR3 GetRotation() const { return objRotation; }
	void SetRotation(float _yaw, float _pitch, float _roll);
	void SetRotation(D3DXVECTOR3 _rotation);

	btVector3 GetSize() const;
	float GetScale() const { return g_fScale; }
	void SetScale(float _scale);
	
	void SetTag(char* tag) { this->tag = tag; }
	char* GetTag()const { return tag; }

	bool GetBody() const { return hadBody; }

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

	ThreeDObject* OnCollisionEnter();
	virtual void CheckCollision() = 0;

	BOOL LoadObjectFile(LPCTSTR sFileName);
	HRESULT InitD3D();
	D3DXVECTOR3 objPosition;
	D3DXVECTOR3 objRotation;
	D3DXMATRIX mT, mR, mS;
	char* tag;

private:
	static std::vector<ThreeDObject*> colliderObjects;
	static std::vector<ThreeDObject*> objectToDelete;
	void DeleteObject();
	LPDIRECT3D9 g_pD3D;
	LPDIRECT3DTEXTURE9 g_pTexture;
	float g_fScale;
	// A simple effect file used for rendering the obj mesh.
	LPD3DXEFFECT g_pEffect;
	CD3DMesh* g_pD3DMesh; // The loaded object. Can be NULL.
	FLOAT g_fFOV;
	FLOAT g_fAspect;

	float rotation;
	D3DXVECTOR3 bbCenter;
	

	
	bool hadBody;
	char* const texturePath;
	char* const shaderName;
	float speed;
};

