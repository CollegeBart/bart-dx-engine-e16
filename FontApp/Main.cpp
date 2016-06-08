#include "D3DApp.h"

int WINAPI WinMain(
	HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmd, int showCmd)
{
	D3DApp* app = new D3DApp(hInstance, "D3DApp");
	gD3DApp = app;

	int assert = app->Run();

	delete app;

	return assert;
}