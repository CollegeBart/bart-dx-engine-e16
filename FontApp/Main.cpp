#include "Engine.h"
#include "GameObject.h"
#include "TextObject.h"


int WINAPI WinMain(
	HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmd, int showCmd)
{
	gEngine = new Engine(hInstance, "Game: ");

	//TestEnvironment* env = new TestEnvironment();
	TextObject* text = new TextObject();

	int res = gEngine->Run();

	//delete env;

	return res;
}