
#include "NtCoreLib.h"
#pragma hdrstop

#include "NtConsole.h"


namespace nt { namespace log {

static const ntShort BUFFER_WIDTH = 120;
static const ntShort BUFFER_HEIGHT = 999;

// 
//  NtConsole Implement
//----------------------------------------------------------------------------

nt::fs::NtFile* NtConsole::ms_file = nullptr;
HANDLE NtConsole::ms_handle = nullptr;

bool NtConsole::Initialize(const ntWchar* subject)
{
	AllocConsole();
	
	if (Crt::IsNullOrEmpty(subject) == false)
	{
		SetConsoleTitle(subject);
	}

	ms_handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
	if (INVALID_HANDLE_VALUE == ms_handle)
	{
		NtAsserte("Console Initialize Failed");
		return false;
	}

	COORD coord =
	{
		BUFFER_WIDTH,
		BUFFER_HEIGHT
	};

	//CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	//GetConsoleScreenBufferInfo(ms_handle, &bufferInfo);

	SetConsoleScreenBufferSize(ms_handle, coord);

	return true;
}

void NtConsole::Print(const NtString& str)
{
	if (INVALID_HANDLE_VALUE == ms_handle)
	{
		Initialize(nullptr);
	}

	ntUlong written;
	WriteConsole(ms_handle, str.Buffer(), str.Size(), &written, nullptr);
}

}	// namespace log
}	// namespace nt
