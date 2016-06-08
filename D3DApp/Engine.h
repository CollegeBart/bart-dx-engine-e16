#pragma once

#include "D3DApp.h"
#include "Component.h"

class Engine
	: public D3DApp
{
public:	
	Engine(HINSTANCE hInstance, std::string winCaption);
	~Engine();

	// Appel Update sur tout les Component dans le vector components	
	void Update();
	void Draw();
	int Run();

private:
	Engine() { }

	ID3DXSprite* mSpriteBatch;

	static Engine* engine;
};

extern Engine* gEngine;

