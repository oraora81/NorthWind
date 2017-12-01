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
    XMGLOBALCONST XMVECTORF32 Silver = { 0.75f, 0.75f , 0.75f, 1.0f };
    XMGLOBALCONST XMVECTORF32 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };

    XMGLOBALCONST XMCOLOR WhiteC = { 1.0f, 1.0f, 1.0f, 1.0f };
    XMGLOBALCONST XMCOLOR BlackC = { 0.0f, 0.0f, 0.0f, 0.0f };
    XMGLOBALCONST XMCOLOR RedC = { 1.0f, 0.0f, 0.0f, 1.0f };
    XMGLOBALCONST XMCOLOR GreenC = { 0.0f, 1.0f, 0.0f, 1.0f };
    XMGLOBALCONST XMCOLOR BlueC = { 0.0f, 0.0f, 1.0f, 1.0f };
    XMGLOBALCONST XMCOLOR YellowC = { 1.0f, 1.0f, 0.0f, 1.0f };
    XMGLOBALCONST XMCOLOR CyanC = { 0.0f, 1.0f, 1.0f, 1.0f };
    XMGLOBALCONST XMCOLOR MagentaC = { 1.0f, 0.0f, 1.0f, 1.0f };

    static UINT ArgbToAbgr(UINT argb)
    {
        BYTE A = (argb >> 24) & 0xff;
        BYTE R = (argb >> 16) & 0xff;
        BYTE G = (argb >> 8) & 0xff;
        BYTE B = (argb >> 0) & 0xff;

        return (A << 24) | (B << 16) | (G << 8) | (R << 0);
    }
}

class NtD3dUtil
{
public:
    static XMMATRIX InverseTranspose(CXMMATRIX M);
};

#endif // USE_DIRECTX
