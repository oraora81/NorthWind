
#include "NtCoreLib.h"

namespace NT
{
	namespace NtDebug
	{
		static PostAssert	callbackFunc = NULL;
		__declspec(thread) static NtUInt countAssertCall = 0;

		void SetPostAssert(PostAssert func)
		{
			callbackFunc = func;
		}

		const NtWChar* NtMakeAssertMessage(const NtWChar* expr, const NtWChar* filename, const NtUInt line)
		{
			const NtUInt bufferSize = sizeof(NtWChar) * MAX_BUFFER_LENGTH;

			NtWChar* buffPtr = Crt::MakeWBuffer();
			NtWChar buff[MAX_BUFFER_LENGTH] = {0, };

			Crt::MemSet(buffPtr, bufferSize);

			swprintf_s(buff, MAX_BUFFER_LENGTH, L"%s%sfile : %s%sline : %d%s", expr, NtWNextLine, filename, NtWNextLine, line, NtWNextLine);
			Crt::StrCat(buffPtr, MAX_BUFFER_LENGTH, buff);

			swprintf_s(buff, MAX_BUFFER_LENGTH, NtWNextLine
				L"Abort : forever Ignore\n"
				L"Retry : call Debugger\n"
				L"Ignore : once Ignore\n\n");
			Crt::StrCat(buffPtr, MAX_BUFFER_LENGTH, buff);

			return buffPtr;
		}
		
		NtInt Assert(const NtWChar* expr, const NtWChar* filename, NtUInt line, const NtWChar* title /* = NULL */)
		{
			struct sAssertBoxCounter
			{
				sAssertBoxCounter() { Atom::Inc((NtLong&)countAssertCall); }
			};

			// for client log
			sAssertBoxCounter boxBounter;

			HWND parent = ::GetActiveWindow();
			if (NULL == parent)
			{
				parent = ::GetLastActivePopup(parent);
			}

			MSG msg;
			NtInt isQuit = ::PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);

			const NtWChar * message = NtMakeAssertMessage(expr, filename, line);
			NtUInt windowType = MB_TASKMODAL
						| MB_ICONHAND
						| MB_ABORTRETRYIGNORE
						| MB_SETFOREGROUND
						| MB_TOPMOST;

			NtInt code = ::MessageBox(parent, message, title ? title : L"Assertion Failed!", windowType);
			if (callbackFunc)
			{
				(*callbackFunc)();
			}

			if (isQuit)
			{
				NT::SystemAbort((NtInt)msg.wParam);
			}

			return code;
		}

		NtBool IsAssertBox()
		{
			return countAssertCall > 0;
		}
	}	// namespace NtDebug
}	// namespace NT