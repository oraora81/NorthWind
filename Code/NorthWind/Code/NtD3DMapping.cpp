
#include "NtCoreLib.h"
#pragma hdrstop

#include "NtD3DMapping.h"

namespace NT
{
#ifdef _NT_DX11
	NTCOLORFMT	g_colorFormat[eColorFormat::NT_FMT_MAX] =
	{
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_B8G8R8A8_UNORM,
		DXGI_FORMAT_B8G8R8X8_UNORM,
		DXGI_FORMAT_D16_UNORM,
		DXGI_FORMAT_D24_UNORM_S8_UINT,
	};
#elif _NTDX9
	NTCOLORFMT	g_colorFormat[eColorFormat::NT_FMT_MAX] =
	{
		D3DFMT_UNKNOWN,
		D3DFMT_A8R8G8B8,
		D3DFMT_X8R8G8B8,
		D3DFMT_D16,
		D3DFMT_D24S8,
	};
#endif
}