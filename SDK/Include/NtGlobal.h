
#pragma once

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)

#define __WFILE__ WIDEN(__FILE__)
#define __WFUNCTION__ WIDEN(__FUNCTION__)


#define NtInline inline

#define NtWNextLine L"\r\n"
#define NtNextLine "\r\n"