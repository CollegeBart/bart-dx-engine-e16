#pragma once

#include "Component.h"

class TextObject :
	public Component
{
public:
	TextObject();
	~TextObject();

	void Update();
	void Draw(ID3DXSprite* spriteBatch);

private:
	ID3DXFont* mFont;
};

