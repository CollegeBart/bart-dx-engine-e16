#include "D3DApp.h"
#include "SpriteApp.h"
#include "GameObject.h"
#include "Engine.h"
int WINAPI WinMain(
	HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmd, int showCmd)
{
	gEngine = new Engine(hInstance, "Game: ");

	SpriteApp* spriteApp = new SpriteApp();

	int res = gEngine->Run();
	
	delete spriteApp;
	
	return res;
}