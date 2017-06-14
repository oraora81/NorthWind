#pragma once

#ifdef USE_DIRECTX

#include <xnamath.h>

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// simple d3d error checker
#if defined(DEBUG) || defined(_DEBUG)
	#ifndef HR
	#define HR(x)                                               \
	{                                                           \
		HRESULT hr = (x);                                       \
		if (FAILED(hr))                                         \
		{                                                       \
			DXTrace(__WFILE__, (DWORD)__LINE__, hr, L"", TRUE);  \
		}                                                       \
	}
	#endif

	#ifndef HRF
	#define HRF(x)                                                  \
		{                                                           \
			HRESULT hr = (x);                                       \
			if (FAILED(hr))                                         \
			{                                                       \
				DXTrace(__WFILE__, (DWORD)__LINE__, hr, L"", TRUE);  \
				return false;                                       \
			}                                                       \
		}
	#endif
#else
	#ifndef HR
	#define HR(x) (x)
	#endif
	#ifndef HRF
	#define HRF(x) (x)
	#endif
#endif


namespace Colors
{
	XMGLOBALCONST XMVECTORF32 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Black = { 0.0f, 0.0f, 0.0f, 0.0f };
	XMGLOBALCONST XMVECTORF32 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
}

#endif // USE_DIRECTX
