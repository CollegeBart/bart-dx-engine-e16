
# include "D3DObjMesh.h"
# include <assert.h>
# include <vector>



// This structure describes a face vertex in an obj mesh. A face vertex simply contains
// indexes to the actual vertex component data in the obj mesh.
struct ObjTriVertex
{
	INT iPos;
	INT iNormal;
	INT iTex;

	void Init() { iPos = iNormal = iTex = -1; }
};


// This structure describes a triangle in the obj mesh. Obj meshes are composed of faces, or polygons.
// Each polygon consists of one or more triangles. Each triangle is made of 3 face vertices.
// See [...] for a description of the difference between a vertex and a face vertex.
struct ObjTriangle
{
	ObjTriVertex vertex[3];
	//INT subsetIndex;

	void Init() { vertex[0].Init(); vertex[1].Init(); vertex[2].Init(); /*subsetIndex = -1;*/ }
};


typedef std::vector< ObjTriangle > ObjTriangleList;



// Adds an OBJ face to the specified triangle list. If the face is not triangular, it is
// triangulated by taking, for the nth vertex, where n starts from 2, the triangle that
// consists of the first vertex, the nth vertex, and the (n-1)th vertex. This triangulation
// method is fast but may yield weird overlapping triangles.
//
VOID AddObjFace(ObjTriangleList& objTriangleList, const TObjMesh& objMesh,
	UINT objFaceIndex, BOOL bFlipTriangles, BOOL bFlipUVs)
{
	const TObjMesh::TFace& objFace = objMesh.faces[objFaceIndex];
	UINT triCount = objFace.vCount - 2;

	for (INT fv = 2; fv < objFace.vCount; fv++)
	{
		ObjTriangle tri;
		tri.Init();

		tri.vertex[0].iPos = objMesh.faceVertices[objFace.firstVertex];
		tri.vertex[1].iPos = objMesh.faceVertices[objFace.firstVertex + fv - 1];
		tri.vertex[2].iPos = objMesh.faceVertices[objFace.firstVertex + fv];
# ifdef DEBUG
		if (tri.vertex[0].iPos >= objMesh.vertices.Size()) DebugBreak();
		if (tri.vertex[1].iPos >= objMesh.vertices.Size()) DebugBreak();
		if (tri.vertex[2].iPos >= objMesh.vertices.Size()) DebugBreak();
# endif


		if (!objMesh.normals.empty() && objFace.firstNormal >= 0)
		{
			tri.vertex[0].iNormal = objMesh.faceNormals[objFace.firstNormal];
			tri.vertex[1].iNormal = objMesh.faceNormals[objFace.firstNormal + fv - 1];
			tri.vertex[2].iNormal = objMesh.faceNormals[objFace.firstNormal + fv];
# ifdef DEBUG
			if (tri.vertex[0].iNormal >= objMesh.normals.Size()) DebugBreak();
			if (tri.vertex[1].iNormal >= objMesh.normals.Size()) DebugBreak();
			if (tri.vertex[2].iNormal >= objMesh.normals.Size()) DebugBreak();
# endif
		}

		if (!objMesh.texCoords.empty() && objFace.firstTexCoord >= 0)
		{
			tri.vertex[0].iTex = objMesh.faceTexCoords[objFace.firstTexCoord];
			tri.vertex[1].iTex = objMesh.faceTexCoords[objFace.firstTexCoord + fv - 1];
			tri.vertex[2].iTex = objMesh.faceTexCoords[objFace.firstTexCoord + fv];
		}

		objTriangleList.push_back(tri);
	}
}





HRESULT CD3DMesh::Create(LPDIRECT3DDEVICE9 pD3DDevice, const TObjMesh& objMesh,
	BOOL bFlipTriangles, BOOL bFlipUVs)
{
	if (objMesh.vertices.empty() || objMesh.numTriangles == 0)
	{
		OutputDebugString(TEXT(__FUNCTION__)TEXT(": obj mesh is invalid!"));
		return E_FAIL;
	}

	// Get bounding box info.
	bbmin = objMesh.bbmin;
	bbmax = objMesh.bbmax;

	return InitVB(pD3DDevice, objMesh, bFlipTriangles, bFlipUVs);
}




