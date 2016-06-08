#pragma once
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

#include <iostream>
#include <string>

#include <d3dx9.h>
#include <dxerr.h>

//int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;

#define ReleaseCOM(x)	\
{						\
	if(x)				\
	{					\
		x->Release();	\
		x = nullptr;	\
	}					\
}

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)										\
{													\
	HRESULT hr = x;									\
	if(FAILED(hr))									\
	{												\
		DXTrace(__FILE__, __LINE__, hr, #x, true);	\
	}												\
}
#endif
#else
#ifndef HR
#define HR(x) x;
#endif
#endif