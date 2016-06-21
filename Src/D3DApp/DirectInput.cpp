//=============================================================================
// DirectInput.cpp by Frank Luna (C) 2005 All Rights Reserved.
//=============================================================================

#include "DirectInput.h"

extern DirectInput* gDInput = 0;

DirectInput::DirectInput(HINSTANCE instance, HWND mainWindow, DWORD keyboardCoopFlags, DWORD mouseCoopFlags)
{
	ZeroMemory(mKeyboardState, sizeof(mKeyboardState));
	ZeroMemory(&mMouseState, sizeof(mMouseState));

	HR(DirectInput8Create(instance, DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (void**)&mDInput, 0));

	HR(mDInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, 0));
	HR(mKeyboard->SetDataFormat(&c_dfDIKeyboard));
	HR(mKeyboard->SetCooperativeLevel(mainWindow, keyboardCoopFlags));
	HR(mKeyboard->Acquire());

	HR(mDInput->CreateDevice(GUID_SysMouse, &mMouse, 0));
	HR(mMouse->SetDataFormat(&c_dfDIMouse2));
	HR(mMouse->SetCooperativeLevel(mainWindow, mouseCoopFlags));
	HR(mMouse->Acquire());
}

DirectInput::~DirectInput()
{
	ReleaseCOM(mDInput);
	mKeyboard->Unacquire();
	mMouse->Unacquire();
	ReleaseCOM(mKeyboard);
	ReleaseCOM(mMouse);
}

void DirectInput::poll()
{
	memcpy(mLastKeyboardState, mKeyboardState, sizeof(mKeyboardState));

	// Poll keyboard.
	HRESULT hr = mKeyboard->GetDeviceState(sizeof(mKeyboardState), (void**)&mKeyboardState); 
	if( FAILED(hr) )
	{
		// Keyboard lost, zero out keyboard data structure.
		ZeroMemory(mKeyboardState, sizeof(mKeyboardState));

		 // Try to acquire for next time we poll.
		hr = mKeyboard->Acquire();
	}

	// Poll mouse.
	hr = mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&mMouseState); 
	if( FAILED(hr) )
	{
		memcpy(mlastMouseButtons, mMouseState.rgbButtons, sizeof(mlastMouseButtons));

		// Mouse lost, zero out mouse data structure.
		ZeroMemory(&mMouseState, sizeof(mMouseState));

		// Try to acquire for next time we poll.
		hr = mMouse->Acquire(); 
	}
	//mMousePos.x += mouseDX();
	//mMousePos.y += mouseDY();
	//mMousePos.z += mouseDZ();
}

bool DirectInput::keyDown(unsigned char key)
{
	return (mKeyboardState[key] & 0x80) != 0;
}

bool DirectInput::keyPressed(unsigned char key)
{
	return (!(mLastKeyboardState[key] & 0x80) != 0) && (mKeyboardState[key] & 0x80) != 0;
}

bool DirectInput::keyReleased(unsigned char key)
{
	return ((mLastKeyboardState[key] & 0x80) != 0) && !((mKeyboardState[key] & 0x80) != 0);
}

bool DirectInput::mouseButtonDown(int button)
{
	return (mMouseState.rgbButtons[button] & 0x80) != 0;
}

bool DirectInput::mouseButtonPressed(int button)
{
	return false;
}

bool DirectInput::mouseButtonReleased(int button)
{
	return (mMouseState.rgbButtons[button] & 0x80) != 1;
}

float DirectInput::mouseDX()
{
	return (float)mMouseState.lX;
}

float DirectInput::mouseDY()
{
	return (float)mMouseState.lY;
}

float DirectInput::mouseDZ()
{
	return (float)mMouseState.lZ;
}