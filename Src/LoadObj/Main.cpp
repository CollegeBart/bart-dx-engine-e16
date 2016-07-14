#include "Obj.h"
#include "Engine.h"

int WINAPI WinMain(
	HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmd, int showCmd)
{
	gEngine = new Engine(hInstance, "Game: ");
	/* 
	ThreeDObject* obj = 
		new ThreeDObject("Art/menu_arwing.obj", "Art/texture.png","D3DObjViewer.fx");
	*/
	Obj* obj = new Obj("Art/menu_arwing.obj");
	D3DXVECTOR3 pos(0.f, 0.f, -10.f);
	gEngine->SetCameraPostion(pos);

	int res = gEngine->Run();

	delete obj;
	return res;
}