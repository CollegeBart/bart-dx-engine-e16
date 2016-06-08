#include "Component.h"

std::vector<Component*> Component::components;

Component::Component()
	: mTexture(nullptr)
	, mCenter(0.f, 0.f, 0.f)
	, mPosition(-600.f, -450.f, 0.f)
{
	// Ajoute l'instance au vector de Component*
	components.push_back(this);
}

Component::~Component()
{
}

