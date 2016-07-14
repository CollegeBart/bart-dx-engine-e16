// Implementation file for the functions in ObjLoader.h.


# include "ObjLoader.h"
# include <stdio.h> // For std file routines
# include <tchar.h>


# define LINE_BUFF_SIZE 4096





///////////////
// Utility function used internally by this module.
// Used by LoadMtlLib to read Ka, Kd, Ks, and Tf.
bool LoadMtl_ReadKx(const char* line, float* kx)
{
	// Specs say that y and z components are optional, and if they're not there
	// then they're equal to x.
	if (0 == _stricmp("spectral", line + 3)) return false; // Not supported.
	else if (0 == _stricmp("xyz", line + 3)) return false; // Not supported.
	else if (3 != sscanf(line + 3, "%f %f %f", &kx[0], &kx[1], &kx[2]))
		kx[1] = kx[2] = kx[0];
	return true;
}


bool IsCharNumber(char c) { return c >= '0' && c <= '9'; }



// Returns the number of numbers in the string.
int CountNumbers(const char* s)
{
	const char* s2 = s;
	while (IsCharNumber(*s2))
		s2++;
	return s2 - s;
}




// Used with face definition statement lines in an obj file. The line has the following format:
// "f [vertex] [vertex]..."
// where [vertex] defines the indices of a vertex component in that face. It has the following format:
// "v/t/n"
// where v is the vertex position index, t is an optional texture coordinate index, and n is an optional normal index.
// This function is used to inspect one [vertex] definition to determine if the texture index and normal index exist.
bool InspectVertexDefinition(const char* sVertexDef, bool& hasNormals, bool& hasTexCoords)
{
	hasNormals = false;
	hasTexCoords = false;

	const char* s = sVertexDef;
	// Skip the vertex position component.
	int len = CountNumbers(s);
	if (len == 0)
		return false; // There is no vertex index component either!!??
	s += len;

	if (*s != '/' || *(s + 1) == 0)
		return true;
	s++;

	// Now move on to check for tex coords info.
	len = CountNumbers(s);
	if (len > 0)
	{
		hasTexCoords = true;
		s += len;
	}

	if (*s != '/' || *(s + 1) == 0)
		return true;

	s++;

	// Now check for normal info.
	len = CountNumbers(s);
	if (len > 0)
		hasNormals = true;

	return true;
}





///////////////////
// Utility function used internally by this module.
// This function inspects the contents of a face definition line in an obj file.
// That's a line which starts with the "f " sequence and defines a polygon.
// If the inspectVertexComponents is set to TRUE, the line is processed to figure out
// whether it contains texture coordinates and normals information.
// TODO: This may be redundant - the existance of arrays for vertex normals and texture
// coordinates could be a sure indication of whether this information is also specified
// for each face or not.
void InspectFaceLine(const char* sLine, int& vCount,
	bool inspectVertexComponents, bool& hasTexCoords, bool& hasNormals)
{
	int spaceCount = 0;

	// We determine the number of vertices by counting the spaces. Before each
	// vertex definition, there is always a space. Some exporters write an extra space
	// at the end of the line, so we'll skip that.
	for (const char* s = sLine + 1; *s != 0; s++)
	{
		if (*s != ' ')
			continue;

		//if( *(s+1) == 0 )
		//	continue;
		if (!IsCharNumber(*(s + 1)))
			continue;

		spaceCount++;
		if (spaceCount != 1)
			continue;

		// Ok, this is the first space. Let's examine the vertex info that comes next to determine
		// which vertex components the face contains.
		if (inspectVertexComponents)
			InspectVertexDefinition(s + 1, hasNormals, hasTexCoords);
	}
	vCount = spaceCount;
}









