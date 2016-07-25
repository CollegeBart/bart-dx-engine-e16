#include "Component.h"

std::vector<Component*> Component::components;

Component::Component()
	: mTexture(nullptr)
	, mCenter(0.f, 0.f, 0.f)
	, mPosition(0.f, 0.f, 0.f)
	, mScale(1.0f, -1.0f, 1.0)
{
	D3DXMatrixIdentity(&R);
	D3DXMatrixIdentity(&T);
	D3DXMatrixScaling(&S, mScale.x, mScale.y, mScale.z);

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

