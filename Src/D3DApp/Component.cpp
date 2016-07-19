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

