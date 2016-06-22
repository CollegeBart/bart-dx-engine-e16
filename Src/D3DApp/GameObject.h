#pragma once
#include "Component.h"
#include <iostream>
#include "Engine.h"
class GameObject : public Component
{
public:
	// Crée une GameObject et appel le constructeur de Component
	// le GameObject est donc ajouter au vector de Component
	GameObject();
	GameObject(const char* path);
	GameObject(const char* path, float startX, float startY);

	~GameObject();

	int GetWidth() const { return info.Width; }
	int GetHeight() const { return info.Height;	}

	void OnLostDevice();
	void OnResetDevice();

	virtual void CreateBody(const btVector3& pos, float mass, btCollisionShape* shape);
	virtual void CreateBody(const btVector3& pos, float mass, btCollisionShape* shape, short group, short mask);

	virtual void Start();
	virtual void Update();
	virtual void Draw(ID3DXSprite* spriteBatch);

protected:
	D3DXIMAGE_INFO info;

	D3DXMATRIX rot;
	D3DXMATRIX trans;
	D3DXMATRIX scale;
	D3DXMATRIX resultant;

	btTransform transform;
	btRigidBody* body;

private :
	bool needImpl;
	void MakeResultantMatrix();
};

