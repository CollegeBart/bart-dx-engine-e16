// Functions and structures to read .obj files.
// If you use this, or if you find any bugs or make enhancements or corrections,
// share your findings on the blog post's comments section.
// -Adel Amro (http://code-section.com)



# ifndef OBJ_LOADER_H
# define OBJ_LOADER_H


# include <windows.h>
# include <vector>





// A structure containing information about a material loaded from an .mtl file.
// TODO: Use dynamically allocated strings for map names.
struct TObjMaterial
{
	char name[MAX_PATH]; // Material name.
	float Ka[3];	// Ambient.
	float Kd[3];	// Diffuse (color).
	float Ks[3];	// Specular color.
	float Tf[3];	// Transmission filter.
	float Tr;		// Transparency (also called d).
	float Ns;		// Specular power (shininess).
	float Ni;		// Index of refraction (optical density).
	int illum;		// Illumination model (See remark at bottom of file).
	char map_Ka[MAX_PATH]; // Ambient texture file name.
	char map_Kd[MAX_PATH]; // Diffuse texture file name.
	char map_Ks[MAX_PATH]; // Specular color texture file name.
	char map_Ns[MAX_PATH]; // Specular power texture file name.
	char map_Tr[MAX_PATH]; // Transparency texture file name.
	char map_Disp[MAX_PATH]; // Displacement map.
	char map_Bump[MAX_PATH]; // Bump map.
	char map_Refl[MAX_PATH]; // Reflection map.


	TObjMaterial()
	{
		// Set material defaults (using mtl file specs).
		strncpy_s(name, "default", MAX_PATH);
		Ka[0] = 0.2f; Ka[1] = 0.2f; Ka[2] = 0.2f;
		Kd[0] = 0.8f; Kd[1] = 0.8f; Kd[2] = 0.8f;
		//Ks[0] = 1.0f; Ks[1] = 1.0f; Ks[2] = 1.0f;
		Ks[0] = 0.f; Ks[1] = 0.f; Ks[2] = 0.f;
		Tf[0] = 1.0f; Tf[1] = 1.0f; Tf[2] = 1.0f;
		Tr = 1.f; // Fully opaque.
				  //Ns = 0.f;
		Ns = 32.f;
		Ni = 1.f;
		illum = 2; // No default specified (?).
		map_Ka[0] = 0;
		map_Kd[0] = 0;
		map_Ks[0] = 0;
		map_Ns[0] = 0;
		map_Tr[0] = 0;
		map_Disp[0] = 0;
		map_Bump[0] = 0;
		map_Refl[0] = 0;
	}
};





struct TObjMesh
{
# ifdef __D3DX9_H__
	typedef D3DXVECTOR3 TFloat3;
	typedef D3DXVECTOR2 TFloat2;
# else
	struct TFloat3 { float x, y, z; };
	struct TFloat2 { float x, y; };
# endif

	struct TFace
	{
		INT firstVertex;	// Index into the mesh's vertex array - defines the first vertex of this face.
		INT firstTexCoord;	// Like above, but can be -1 to specify face has no tex coords.
		INT firstNormal;	// ~~
		INT vCount;			// Number of vertices in this face.
	};

	struct TGroup
	{
		UINT firstFace;
		UINT numFaces;
		char name[MAX_PATH];
	};


	std::vector< TFloat3 >		vertices;
	std::vector< TFloat3 >		normals;
	std::vector< TFloat2 >		texCoords;
	std::vector< TFace >		faces;

	std::vector< INT >			faceVertices;
	std::vector< INT >			faceNormals;
	std::vector< INT >			faceTexCoords;

	std::vector< TGroup >		groups;
	std::vector< TGroup >		matGroups;

	std::vector< TObjMaterial* > materials;

	UINT					numTriangles; // Differs from faces.size() if the mesh is not triangulated.
	TFloat3					bbmin; // Bounding box minimum values.
	TFloat3					bbmax; // Bounding box maximum values.

	char sMtlFileName[MAX_PATH];	// .mtl file name. We store it in a char array (rather than TCHAR) because
									// it's read from the obj file, and obj files are ascii.

	TObjMesh() { numTriangles = 0; }

	void Free()
	{
		vertices.clear();
		normals.clear();
		texCoords.clear();
		faces.clear();
		matGroups.clear();
		faceVertices.clear();
		faceNormals.clear();
		numTriangles = 0;
		for (UINT i = 0; i<materials.size(); i++)
			delete materials[i];
		materials.clear();
	}
};







// Loads an .mtl file. This function is primarily used by LoadObj(), but is exposed here
// in case it's otherwise needed.
// Returns 0 on failure, 1 on success.
INT LoadMtlLib(LPCTSTR sFileName, std::vector<TObjMaterial*>& materials);







// Loads an Obj file. Returns 1 on success, 0 on failure. Returns 2 if the obj
// file was loaded but the associated mtl file was not found.
INT LoadObj(LPCTSTR sFileName, TObjMesh* pOutObjMesh);





/////////////////////
// Remark 1: Meaning of the illum member of the material structure.
// From a document titled "Alias/WaveFront Material (.mtl) File Format" which
// was seemingly written in 1995 by Diane Ramey, Linda Rose, and Lisa Tyerman,
// the following semantics are defined.
//
// 0 Color on and Ambient off 
// 1 Color on and Ambient on 
// 2 Highlight on 
// 3 Reflection on and Ray trace on 
// 4 Transparency: Glass on, Reflection: Ray trace on 
// 5 Reflection: Fresnel on and Ray trace on 
// 6 Transparency: Refraction on, Reflection: Fresnel off and Ray trace on 
// 7 Transparency: Refraction on, Reflection: Fresnel on and Ray trace on 
// 8 Reflection on and Ray trace off 
// 9 Transparency: Glass on, Reflection: Ray trace off 
// 10 Casts shadows onto invisible surfaces
//
// The DirectX SDK says that illum can be either 1 or 2 with the meanings:
// (1: disable specular, 2: enable specular).
// This interpretation is a subset of the above definition.



// Remark 2:
// The obj file specification says that the vertex indices for a face are defined
// using the following format:
// f v/t/n v/t/n v/t/n
// However, tex coords and normals are optional. So we have:
// f v//n v//n  ... <-- when there are no tex coords
// f v/t/ v/t/  ... <-- When there are no normals
// f v// v//    ... <-- When there are neither.

// However, in reality there are many obj exporters of famous apps
// that don't follow this convention. They follow the following convention:
// f v v v			.. <-- face with no tex coords or normals
// f v/t v/t v/t	.. <-- face with tex coords but no normals
// f v//n v//n v//n .. <-- face with normals but no tex coords.

// This importer handles both scenarios.

# endif // inclusion guard