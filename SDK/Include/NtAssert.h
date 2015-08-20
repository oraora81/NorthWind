
#pragma once

#define NtAssert_BODY(x, title, out, info) \
do \
{ \
	static bool foreverIgnore = false; \
	if((false == foreverIgnore) || (x)) \
		break; \
	ntWchar buff[1024]; \
	if(info == true) \
		swprintf_s(buff, L"%s%s", out, NtWNextLine); \
	else \
		wcscpy_s(buff, 1024, out); \
	ntInt result = nt::ntDebug::Assert(buff, __WFILE__, __LINE__, title); \
	if (result == IDRETRY) \
		__debugbreak(); \
	else if (result == IDABORT) \
		foreverIgnore = true; \
} while(0)

#define NtError_BODY(x, title, out, info) \
do \
{ \
	static bool foreverIgnore = false; \
	if (foreverIgnore || (x)) \
		break; \
	ntWchar buff[1024]; \
	if (info == true) \
		swprintf_s(buff, "%s%s", out, NtWNextLine); \
	else \
		wcscpy_s(buff, 1024, out); \
	ntInt result = nt::ntDebug::Assert(buff, __WFILE__, __LINE__, title); \
	if (result == IDRETRY) \
		__debugbreak(); \
	else if (result == IDABORT) \
		foreverIgnore = true; \
} while(0)

namespace nt
{
enum { MAX_BUFFER_LENGTH = 1024 };

namespace ntDebug
{
	ntInt Assert(const ntWchar* expr, const ntWchar* filename, ntUint line, const ntWchar* title = NULL);
	bool IsAssertBox();

	typedef void (*PostAssert)();
	void SetPostAssert(PostAssert func);
}	// namespace ntDebug

inline void NtAssert_FUNCTION(bool expr, const ntWchar* file, ntUint line, const ntWchar* format, ...)
{
	if (expr)
	{
		return;
	}

	ntWchar message[MAX_BUFFER_LENGTH] = {0, };
	va_list args;
	va_start(args, format);
	_vsnwprintf_s(message, _countof(message), _countof(message)-1, format, args);
	va_end(args);

	ntInt result = ntDebug::Assert(message, file, line, L"Assertion Failed!");
	switch (result)
	{
	case IDABORT:
		nt::SystemAbort(1);
		break;
	case IDRETRY:
		__debugbreak();
		break;
	case IDIGNORE:
		break;
	}
}

}	// namespace nt

#define NtASSERT(expr, ...) \
	nt::NtAssert_FUNCTION(expr, __WFILE__, __LINE__, __VA_ARGS__)
#define NtASSERTE(expr) \
	nt::NtAssert_FUNCTION(expr, __WFILE__, __LINE__, L#expr)

#define	NtAssert(x, ...)			NtAssert_BODY(x, NULL, nt::Crt::MakeString(__VA_ARGS__), true)
#define NtAsserte(x)				NtAssert_BODY(x, NULL, L#x, true)
#define NtAssertFail(...)			NtAssert_BODY(false, L"Critical Assertion Failed!", nt::Crt::MakeString(__VA_ARGS__), true)
#define NtAssertFail2(...)			NtAssert_BODY(false, L"Critical Assertion Failed!", nt::Crt::MakeString(__VA_ARGS__), false)

#define NtError(x, ...)				NtError_BODY(x, NULL, nt::Crt::MakeString(__VA_ARGS__), true)
#define NtErrore(x)					NtError_BODY(x, NULL, L#x, true)
