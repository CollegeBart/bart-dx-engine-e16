#include "Engine.h"
#include "GameObject.h"
#include "TextObject.h"


int WINAPI WinMain(
	HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmd, int showCmd)
{
	gEngine = new Engine(hInstance, "Game: ");

	//TestEnvironment* env = new TestEnvironment();
	TextObject* text = new TextObject("Times New Roman", "lol",D3DCOLOR_XRGB(0,0,0));
	text->SetProperties(10, 10, true, true, D3DCOLOR_XRGB(10, 100, 25));
	text->SetText("Salut");																			
	TextObject* text2 = new TextObject("Times New Roman", "Center", D3DCOLOR_XRGB(0, 0, 0),
		D3DXVECTOR3(500,500,0)); 		//500,500 etant ça position dans le monde

	int res = gEngine->Run();

	//delete env;

	return res;
}