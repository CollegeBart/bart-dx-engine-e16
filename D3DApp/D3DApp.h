#pragma once

#include "D3DUtils.h"

class D3DApp
{
public:
	D3DApp();
	D3DApp(
		HINSTANCE hInstance, 
		std::string winCaption);
	~D3DApp();

	virtual int Run();
	virtual LRESULT MsgProc(
		UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	virtual void InitMainWindow();
	virtual void InitDirect3D();
	virtual bool CheckDeviceCaps() { return true; }
	virtual void OnResetDevice();
	virtual void OnLostDevice();
	virtual void Update();
	virtual void Draw();

private:
	IDirect3D9* mD3DObject;
	D3DPRESENT_PARAMETERS mD3Dpp;
	
	HWND mhMainWindow;
	HINSTANCE mhAppInstance;
	std::string mMainWindowCaption;

	ID3DXSprite* mSpriteBatch;
	IDirect3DTexture9* mTexture;
	D3DXVECTOR3 mCenter;
	D3DXVECTOR3 mPosition;

};

extern D3DApp* gD3DApp;
extern 	IDirect3DDevice9* gD3DDevice;

