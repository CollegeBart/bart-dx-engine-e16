#include "D3DApp.h"

extern D3DApp* gD3DApp = 0;
extern 	IDirect3DDevice9* gD3DDevice = 0;

LRESULT CALLBACK MainWndProc(
	HWND hwnd, UINT msg, 
	WPARAM wParam, LPARAM lParam)
{
	if (gD3DApp != 0)
	{
		return gD3DApp->MsgProc(msg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

D3DApp::D3DApp()
	: mhAppInstance(nullptr)
	, mhMainWindow(nullptr)
	, mMainWindowCaption("")
	, mSpriteBatch(nullptr)
	, mTexture(nullptr)
	, mCenter(0.f, 0.f, 0.f)
	, mPosition(0.f, 0.f, 0.f)
{
}

D3DApp::D3DApp(HINSTANCE hInstance, std::string winCaption)
	: mhAppInstance(hInstance)
	, mhMainWindow(nullptr)
	, mMainWindowCaption(winCaption)
	, mSpriteBatch(nullptr)
	, mTexture(nullptr)
	, mCenter(0.f, 0.f, 0.f)
	, mPosition(-392.f, -281.f, 0.f)
{
	InitMainWindow();
	InitDirect3D();

	HR(D3DXCreateSprite(gD3DDevice, &mSpriteBatch));
	
	// TODO - D3DXCreateTextureFromFileEx
	HR(D3DXCreateTextureFromFile(gD3DDevice, "hey-girl-sloth.jpg", &mTexture));

	D3DXMATRIX view;
	D3DXMATRIX orth;
	D3DXVECTOR3 pos(0.f, 0.f, -100.0f);
	D3DXVECTOR3 up(0.f, 1.f, 0.f);
	D3DXVECTOR3 target(0.f, 0.f, 0.f);

	D3DXMatrixLookAtLH(&view, &pos, &target, &up);
	D3DXMatrixOrthoLH(
		&orth, (float)mD3Dpp.BackBufferWidth, (float)mD3Dpp.BackBufferHeight,
		1.0f, 5000.0f);

	HR(gD3DDevice->SetRenderState(D3DRS_LIGHTING, false));
	HR(gD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE));

	HR(gD3DDevice->SetTransform(D3DTS_VIEW, &view));
	HR(gD3DDevice->SetTransform(D3DTS_PROJECTION, &orth));
}

D3DApp::~D3DApp()
{
	ReleaseCOM(mD3DObject);
	ReleaseCOM(gD3DDevice);
}

int D3DApp::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update();
			Draw();
		}
	}
	return (int)msg.wParam;
}

void D3DApp::InitMainWindow()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;	
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mhAppInstance;
	wc.hIcon = ::LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
	wc.lpszMenuName = 0;
	wc.lpszClassName = _T("Hello");

	if (!::RegisterClass(&wc))
	{
		::MessageBox(0, _T("Register class - Failed"), 0, MB_OK);
		PostQuitMessage(0);
	}

	RECT r = { 0, 0, 800, 600 };

	mhMainWindow = CreateWindow(
		wc.lpszClassName,
		mMainWindowCaption.c_str(),
		WS_OVERLAPPEDWINDOW,
		r.left,
		r.top,
		r.right,
		r.bottom,
		0, 0, mhAppInstance, 0);

	if (!mhMainWindow)
	{
		::MessageBox(0, _T("Create window - Failed"), 0, MB_OK);
		PostQuitMessage(0);
	}

	::ShowWindow(mhMainWindow, SW_SHOW);
	::UpdateWindow(mhMainWindow);
}

void D3DApp::InitDirect3D()
{
	// Step 1
	mD3DObject = Direct3DCreate9(D3D_SDK_VERSION);
	if (!mD3DObject)
	{
		::MessageBox(0, _T("Direct3DCreate9 - Failed"), 0, MB_OK);
		PostQuitMessage(0);
	}

	// Step 2
	D3DDISPLAYMODE mode;
	HR(mD3DObject->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode));
	HR(mD3DObject->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mode.Format, mode.Format, true));
	HR(mD3DObject->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mode.Format, D3DFMT_X8R8G8B8, false));

	// Step 3
	D3DCAPS9 caps;
	HR(mD3DObject->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps));
	DWORD devBehaviourFlags = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		devBehaviourFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		devBehaviourFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	if (caps.DevCaps & D3DDEVCAPS_PUREDEVICE && devBehaviourFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
	{
		devBehaviourFlags |= D3DCREATE_PUREDEVICE;
	}

	// Step 4
	mD3Dpp.BackBufferWidth = 0;
	mD3Dpp.BackBufferHeight = 0;
	mD3Dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	mD3Dpp.BackBufferCount = 1;
	mD3Dpp.MultiSampleQuality = 0;
	mD3Dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	mD3Dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	mD3Dpp.hDeviceWindow = mhMainWindow;
	mD3Dpp.Windowed = true;
	mD3Dpp.EnableAutoDepthStencil = true;
	mD3Dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	mD3Dpp.Flags = 0;
	mD3Dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	mD3Dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Step 5
	HR(mD3DObject->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		mhMainWindow, devBehaviourFlags, &mD3Dpp, &gD3DDevice));
}

void D3DApp::OnResetDevice()
{
}

void D3DApp::OnLostDevice()
{
}

void D3DApp::Update()
{
}

void D3DApp::Draw()
{
	D3DXMATRIX S;
	D3DXMatrixScaling(&S, 1.f, -1.0f, 1.0f);

	HR(gD3DDevice->Clear(
		0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
		D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0));

	HR(gD3DDevice->BeginScene());
	{
		HR(mSpriteBatch->Begin(
			D3DXSPRITE_ALPHABLEND  |
			D3DXSPRITE_OBJECTSPACE |
			D3DXSPRITE_DONOTMODIFY_RENDERSTATE));
		{
			HR(mSpriteBatch->SetTransform(&S));
			HR(mSpriteBatch->Draw(mTexture, 0,
				&mCenter, &mPosition, D3DCOLOR_XRGB(255, 255, 255)));
		}
		HR(mSpriteBatch->End());
	}
	HR(gD3DDevice->EndScene());

	HR(gD3DDevice->Present(0, 0, 0, 0));
}

LRESULT D3DApp::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(mhMainWindow);
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(mhMainWindow);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return ::DefWindowProc(mhMainWindow, msg, wParam, lParam);
	}
}
