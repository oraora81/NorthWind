#pragma once

#ifdef USE_DIRECTX

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

#endif // USE_DIRECTX
