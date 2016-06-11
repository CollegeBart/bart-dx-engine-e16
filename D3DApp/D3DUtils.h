
#pragma once
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

#include <iostream>
#include <string>

#include <d3dx9.h>
#include <dxerr.h>

//int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;
#define PI 3.14159265358979323846f
#define DEG_TO_RAD PI / 180.0f
#define RAD_TO_DEG  180.0f / PI
#define RADIAN(x)\
{\
	 x * DEG_TO_RAD\
}

#define DEGREE(y)\
{\
	 y * RAD_TO_DEG\
}

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