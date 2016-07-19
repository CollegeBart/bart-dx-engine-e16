#include "CubeApp.h"
#include "Engine.h"

int WINAPI WinMain(
	HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmd, int showCmd)
{
	gEngine = new Engine(hInstance, "Game: ");

	CubeApp* spriteApp = new CubeApp();

	D3DXVECTOR3 pos(0.f, 100.f, -100.f);
	gEngine->SetCameraPostion(pos);

	int res = gEngine->Run();

	delete spriteApp;

	return res;
}