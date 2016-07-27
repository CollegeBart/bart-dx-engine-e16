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

void Engine::PreDraw()
{
	HR(gD3DDevice->Clear(
		0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(15, 15, 15), 1.0f, 0));
		//D3DCOLOR_XRGB(119, 81, 44), 1.0f, 0));

	HR(gD3DDevice->BeginScene());

	
}

void Engine::Draw()
{
	for (int i = 0; i < Component::components.size(); i++)
	{
		Component::components[i]->Draw(mSpriteBatch, view, proj);
	}
}
void Engine::PostDraw()
{
	HR(gD3DDevice->EndScene());
	HR(gD3DDevice->Present(0, 0, 0, 0));


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
			
			PHYSICS->Step(gTimer->GetDeltaTime());

			PreDraw();
			Draw();
			PostDraw();
		}
	}

	gTimer->Stop();

	return (int)msg.wParam;
}

Engine::Engine()
	: cameraPos(0.f, 0.f, CAMERA_DEFAULT_POS)
	, cameraUp(0.f, 1.f, 0.f)
	, cameraTarget(0.f, 0.f, 0.f)
{
	OnResetDevice();
}

Engine::Engine(HINSTANCE hInstance, std::string winCaption)
	: D3DApp(hInstance, winCaption)
	, mSpriteBatch(nullptr)
	, cameraPos(0.f, 0.f, CAMERA_DEFAULT_POS)
	, cameraUp(0.f, 1.f, 0.f)
	, cameraTarget(0.f, 0.f, 0.f)
{
	gD3DApp = this;

	HR(D3DXCreateSprite(gD3DDevice, &mSpriteBatch));
	PHYSICS->CreateEmptyWorld();

	OnResetDevice();
}

Engine::Engine(HINSTANCE hInstance, std::string winCaption, int resWidth, int resHeight)
	: D3DApp(hInstance, winCaption, resWidth, resHeight)
	, mSpriteBatch(nullptr)
	, cameraPos(0.f, 0.f, CAMERA_DEFAULT_POS)
	, cameraUp(0.f, 1.f, 0.f)
	, cameraTarget(0.f, 0.f, 0.f)
{
	gD3DApp = this;

	HR(D3DXCreateSprite(gD3DDevice, &mSpriteBatch));
	PHYSICS->CreateEmptyWorld();

	OnResetDevice();
}

Engine::~Engine()
{
	PHYSICS->exitPhysics();
}

void Engine::OnLostDevice()
{
	for (int i = 0; i < Component::components.size(); i++)
	{
		Component::components[i]->OnLostDevice();
	}
}

void Engine::OnResetDevice()
{
	RECT r;
	GetClientRect(gD3DApp->GetMainWindow(), &r);

	D3DXMatrixLookAtLH(&view, &cameraPos, &cameraTarget, &cameraUp);
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI*0.25f, (float) r.right / (float)r.bottom, 0.1f, 5000000.0f);

	//HR(gD3DDevice->SetTransform(D3DTS_VIEW, &view));
	//HR(gD3DDevice->SetTransform(D3DTS_PROJECTION, &persp));


	for (int i = 0; i < Component::components.size(); i++)
	{
		Component::components[i]->OnResetDevice();
	}
}
