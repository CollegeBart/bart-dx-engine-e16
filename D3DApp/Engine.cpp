#include "Engine.h"

extern Engine* gEngine = 0;

void Engine::Update()
{
	// Passe dans toute le vectir de Component
	for (int i = 0; i < Component::components.size(); i++)
	{
		Component::components[i]->Update();
	}
}

void Engine::Draw()
{
	for (int i = 0; i < Component::components.size(); i++)
	{
		Component::components[i]->Draw(mSpriteBatch);
	}
}

int Engine::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	gTimer->Start();

	while (msg.message != WM_QUIT)
	{

		gDInput->poll();
		gTimer->Tick();

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update();

			PreDraw();
			Draw();
			PostDraw();
		}
	}

	gTimer->Stop();

	return (int)msg.wParam;
}

Engine::Engine(HINSTANCE hInstance, std::string winCaption)
	: D3DApp(hInstance, winCaption)
	, mSpriteBatch(nullptr)
{
	gD3DApp = this;

	HR(D3DXCreateSprite(gD3DDevice, &mSpriteBatch));
}

Engine::~Engine()
{
}
