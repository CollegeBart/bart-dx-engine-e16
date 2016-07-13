#pragma once

#include "D3DApp.h"
#include "D3DUtils.h"

struct VertexPos
{
	VertexPos() 
		: pos(0.f, 0.f, 0.f) {}
	VertexPos(float x, float y, float z) 
		: pos(x, y, z) {}
	VertexPos(D3DXVECTOR3& v) 
		: pos(v) {}

	D3DXVECTOR3 pos;

	static IDirect3DVertexDeclaration9* decl;
};

struct VertexPosCol
{
	VertexPosCol() 
		: pos(0.f, 0.f, 0.f), col(0x00000000) {}
	VertexPosCol(int r, int g, int b, int a) 
		: pos(0.f, 0.f, 0.f), col(D3DCOLOR_ARGB(a, r, g, b)) {}
	VertexPosCol(float x, float y, float z) 
		: pos(x, y, z), col(0x00000000) {}
	VertexPosCol(float x, float y, float z, D3DCOLOR& c)
		: pos(x, y, z), col(c) {}
	VertexPosCol(D3DXVECTOR3& v, D3DCOLOR& c) 
		: pos(v), col(c) {}

	D3DXVECTOR3 pos;
	D3DCOLOR col;

	static IDirect3DVertexDeclaration9* decl;
};

void InitAllVertexdeclaration();
void DestroyAllVertexDeclaration();
