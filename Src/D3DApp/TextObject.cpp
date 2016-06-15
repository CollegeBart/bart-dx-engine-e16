#include "TextObject.h"

TextObject::TextObject()
{
	D3DXFONT_DESC fontDesc;
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

TextObject::~TextObject()
{
}

void TextObject::OnLostDevice()
{
	mFont->OnLostDevice();
}

void TextObject::OnResetDevice()
{
	mFont->OnResetDevice();
}

void TextObject::Update()
{
}

void TextObject::Draw(ID3DXSprite * spriteBatch)
{
	RECT fontRect;
	::GetClientRect(gD3DApp->GetMainWindow(), &fontRect);

	HR(mFont->DrawText(0, _T("Hello Direct 3D!!!"), -1, &fontRect, DT_CENTER | DT_VCENTER,
		D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256)));

}
