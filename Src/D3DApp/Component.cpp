#include "Component.h"

std::vector<Component*> Component::components;

Component::Component()
	: mTexture(nullptr)
	, mCenter(0.f, 0.f, 0.f)
	, mPosition(0.f, 0.f, 0.f)
	, hadBbis(false)
{
	D3DXMatrixIdentity(&R);
	D3DXMatrixIdentity(&T);
	D3DXMatrixScaling(&S, 1.0f, -1.0f, 1.0);

	// Ajoute l'instance au vector de Component*
	components.push_back(this);
}

Component::Component(bool _bbis) : mTexture(nullptr)
, mCenter(0.f, 0.f, 0.f)
, mPosition(0.f, 0.f, 0.f)
, hadBbis(_bbis)
{
	D3DXMatrixIdentity(&R);
	D3DXMatrixIdentity(&T);
	D3DXMatrixScaling(&S, 1.0f, -1.0f, 1.0);

	// Ajoute l'instance au vector de Component*
	components.push_back(this);
}

Component::~Component()
{
	ReleaseCOM(mTexture);
}

void Component::OnLostDevice()
{
}

void Component::OnResetDevice()
{
}
void Component::PreDraw()
{
	if (!hadBbis) {
		HR(gD3DDevice->Clear(
			0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_XRGB(119, 81, 44), 1.0f, 0));

		HR(gD3DDevice->BeginScene());
	}
}

void Component::PostDraw()
{
	if (!hadBbis) {
		HR(gD3DDevice->EndScene());
		HR(gD3DDevice->Present(0, 0, 0, 0));
	}
}

