#pragma once

#include "Component.h"

class TriGrid :
	public Component
{
public:
	TriGrid(int w, int h, int dx);
	~TriGrid();

	void Update();

protected:
	int GetNumFaces() { return (width - 1) * (height - 1) * 2; }
	int GetNumVertices(){ return width * height; }

	void BuildVertexBuffer();
	void BuildIndexBuffer();

	void Draw(ID3DXSprite* spriteBatch, const D3DXMATRIX& view, const D3DXMATRIX& proj);

private:
	TriGrid();

	//float GetHeight(float x, float z)
	//{
	//	return (0.3f * (z*sinf(0.1f * x) + x * cosf(0.1f * z)));
	//}

	float tileW;
	int width, height;
	float rotation;

	IDirect3DVertexBuffer9* mVB;
	IDirect3DIndexBuffer9* mIB;

	ID3DXEffect* mFX;
	ID3DXBuffer* mErrors;

	D3DXHANDLE mhTech;
	D3DXHANDLE mhWVP;

	//D3DXHANDLE mhTime;
	//D3DXHANDLE mhEyePos;
	//D3DXHANDLE mhFogColor;

};

