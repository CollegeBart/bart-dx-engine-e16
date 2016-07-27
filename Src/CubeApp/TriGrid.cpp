#include "TriGrid.h"

TriGrid::TriGrid()
	: width(100)
	, height(100)
	, tileW(1.0f)
	, rotation(0.f)
{
	BuildVertexBuffer();
	BuildIndexBuffer();
}

TriGrid::TriGrid(int w, int h, int dx)
	: width(w)
	, height(h)
	, tileW(dx)
	, rotation(0.f)
{
	BuildVertexBuffer();
	BuildIndexBuffer();

	HR(D3DXCreateEffectFromFile(gD3DDevice, "Transform.fx", 0, 0, D3DXSHADER_DEBUG, 0, &mFX, &mErrors));
	if (mErrors)
	{
		MessageBox(0, (char*)mErrors->GetBufferPointer(), 0, 0);
	}
	
	mhTech = mFX->GetTechniqueByName("TransformColTech");
	mFX->SetTechnique(mhTech);

	mhWVP = mFX->GetParameterByName(0, "gWVP");
	mhTime = mFX->GetParameterByName(0, "gTime");
	mhEyePos = mFX->GetParameterByName(0, "gEye");
	mhFogColor = mFX->GetParameterByName(0, "gFogColor");
	mhMinDist = mFX->GetParameterByName(0, "gMinDist");
	mhMaxDist = mFX->GetParameterByName(0, "gMaxDist");

}

TriGrid::~TriGrid()
{
	ReleaseCOM(mFX);
	ReleaseCOM(mErrors);
}

void TriGrid::Update()
{
	rotation += 1.0f * gTimer->GetDeltaTime();
}

void TriGrid::BuildVertexBuffer()
{
	HR(gD3DDevice->CreateVertexBuffer(GetNumVertices() * sizeof(VertexPosCol),
		D3DUSAGE_WRITEONLY, 0,
		D3DPOOL_MANAGED, &mVB, 0));

	VertexPosCol* v = 0;
	HR(mVB->Lock(0, 0, (void**)&v, 0));

	float w = (width - 1) * tileW;
	float d = (height - 1) * tileW;

	float halfWidth = -w * 0.5f;
	float halfHeight = d * 0.5f;

	int k = 0;
	for (float i = 0; i < width; i++)
	{
		for (float j = 0; j < height; j++)
		{
			float x = j * tileW + halfWidth;
			float z = -i * tileW + halfHeight;
			float y = GetHeight(x, z);
	
			v[k].pos = D3DXVECTOR3(x, y, z);
			v[k].col = D3DCOLOR_XRGB(0, 255, 0);
			++k;
		}
	}
	
	HR(mVB->Unlock());
}

void TriGrid::BuildIndexBuffer()
{
	HR(gD3DDevice->CreateIndexBuffer(GetNumFaces() * 3 * sizeof(WORD), 
		D3DUSAGE_WRITEONLY,	D3DFMT_INDEX16, D3DPOOL_MANAGED, &mIB, 0));

	WORD* indices = 0;
	HR(mIB->Lock(0, 0, (void**)&indices, 0));

	int k = 0;
	for (int i = 0; i < width-1; i++)
	{
		for (int j = 0; j < height-1; j++)
		{
			indices[k]		=    i    * height + j;
			indices[k + 1]	=    i    * height + j + 1;
			indices[k + 2]	= (i + 1) * height + j;
			indices[k + 3]	= (i + 1) * height + j;
			indices[k + 4]	=    i    * height + j + 1;
			indices[k + 5]	= (i + 1) * height + j + 1;
			k += 6; // Next quad
		}
	}

	HR(mIB->Unlock());
}

void TriGrid::Draw(ID3DXSprite * spriteBatch, const D3DXMATRIX & view, const D3DXMATRIX & proj)
{
	HR(gD3DDevice->SetStreamSource(0, mVB, 0, sizeof(VertexPosCol)));
	HR(gD3DDevice->SetIndices(mIB));
	HR(gD3DDevice->SetVertexDeclaration(VertexPosCol::decl));

	//D3DXMatrixTranslation(&T, -2.0f, -2.0f, 0.0f);
	//D3DXMatrixRotationYawPitchRoll(&R, rotation, rotation, 0.0f);
	//D3DXMatrixScaling(&S, 2.0f, 2.0f, 2.0f);
	D3DXMATRIX WVP = view * proj;

	D3DXVECTOR3 eye = gEngine->GetCameraPos();

	HR(mFX->SetMatrix(mhWVP, &WVP));
	HR(mFX->SetFloat(mhMinDist, 75.0f));
	HR(mFX->SetFloat(mhMaxDist, 160.0f));
	HR(mFX->SetFloat(mhTime, gTimer->GetGameTime()));
	HR(mFX->SetValue(mhEyePos, eye, sizeof(D3DXVECTOR3)));
	HR(mFX->SetVector(mhFogColor, &D3DXVECTOR4(1.0f, 0.5f, 0.5f, 1.0f)));
	
	UINT numPasses = 0;
	mFX->Begin(&numPasses, 0);
	
	for (int i = 0; i < numPasses; i++)
	{
		mFX->BeginPass(i);
	
		//gD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 6);
		HR(gD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, GetNumVertices(), 0, GetNumFaces()));
	
		mFX->EndPass();
	}
	
	mFX->End();
}
