
#include "NtCoreLib.h"
#pragma hdrstop

namespace NT
{

namespace LOG
{

#ifdef _DEBUG
int DebugTrace(const NtWChar* msg, ...)
{
	NtWChar buff[1024];
	va_list argptr;
	va_start(argptr, msg);
	int ret = _vsnwprintf_s(buff, _countof(buff), _TRUNCATE, msg, argptr);
	assert(ret != -1);
	va_end(argptr);

	OutputDebugStringW(buff);

	return ret;
}

int DebugTrace( const NtChar* msg, ... )
{
	NtChar buff[1024];

	va_list argptr;
	va_start(argptr, msg);
	int ret = _vsnprintf_s(buff, _countof(buff), _TRUNCATE, msg, argptr);
	assert(ret != -1);
	va_end(argptr);

	OutputDebugStringA(buff);

	return ret;
}

#endif


/*

 */
NtLOGConsole::NtLOGConsole()
: m_bufferIndex(0)
{
	Crt::MemSet(m_textBuffer, sizeof(NtWChar*) * MAX_BUFFER);
}

NtLOGConsole::~NtLOGConsole()
{

}

//
//
NtLOGFile::NtLOGFile()
{

}

NtLOGFile::~NtLOGFile()
{

}


//
//
NtLOG::NtLOG()
{

}

NtLOG::~NtLOG()
{

}

}	// namespace LOG

}	// namespace NT