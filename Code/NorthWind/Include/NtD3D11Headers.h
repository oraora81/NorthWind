
#include <d3d11.h>
#include <d3dx11.h>
#include <d3d10.h>
#include <d3d10_1.h>
#include <dxgi.h>
#include <d2d1.h>
#include <D2D1Helper.h>
#include <dwrite.h>
#include "dxerr.h"
#include "d3dx11effect.h"


#pragma comment (lib, "d3d11.lib")

#if defined(_DEBUG) || defined(DEBUG)
#pragma comment (lib, "d3dx11d.lib")
#pragma comment (lib, "d3dx10d.lib")
#pragma comment (lib, "Effects11d.lib")
#else
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")
#pragma comment (lib, "d3d10_1.lib")
#pragma comment (lib, "Effects11.lib")
#endif

#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "dwrite.lib")
