#pragma once
#include "PhysicsManager.h"
#include "D3DApp.h"
#include "Component.h"

#define PHYSICS PhysicsManager::Physics()

class Engine
	: public D3DApp
{
public:	
	Engine(HINSTANCE hInstance, std::string winCaption);
	Engine(HINSTANCE hInstance, std::string winCaption, int resWidth, int resHeight);
		
	~Engine();

	// Appel Update sur tout les Component dans le vector components	
	void OnLostDevice();
	void OnResetDevice();
	
	void Update();
	void Draw();
	int Run();

private:
	Engine() { }

	ID3DXSprite* mSpriteBatch;

	static Engine* engine;
};

extern Engine* gEngine;

