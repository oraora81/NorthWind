
#include "NtCoreLib.h"

namespace nt { namespace ntDebug {


static postAssert callbackFunc = NULL;
__declspec(thread) static ntUint countAssertCall = 0;


void SetPostAssert(postAssert func)
{
	callbackFunc = func;
}

const ntWchar* NtMakeAssertMessage(const ntWchar* expr, const ntWchar* filename, const ntUint line)
{
	const ntUint bufferSize = sizeof(ntWchar) * MAX_ASSERT_BUFFER_LENGTH;

	ntWchar* buffPtr = Crt::MakeWBuffer();
	ntWchar buff[MAX_ASSERT_BUFFER_LENGTH] = { 0, };

	Crt::MemSet(buffPtr, bufferSize);

	swprintf_s(buff, MAX_ASSERT_BUFFER_LENGTH, L"%s%sfile : %s%sline : %d%s", expr, NtWNextLine, filename, NtWNextLine, line, NtWNextLine);
	Crt::StrCat(buffPtr, MAX_ASSERT_BUFFER_LENGTH, buff);

	swprintf_s(buff, MAX_ASSERT_BUFFER_LENGTH, NtWNextLine
		L"Abort : forever Ignore\n"
		L"Retry : call Debugger\n"
		L"Ignore : once Ignore\n\n");
	Crt::StrCat(buffPtr, MAX_ASSERT_BUFFER_LENGTH, buff);

	return buffPtr;
}
		
ntInt Assert(const ntWchar* expr, const ntWchar* filename, ntUint line, const ntWchar* title /* = NULL */)
{
	struct sAssertBoxCounter
	{
		sAssertBoxCounter() { Atom::Inc((ntLong&)countAssertCall); }
	};

	// for client log
	sAssertBoxCounter boxBounter;

	HWND parent = ::GetActiveWindow();
	if (NULL == parent)
	{
		parent = ::GetLastActivePopup(parent);
	}

	MSG msg;
	ntInt isQuit = ::PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);

	const ntWchar * message = NtMakeAssertMessage(expr, filename, line);
	ntUint windowType = MB_TASKMODAL
				| MB_ICONHAND
				| MB_ABORTRETRYIGNORE
				| MB_SETFOREGROUND
				| MB_TOPMOST;

	ntInt code = ::MessageBox(parent, message, title ? title : L"Assertion Failed!", windowType);
	if (callbackFunc)
	{
		(*callbackFunc)();
	}

	if (isQuit)
	{
		nt::SystemAbort((ntInt)msg.wParam);
	}

	return code;
}

bool IsAssertBox()
{
	return countAssertCall > 0;
}

}	// namespace ntDebug
}	// namespace nt
