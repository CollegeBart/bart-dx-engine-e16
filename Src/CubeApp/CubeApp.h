#pragma once

#include "D3DApp.h"

class CubeApp :
	public D3DApp
{
public:
	CubeApp();
	CubeApp(HINSTANCE hInstance, std::string winCaption);
	~CubeApp();

protected:
	virtual void OnResetDevice()	{}
	virtual void OnLostDevice()		{}
	virtual void Update()			{}
	virtual void Draw();

private:
	IDirect3DVertexBuffer9* mVB;

};

