#pragma once

#include "D3DUtils.h"
#include "DirectInput.h"
#include "GameTimer.h"
#include "time.h"

class D3DApp
{
public:
	D3DApp();
	D3DApp(
		HINSTANCE hInstance, 
		std::string winCaption);
	virtual ~D3DApp();

	virtual int Run();
	virtual LRESULT MsgProc(
		UINT msg, WPARAM wParam, LPARAM lParam);

	HWND GetMainWindow() const { return mhMainWindow; }

protected:
	virtual void InitMainWindow();
	virtual void InitDirect3D();
	virtual bool CheckDeviceCaps() { return true; }
	virtual void OnResetDevice();
	virtual void OnLostDevice();
	virtual void Update();
	virtual void PreDraw();
	virtual void Draw();
	virtual void PostDraw();

private:
	IDirect3D9* mD3DObject;
	D3DPRESENT_PARAMETERS mD3Dpp;
	
	HWND mhMainWindow;
	HINSTANCE mhAppInstance;
	std::string mMainWindowCaption;
};

extern D3DApp* gD3DApp;
extern 	IDirect3DDevice9* gD3DDevice;