// Loads an .mtl file. This function is primarily used by LoadObj().
// Returns 0 on failure, 1 on success.
INT LoadMtlLib(LPCTSTR sFileName, std::vector<TObjMaterial*>& materials)
{
	CHAR buffer[LINE_BUFF_SIZE];

	FILE* pFile = _tfopen(sFileName, TEXT("r"));

	if (!pFile) return 0;

	TObjMaterial* pMat = NULL;

	while (!feof(pFile))
	{
		buffer[0] = 0;
		fgets(buffer, LINE_BUFF_SIZE, pFile);
		if (0 == strncmp("newmtl ", buffer, 7))
		{
			pMat = new TObjMaterial;
			materials.push_back(pMat);
			sscanf(buffer + 7, "%s", pMat->name);
		}
		else if (pMat == NULL)
			continue; // Skip anything until we find a newmtl statement.
		else if (0 == _strnicmp("ka ", buffer, 3))		LoadMtl_ReadKx(buffer, pMat->Ka);
		else if (0 == _strnicmp("ks ", buffer, 3))		LoadMtl_ReadKx(buffer, pMat->Ks);
		else if (0 == _strnicmp("kd ", buffer, 3))		LoadMtl_ReadKx(buffer, pMat->Kd);
		else if (0 == _strnicmp("tf ", buffer, 3))		LoadMtl_ReadKx(buffer, pMat->Tf);
		else if (0 == _strnicmp("tr ", buffer, 3))		pMat->Tr = (float)atof(buffer + 3);
		else if (0 == _strnicmp("d ", buffer, 2))		pMat->Tr = (float)atof(buffer + 2);
		else if (0 == _strnicmp("ns ", buffer, 3))		pMat->Ns = (float)atof(buffer + 3);
		else if (0 == _strnicmp("Ni ", buffer, 3))		pMat->Ni = (float)atof(buffer + 3);
		else if (0 == _strnicmp("illum ", buffer, 6))	pMat->illum = atoi(buffer + 6);

		else if (0 == _strnicmp("map_Ka ", buffer, 7))	sscanf(buffer + 7, "%s", pMat->map_Ka);
		else if (0 == _strnicmp("map_Kd ", buffer, 7))	sscanf(buffer + 7, "%s", pMat->map_Kd);
		else if (0 == _strnicmp("map_Ks ", buffer, 7))	sscanf(buffer + 7, "%s", pMat->map_Ks);
		else if (0 == _strnicmp("map_Ns ", buffer, 7))	sscanf(buffer + 7, "%s", pMat->map_Ns);
		else if (0 == _strnicmp("map_Tr ", buffer, 7))	sscanf(buffer + 7, "%s", pMat->map_Tr);
		else if (0 == _strnicmp("map_Disp ", buffer, 7))	sscanf(buffer + 9, "%s", pMat->map_Disp);
		else if (0 == _strnicmp("map_Bump ", buffer, 7))	sscanf(buffer + 9, "%s", pMat->map_Bump);
		else if (0 == _strnicmp("map_Refl ", buffer, 7))	sscanf(buffer + 9, "%s", pMat->map_Refl);
	}
	fclose(pFile);
	return 1;
}








// Returns the index designating the end of the portion of the specified file name
// that defines the file path. Returns -1 if it fails.
INT PathFromFileName(LPCTSTR sFileName)
{
	INT ret = -2;
	INT i = 0;
	LPCTSTR s = sFileName;
	while (*s)
	{
		if (*s == TEXT('\\') || *s == TEXT('/'))
			ret = i;
		i++;
		s++;
	}
	return ret + 1;
}






