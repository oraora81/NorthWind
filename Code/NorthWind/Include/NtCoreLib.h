
#pragma once

#define _WIN32_WINNT 0x0600

#pragma warning( disable : 4127 )
#pragma warning( disable : 4838 )
#pragma warning( disable : 4100 )

// 
// Common Library
//----------------------------------------------------------------------------
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <ostream>
#include <sstream>
#include <cwchar>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <vector>
#include <list>
#include <map>
#include <limits>


// 
// Need for WIN32 macro Check.
//----------------------------------------------------------------------------
#include <Windows.h>
#include <windowsx.h>

// 
// d3d
//----------------------------------------------------------------------------
#include <D3Dcompiler.h>
#include <NtD3D11Headers.h>
#include <NtD3dUtil.h>
#include <xnamath.h>
#include <dinput.h>

// 
// NT Library
//----------------------------------------------------------------------------
#include <NtType.h>
#include <NtErrorHandling.h>
#include <NtGlobal.h>
#include <NtCrt.h>
#include <NtAtom.h>
#include <NtSystem.h>
#include <NtAssert.h>
#include <NtTypeManip.h>
#include <NtTypeTrait.h>
#include <NtMath.h>

#include <NtSetup.h>
#include <NtContainer.h>
#include <NtLog.h>
#include <NtException.h>
#include <NtInitDelegator.h>
#include <NtString.h>
#include <NtFactory.h>
#include <NtRTTI.h>
#include <NtObject.h>
#include <NtAllocator.h>
#include <NtLinearAllocator.h>
#include <NtBuffer.h>
#include <NtFile.h>

// render
#include <NtRenderDefine.h>
#include <NtRenderType.h>
#include <NtShader.h>
#include <NtResourceManager.h>
#include <NtApplication.h>

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
//
//
#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) && !defined(DXGI_1_2_FORMATS)
//#define DXGI_1_2_FORMATS
#endif
