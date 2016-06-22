#include "D3DApp.h"
#include "SpriteApp.h"
#include "GameObject.h"
#include "Engine.h"
int WINAPI WinMain(
	HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmd, int showCmd)
{
	gEngine = new Engine(hInstance, "Game: ");
	
	D3DXVECTOR3 camPos(0.f, 0.f, -1000.f);
	gEngine->SetCameraPostion(camPos);

	//TestEnvironment* env = new TestEnvironment();
	GameObject* test = new GameObject("background.png");
	test->SetPosition(test->GetWidth() / 2, test->GetHeight());
	GameObject* test2 = new GameObject("hey-girl-sloth.jpg", -250,-200);

	btVector3 pos{ 0,0,0 };
	test2->CreateBody(pos, 1.0f, SPHERE);

	int res = gEngine->Run();
	
	//delete env;
	
	return res;
}