// Loads an Obj file. Returns 1 on success, 0 on failure. Returns 2 if the obj
// file was loaded but the associated mtl file was not found.
INT LoadObj(LPCTSTR sFileName, TObjMesh* pOutObjMesh)
{
	CHAR buffer[LINE_BUFF_SIZE];

	TObjMesh& obj = *pOutObjMesh;

	obj.Free();

	FILE* pFile = _tfopen(sFileName, TEXT("r"));
	if (!pFile) return 0;


	int numVertices = 0;
	int numNormals = 0;
	int numTexCoords = 0;
	int numFaces = 0;
	int numObjects = 0;
	int numGroups = 0;
	int numMatGroups = 0;
	int numFaceVertices = 0;
	int numFaceNormals = 0;
	int numFaceTexCoords = 0;

	pOutObjMesh->sMtlFileName[0] = 0;

	bool hasTexCoords = false, hasNormals = false;

	// We scan the file with two passes to determine the number of elements to Allocate.
	// Stupid obj file design.
	while (!feof(pFile))
	{
		buffer[0] = 0;
		fgets(buffer, LINE_BUFF_SIZE, pFile);

		if (0 == strncmp("v ", buffer, 2))			numVertices++;
		else if (0 == strncmp("vn ", buffer, 3))		numNormals++;
		else if (0 == strncmp("vt ", buffer, 3))		numTexCoords++;
		else if (0 == strncmp("f ", buffer, 2))
		{
			// TODO: 'fo' (face outline) is seemingly an old flag equivalent to f (face).
			// Consider adding support for that too.
			numFaces++;
			int vCount = 0;
			InspectFaceLine(buffer, vCount, numFaces == 1, hasTexCoords, hasNormals);

			numFaceVertices += vCount;
			if (hasNormals) numFaceNormals += vCount;
			if (hasTexCoords) numFaceTexCoords += vCount;

			obj.numTriangles += vCount - 2;
		}
		else if (0 == strncmp("o ", buffer, 2))			numObjects++;
		else if (0 == _strnicmp("usemtl ", buffer, 7))	numMatGroups++;
		else if (0 == strncmp("g ", buffer, 2))
		{
			// The 'g' statement can include more than one group.
			for (const char* s = buffer; *s; s++)
				if (*s == ' ')
					numGroups++;
		}
		else if (0 == _strnicmp("mtllib ", buffer, 7))
			sscanf(buffer + 7, "%s", pOutObjMesh->sMtlFileName);
	}

	if (numVertices == 0 || numFaces == 0)
	{
		fclose(pFile); return 0;
	} // Failure.


	obj.vertices.resize(numVertices);
	obj.normals.resize(numNormals);
	obj.texCoords.resize(numTexCoords);
	obj.faces.resize(numFaces);

	obj.faceVertices.resize(numFaceVertices);
	obj.faceNormals.resize(numFaceNormals);
	obj.faceTexCoords.resize(numFaceTexCoords);

	obj.groups.resize(numGroups);
	obj.matGroups.resize(numMatGroups);

	rewind(pFile);

	UINT vc = 0, nc = 0, tc = 0, fc = 0;
	UINT fvc = 0, fnc = 0, ftc = 0;
	UINT mc = 0; // material counter.
	UINT gc = 0; // Group counter.
	UINT ag = 0; // Number of groups on last encountered 'g ' line.

	while (!feof(pFile))
	{
		buffer[0] = '\0';
		fgets(buffer, LINE_BUFF_SIZE, pFile);

		if (0 == strncmp("v ", buffer, 2))
		{
			// Vertex
			TObjMesh::TFloat3& v = obj.vertices[vc++];
			sscanf(buffer + 1, "%f %f %f", &v.x, &v.y, &v.z);
			if (vc == 1)
				obj.bbmin = obj.bbmax = v;
			else
			{
				if (v.x < obj.bbmin.x) obj.bbmin.x = v.x; else if (v.x > obj.bbmax.x) obj.bbmax.x = v.x;
				if (v.y < obj.bbmin.y) obj.bbmin.y = v.y; else if (v.y > obj.bbmax.y) obj.bbmax.y = v.y;
				if (v.z < obj.bbmin.z) obj.bbmin.z = v.z; else if (v.z > obj.bbmax.z) obj.bbmax.z = v.z;
			}
		}
		else if (0 == strncmp("vn ", buffer, 3))
		{
			// Normal
			sscanf(buffer + 2, "%f %f %f",
				&obj.normals[nc].x, &obj.normals[nc].y, &obj.normals[nc].z);
			nc++;
		}
		else if (0 == strncmp("vt ", buffer, 3))
		{
			// Texture coordinate
			sscanf(buffer + 2, "%f %f",
				&obj.texCoords[tc].x, &obj.texCoords[tc].y);
			tc++;
		}
		else if (0 == strncmp("f ", buffer, 2))
		{
			// Face. See remark at the bottom of the file to see why the somewhat
			// crpytic interpretation of an 'f ' line in the source file.

			TObjMesh::TFace& face = obj.faces[fc];

			InspectFaceLine(buffer, face.vCount, false, hasTexCoords, hasNormals);

			face.firstVertex = fvc;
			face.firstNormal = hasNormals ? fnc : -1;
			face.firstTexCoord = hasTexCoords ? ftc : -1;


			const char* s = buffer;
			for (int i = 0; i<face.vCount; i++)
			{
				int v = -1, t = -1, n = -1;
				while (*s != ' ')
					s++;
				s++;

				// NOTE: Negative indices are relative.

				v = atoi(s); // or sscanf( s, "%d", &v );
				if (v < 0) v = vc + v + 1;
				obj.faceVertices[fvc] = v - 1; // NOTE: This is to make the indices 0-based.
				fvc++;

				if (hasTexCoords || hasNormals)
				{
					while (*s != '/')
						s++;
					s++;
					if (hasTexCoords)
					{
						t = atoi(s);
						if (t < 0) t = tc + t + 2;
						obj.faceTexCoords[ftc] = t - 1;
						ftc++;
					}
				}
				if (hasNormals)
				{
					while (*s != '/')
						s++;
					s++;
					n = atoi(s);
					if (n < 0) n = nc + n + 2;
					obj.faceNormals[fnc] = n - 1;
					fnc++;
				}
			}
			fc++;
		}
		else if (0 == _strnicmp("usemtl ", buffer, 7))
		{
			obj.matGroups[mc].firstFace = fc;
			obj.matGroups[mc].name[0] = 0;
			//strncpy( obj.matGroups[ mc ].name, buffer + 7, sizeof(obj.matGroups[0].name) );
			sscanf(buffer + 7, "%s", obj.matGroups[mc].name);
			obj.matGroups[mc].numFaces = 0;
			if (mc > 0)
				obj.matGroups[mc - 1].numFaces = fc - obj.matGroups[mc - 1].firstFace;
			mc++;
		}
		else if (0 == strncmp("g ", buffer, 2))
		{
			// The 'g' statement can include more than one group, in which case all that
			// follows belong to all groups on that line.
			if (gc > 0)
				for (UINT j = ag; j>0; j--)
					obj.groups[gc - j].numFaces = fc - obj.groups[gc - j].firstFace;
			ag = 0;
			for (const char* s = buffer; *s; s++)
			{
				if (*s == ' ')
				{
					// TODO: sscanf() might not be the best solution to read strings.
					sscanf(s + 1, "%s", obj.groups[gc + ag].name);
					obj.groups[gc + ag].firstFace = fc;
					obj.groups[gc + ag].numFaces = 0;
					ag++;
				}
			}
			gc += ag;
		}
	}

	// Calculate face count for last defined material.
	if (mc > 0)
		obj.matGroups[mc - 1].numFaces = fc - obj.matGroups[mc - 1].firstFace;

	// Calculate face count for groups defined in last 'g ' statement.
	if (gc > 0)
		for (UINT j = ag; j>0; j--)
			obj.groups[gc - j].numFaces = fc - obj.groups[gc - j].firstFace;

	fclose(pFile);



	// Now load mtl file.
	if (obj.sMtlFileName == NULL || obj.sMtlFileName[0] == 0)
		return 1;

# ifndef UNICODE
	LPCTSTR sMtlFileName = obj.sMtlFileName;
# else
	TCHAR sMtlFileName[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, obj.sMtlFileName, -1, sMtlFileName, MAX_PATH);
# endif
	if (0 >= LoadMtlLib(sMtlFileName, obj.materials))
	{
		//g_Log( "Failed to load mtl file %s", sMtlFileName );
		TCHAR libFile[MAX_PATH];
		INT n = PathFromFileName(sFileName);
		if (n < 0) return 2;

		lstrcpyn(libFile, sFileName, n + 1);
		lstrcpyn(libFile + n, sMtlFileName, MAX_PATH - n);
		if (0 >= LoadMtlLib(libFile, obj.materials))
			return 2; // Failed to load mtl file.
	}
	return 1; // Success
}