#include "TextObject.h"

TextObject::TextObject() :
	Component()
	, text(""), color(0), position(0, 0, 0)
	, fontRect(), str(new char[5])
{
	::GetClientRect(gD3DApp->GetMainWindow(), &fontRect);

	fontDesc.Height = 80;
	fontDesc.Width = 40;
	fontDesc.MipLevels = 0;
	fontDesc.Italic = true;
	fontDesc.CharSet = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality = DEFAULT_QUALITY;
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s(fontDesc.FaceName, _T("Times New Roman"));

	HR(D3DXCreateFontIndirect(gD3DDevice, &fontDesc, &mFont));
}

TextObject::TextObject(const char * _fontName, const char * _text) :
	Component()
	, text(_text), color(0), position(0, 0, 0)
	, fontRect(), str(new char[5])
{
	::GetClientRect(gD3DApp->GetMainWindow(), &fontRect);

	fontDesc.Height = 80;
	fontDesc.Width = 40;
	fontDesc.Weight = FW_DONTCARE;
	fontDesc.MipLevels = 0;
	fontDesc.Italic = true;
	fontDesc.CharSet = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality = DEFAULT_QUALITY;
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s(fontDesc.FaceName, _T(_fontName));

	HR(D3DXCreateFontIndirect(gD3DDevice, &fontDesc, &mFont));
}

TextObject::TextObject(const char * _fontName, const char * _text, D3DCOLOR  _color) :
	Component()
	, text(_text), color(_color), position(0, 0, 0)
	, fontRect(), str(new char[5])
{
	::GetClientRect(gD3DApp->GetMainWindow(), &fontRect);

	fontDesc.Weight = FW_DONTCARE;
	fontDesc.Height = 80;
	fontDesc.Width = 40;
	fontDesc.MipLevels = 0;
	fontDesc.Italic = false;
	fontDesc.CharSet = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality = DEFAULT_QUALITY;
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s(fontDesc.FaceName, _T(_fontName));

	HR(D3DXCreateFontIndirect(gD3DDevice, &fontDesc, &mFont));
}

TextObject::TextObject(const char * _fontName, const char * _text, D3DCOLOR _color, D3DXVECTOR3 _position) :
	Component()
	, text(_text), color(_color), position(_position)
	, fontRect() , str(new char[5])
{
	//Ne gere pas les paragraphe
	fontDesc.Weight = FW_DONTCARE;
	fontDesc.Height = 80;
	fontDesc.Width = 40;
	fontDesc.MipLevels = 0;
	fontDesc.Italic = false;
	fontDesc.CharSet = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality = DEFAULT_QUALITY;
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s(fontDesc.FaceName, _T(_fontName));

	HR(D3DXCreateFontIndirect(gD3DDevice, &fontDesc, &mFont));

	fontRect.top = _position.y;
	fontRect.bottom = _position.y + fontDesc.Height;
	fontRect.left = _position.x;
	fontRect.right = _position.x + GetTextWidth(_text, mFont);
}

TextObject::TextObject(const char * _fontName, const char * _text, D3DCOLOR _color, D3DXVECTOR3 _position, int _height) :
	Component()
	, text(_text), color(_color), position(_position)
	, fontRect(), str(new char[5])
{
	//Ne gere pas les paragraphe
	fontDesc.Weight = FW_DONTCARE;
	fontDesc.Height = _height;
	fontDesc.Width = (int)(_height * 0.5f);
	fontDesc.MipLevels = 0;
	fontDesc.Italic = true;
	fontDesc.CharSet = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality = DEFAULT_QUALITY;
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s(fontDesc.FaceName, _T(_fontName));

	fontRect.top = _position.y - fontDesc.Height;
	fontRect.bottom = _position.y;
	fontRect.left = _position.x;
	fontRect.right = _position.x + ((strlen(_text) + 1)*fontDesc.Width);

	HR(D3DXCreateFontIndirect(gD3DDevice, &fontDesc, &mFont));
}


TextObject::~TextObject()
{
	delete str;
	delete mFont;
}

void TextObject::OnLostDevice()
{
	mFont->OnLostDevice();
}

void TextObject::OnResetDevice()
{
	mFont->OnResetDevice();
}

void TextObject::SetText(char * _txt)
{
	text = _txt;
}

void TextObject::SetText(int _txt)
{
	std::sprintf(str, "%d", _txt);
	text = str;
}

void TextObject::SetProperties(int _height)
{
	fontDesc.Height = _height;
}

void TextObject::SetProperties(int _height, int _width)
{
	fontDesc.Height = _height;
	fontDesc.Width = _width;
}

void TextObject::SetProperties(int _height, int _width, bool _italic)
{
	fontDesc.Height = _height;
	fontDesc.Width = _width;
	fontDesc.Italic = _italic;
}

void TextObject::SetProperties(int _height, int _width, bool _italic, bool _bold)
{
	fontDesc.Height = _height;
	fontDesc.Width = _width;
	fontDesc.Italic = _italic;
	if (_bold)
		fontDesc.Weight = FW_BOLD;
	else
		fontDesc.Weight = FW_NORMAL;
}

void TextObject::SetProperties(int _height, int _width, bool _italic, bool _bold, D3DCOLOR _color)
{
	fontDesc.Height = _height;
	fontDesc.Width = _width;
	fontDesc.Italic = _italic;
	color = _color;
	if (_bold)
		fontDesc.Weight = FW_BOLD;
}

void TextObject::SetProperties(D3DCOLOR _color)
{
	color = _color;
}

void TextObject::SetPosition(D3DXVECTOR3 _position)
{
	fontRect.top = _position.y;
	fontRect.bottom = _position.y + fontDesc.Height;
	fontRect.left = _position.x;
	fontRect.right = _position.x + ((strlen(text) + 1) * fontDesc.Width);
}

void TextObject::SetTranslationText(D3DXVECTOR3 _position)
{
	position = _position;
	SetPosition(position);
}

void TextObject::Update()
{
}

void TextObject::Draw(ID3DXSprite* spriteBatch, const D3DXMATRIX& view, const D3DXMATRIX& proj)
{
	HR(mFont->DrawText(0, _T(text), -1, &fontRect, DT_LEFT | DT_TOP, color));
}