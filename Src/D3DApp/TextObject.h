#pragma once
#include "Component.h"

	class TextObject :
	public Component
{
public:
	TextObject(const char* _fontName, const char* _text);
	TextObject(const char* _fontName, const char* _text, D3DCOLOR _color);
	TextObject(const char* _fontName, const char* _text, D3DCOLOR _color, D3DXVECTOR3 _position);
	TextObject(const char * _fontName, const char * _text, D3DCOLOR _color, D3DXVECTOR3 _position, int _height);
	virtual ~TextObject();

	void OnLostDevice();
	void OnResetDevice();

	void SetText(char* _txt);
	void SetText(int _txt);
	void SetProperties(int _height);
	void SetProperties(int _height, int _width);
	void SetProperties(int _height, int _width, bool _italic);
	void SetProperties(int _height, int _width, bool _italic, bool _bold);
	void SetProperties(int _height, int _width, bool _italic, bool _bold, D3DCOLOR _color);
	void SetProperties(bool _italic) { fontDesc.Italic = _italic; }
	void SetProperties(D3DCOLOR _color);
	void SetPosition(D3DXVECTOR3 _position);
	//Used to move the textobject on the screen.
	void SetTranslationText(D3DXVECTOR3 _position);
	D3DXVECTOR3 GetTranslationText() const { return position; }



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

	char* str;
};