// IMPORTANT: See the comment above the method's declaration in the header file.
HRESULT CD3DMesh::InitVB(LPDIRECT3DDEVICE9 pD3DDevice, const TObjMesh& objMesh, BOOL bFlipTriangles, BOOL bFlipUVs)
{
	HRESULT hr;
	SAFE_RELEASE(pVB);

	vertexSize = sizeof(D3DXVECTOR3); // Has at least positional data.
	FVF = D3DFVF_XYZ;

	BOOL hasNormals = TRUE;// We'll compute them when needed. !objMesh.normals.empty();
	BOOL hasTexCoords = !objMesh.texCoords.empty();

	if (hasNormals) { vertexSize += sizeof(D3DXVECTOR3); FVF |= D3DFVF_NORMAL; }
	if (hasTexCoords) { vertexSize += sizeof(D3DXVECTOR2); FVF |= (D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE2(0)); }


	ObjTriangleList triList;
	triList.reserve(objMesh.numTriangles);

	for (UINT i = 0; i<objMesh.faces.size(); i++)
		AddObjFace(triList, objMesh, i, FALSE, FALSE);

	triCount = triList.size();


	struct VBVertex
	{
		D3DXVECTOR3 pos, normal;
		D3DXVECTOR2 tex;
	};

	UINT bufferSize = triCount * vertexSize * 3;
	hr = pD3DDevice->CreateVertexBuffer(bufferSize, D3DUSAGE_WRITEONLY, FVF, D3DPOOL_DEFAULT, &pVB, NULL);
	if (FAILED(hr)) return hr;

	BYTE* pVBData = NULL;
	hr = pVB->Lock(0, 0, (VOID**)&pVBData, 0);
	if (FAILED(hr)) return hr;

	UINT vertexOrder[] = { 0, 1, 2 };
	if (bFlipTriangles)
	{
		vertexOrder[1] = 2; vertexOrder[2] = 1;
	}

	for (UINT i = 0; i<triList.size(); i++)
	{
		ObjTriangle& tri = triList[i];

		// Compute the triangle's normal if the obj mesh does not have normals info.
		D3DXVECTOR3 triNormal;
		if (tri.vertex[0].iNormal < 0)
		{
			D3DXVECTOR3 vec1 = objMesh.vertices[tri.vertex[2].iPos] - objMesh.vertices[tri.vertex[0].iPos];
			D3DXVECTOR3 vec2 = objMesh.vertices[tri.vertex[2].iPos] - objMesh.vertices[tri.vertex[1].iPos];
			if (bFlipTriangles)
				D3DXVec3Cross(&triNormal, &vec2, &vec1);
			else
				D3DXVec3Cross(&triNormal, &vec1, &vec2);
			D3DXVec3Normalize(&triNormal, &triNormal);
		}

		for (UINT tv = 0; tv < 3; tv++)
		{
			UINT v = vertexOrder[tv];

			VBVertex* pVBVertex = (VBVertex*)pVBData;
			pVBVertex->pos = objMesh.vertices[tri.vertex[v].iPos];
			if (tri.vertex[v].iNormal < 0)
				pVBVertex->normal = triNormal;
			else
				pVBVertex->normal = objMesh.normals[tri.vertex[v].iNormal];

			if (hasTexCoords && tri.vertex[v].iTex >= 0)
			{
				if (bFlipUVs)
				{
					D3DXVECTOR2 tex = objMesh.texCoords[tri.vertex[v].iTex];
					tex.y = 1 - tex.y;
					pVBVertex->tex = tex;
				}
				else
					pVBVertex->tex = objMesh.texCoords[tri.vertex[v].iTex];
			}

			pVBData += vertexSize;
		}
	}

	pVB->Unlock();

	return S_OK;
}