#include "Vertex.h"

IDirect3DVertexDeclaration9* VertexPos::decl = 0;
IDirect3DVertexDeclaration9* VertexPosCol::decl = 0;

void InitAllVertexdeclaration()
{
	// VertexPos
	D3DVERTEXELEMENT9 vertexPosElements[] = {
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		D3DDECL_END()
	};
	HR(gD3DDevice->CreateVertexDeclaration(vertexPosElements, &VertexPos::decl));

	// VertexPosCol
	D3DVERTEXELEMENT9 vertexPosColElements[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		D3DDECL_END()
	};
	HR(gD3DDevice->CreateVertexDeclaration(vertexPosColElements, &VertexPosCol::decl));
}

void DestroyAllVertexDeclaration()
{
	ReleaseCOM(VertexPos::decl);
	ReleaseCOM(VertexPosCol::decl);
}
