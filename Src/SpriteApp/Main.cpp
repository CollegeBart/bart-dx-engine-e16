#include "D3DApp.h"
#include "SpriteApp.h"
#include "GameObject.h"
#include "Engine.h"
int WINAPI WinMain(
	HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmd, int showCmd)
{
	gEngine = new Engine(hInstance, "Game: ");
	
	//TestEnvironment* env = new TestEnvironment();
	GameObject* test = new GameObject("background.png");
	GameObject* test2 = new GameObject("hey-girl-sloth.jpg", 0,0);

	btVector3 pos{ 0,0,0 };
	test2->CreateBody(pos, 1.0f, SPHERE);

	int res = gEngine->Run();
	
	//delete env;
	
	return res;
}