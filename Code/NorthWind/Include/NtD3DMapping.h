
#pragma once

namespace nt {

#ifdef _NT_DX11
extern NTCOLORFMT	g_colorFormat[eColorFormat::NT_FMT_MAX];
#elif _NT_DX9
extern NTCOLORFMT	g_colorFormat[eColorFormat::NT_FMT_MAX];
#endif

}
