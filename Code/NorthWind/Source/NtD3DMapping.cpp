
#include "NtCoreLib.h"
#pragma hdrstop

#include "NtD3DMapping.h"

namespace nt {

NTCOLORFMT	g_colorFormat[ColorFormat::NT_FMT_MAX] =
{
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_B8G8R8A8_UNORM,
	DXGI_FORMAT_B8G8R8X8_UNORM,
	DXGI_FORMAT_D16_UNORM,
	DXGI_FORMAT_D24_UNORM_S8_UINT,
};

}
