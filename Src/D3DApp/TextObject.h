#pragma once

#include "Component.h"

class TextObject :
	public Component
{
public:
	TextObject(const char* _fontName,const char* _text);
	TextObject(const char* _fontName, const char* _text, D3DCOLOR _color);
	TextObject(const char* _fontName, const char* _text, D3DCOLOR _color,D3DXVECTOR3 _position);
	~TextObject();

	void OnLostDevice();
	void OnResetDevice();

	void SetText(const char* _txt);
	void SetProperties(int _height);
	void SetProperties(int _height, int _width);
	void SetProperties(int _height, int _width,bool _italic);
	void SetProperties(int _height, int _width, bool _italic,bool _bold);
	void SetProperties(int _height, int _width, bool _italic, bool _bold, D3DCOLOR _color);
	void SetProperties(bool _italic) { fontDesc.Italic = _italic; }
	void SetProperties(D3DCOLOR _color);
	void SetPosition(D3DXVECTOR3 _position);



	void Update();
	void Draw(ID3DXSprite* spriteBatch);

protected:
	D3DXFONT_DESC fontDesc;

private:
	RECT fontRect;
	D3DXVECTOR3 position;
	D3DCOLOR color;
	const char* text;
	TextObject();
	ID3DXFont* mFont;
};

