#pragma once
#include "Component.h"
#include <iostream>

class GameObject : public Component
{
public:
	// Crée une GameObject et appel le constructeur de Component
	// le GameObject est donc ajouter au vector de Component
	GameObject();
	GameObject(const char* path);
	GameObject(const char* path, float startX, float startY);

	~GameObject();

	void OnLostDevice();
	void OnResetDevice();

	void Start();
	void Update();
	void Draw(ID3DXSprite* spriteBatch);
};

