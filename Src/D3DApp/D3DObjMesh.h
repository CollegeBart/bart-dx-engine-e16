// This header contains stuff to facilitate drawing obj meshes using direct3d.

# ifndef D3D_OBJ_MESH_H
# define D3D_IBJ_MESH_H


#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "ObjLoader.h"
//# include "Win32Util.h"


#ifndef SAFE_DELETE
/// For pointers allocated with new.
#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
/// For arrays allocated with new [].
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);   (p)=NULL; } }
#endif

#ifndef SAFE_RELEASE
/// For use with COM pointers.
#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p)=NULL; } }
#endif

#ifndef MIN
#define MIN(x,y)				( (x)<(y)? (x) : (y) )
#endif

#ifndef MAX
#define MAX(x,y)				( (x)>(y)? (x) : (y) )
#endif



class CD3DMesh
{
public:
	CD3DMesh()
	{
		vertexSize = 0;
		FVF = 0;
		pVB = NULL;
	}

	~CD3DMesh() { SAFE_RELEASE(pVB); }

	HRESULT Create(LPDIRECT3DDEVICE9 pD3DDevice, const TObjMesh& objMesh, BOOL flipTriangles, BOOL flipUVs);


	UINT triCount;
	UINT vertexSize;
	DWORD FVF;
	LPDIRECT3DVERTEXBUFFER9 pVB;
	D3DXVECTOR3 bbmin, bbmax; // bounding box.

protected:
	// This method is probably the most important in the sample after the obj loader.
	// This methdo is called by the Create() method to construct the D3D vertex buffer and fill it
	// with the triangles of the obj mesh. Each triangle has its own copy of the vertices, so it's not
	// really very efficient. Instead, in a real world application, the vertex buffer should only contain
	// unique vertices, and an index buffer is neaded to create the triangles. That is, if many face
	// vertices are identical (in ALL their components), only one corresponding vertex needs to be copied
	// to the vertex buffer.
	// This optimization requires sorting and/or searching which can be quite slow for heavy meshes.
	HRESULT InitVB(LPDIRECT3DDEVICE9 pD3DDevice, const TObjMesh& objMesh, BOOL flipTriangles, BOOL flipUVs);
};



# endif // inclusion guard