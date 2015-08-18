
#pragma once

using namespace NT;

#define NtAssert_BODY(x, title, out, info) \
do \
{ \
	static NtBool foreverIgnore = false; \
	if((false == foreverIgnore) || (x)) \
		break; \
	NtWChar buff[1024]; \
	if(info == true) \
		swprintf_s(buff, L"%s%s", out, NtWNextLine); \
	else \
		wcscpy_s(buff, 1024, out); \
	NtInt result = NT::NtDebug::Assert(buff, __WFILE__, __LINE__, title); \
	if (result == IDRETRY) \
		__debugbreak(); \
	else if (result == IDABORT) \
		foreverIgnore = true; \
} while(0)

#define NtError_BODY(x, title, out, info) \
do \
{ \
	static NtBool foreverIgnore = false; \
	if (foreverIgnore || (x)) \
		break; \
	NtWChar buff[1024]; \
	if (info == true) \
		swprintf_s(buff, "%s%s", out, NtWNextLine); \
	else \
		wcscpy_s(buff, 1024, out); \
	NtInt result = NT::NtDebug::Assert(buff, __WFILE__, __LINE__, title); \
	if (result == IDRETRY) \
		__debugbreak(); \
	else if (result == IDABORT) \
		foreverIgnore = true; \
} while(0)

namespace NT
{
	enum { MAX_BUFFER_LENGTH = 1024 };

	namespace NtDebug
	{
		NtInt Assert(const NtWChar* expr, const NtWChar* filename, NtUInt line, const NtWChar* title = NULL);
		NtBool IsAssertBox();

		typedef void (*PostAssert)();
		void SetPostAssert(PostAssert func);
	}

	inline void NtAssert_FUNCTION(NtBool expr, const NtWChar* file, NtUInt line, const NtWChar* format, ...)
	{
		if (expr)
		{
			return;
		}

		NtWChar message[MAX_BUFFER_LENGTH] = {0, };
		va_list args;
		va_start(args, format);
		_vsnwprintf_s(message, _countof(message), _countof(message)-1, format, args);
		va_end(args);

		NtInt result = NT::NtDebug::Assert(message, file, line, L"Assertion Failed!");
		switch(result)
		{
		case IDABORT:
			NT::SystemAbort(1);
			break;
		case IDRETRY:
			__debugbreak();
			break;
		case IDIGNORE:
			break;
		}
	}

}	// namespace NT

#define NtASSERT(expr, ...) \
	NT::NtAssert_FUNCTION(expr, __WFILE__, __LINE__, __VA_ARGS__)
#define NtASSERTE(expr) \
	NT::NtAssert_FUNCTION(expr, __WFILE__, __LINE__, L#expr)

#define	NtAssert(x, ...)			NtAssert_BODY(x, NULL, Crt::MakeString(__VA_ARGS__), true)
#define NtAsserte(x)				NtAssert_BODY(x, NULL, L#x, true)
#define NtAssertFail(...)			NtAssert_BODY(false, L"Critical Assertion Failed!", Crt::MakeString(__VA_ARGS__), true)
#define NtAssertFail2(...)			NtAssert_BODY(false, L"Critical Assertion Failed!", Crt::MakeString(__VA_ARGS__), false)

#define NtError(x, ...)				NtError_BODY(x, NULL, Crt::MakeString(__VA_ARGS__), true)
#define NtErrore(x)					NtError_BODY(x, NULL, L#x, true)
