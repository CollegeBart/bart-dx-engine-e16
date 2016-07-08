#pragma once
#include "PhysicsManager.h"
#include "D3DApp.h"
#include "Component.h"

#define CAMERA_DEFAULT_POS -10

#define PHYSICS PhysicsManager::Physics()

class Engine
	: public D3DApp
{
public:	
	Engine(HINSTANCE hInstance, std::string winCaption);
	Engine(HINSTANCE hInstance, std::string winCaption, int resWidth, int resHeight);
		
	~Engine();

	void SetCameraPostion(const D3DXVECTOR3& pos)
	{
		cameraPos = pos;
		OnResetDevice();
	}

	// Appel Update sur tout les Component dans le vector components	
	void OnLostDevice();
	void OnResetDevice();
	
	void Update();
	void Draw();
	int Run();

private:
	Engine();

	// 2D
	ID3DXSprite* mSpriteBatch;

	// 3D
	D3DXMATRIX view;
	D3DXMATRIX proj;

	// Camera
	D3DXVECTOR3 cameraPos;
	D3DXVECTOR3 cameraUp;
	D3DXVECTOR3 cameraTarget;

	static Engine* engine;
};

extern Engine* gEngine;

