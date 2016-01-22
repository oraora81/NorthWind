
#include "NtCoreLib.h"
#pragma hdrstop

namespace nt { namespace LOG {

#ifdef _DEBUG
int DebugTrace(const ntWchar* msg, ...)
{
	static ntWchar buff[1024];
	Crt::MemSet(buff, sizeof(buff));

	va_list argptr;
	va_start(argptr, msg);
	int ret = _vsnwprintf_s(buff, _countof(buff), _TRUNCATE, msg, argptr);
	assert(ret != -1);
	va_end(argptr);

	OutputDebugStringW(buff);

	return ret;
}

int DebugTrace( const ntChar* msg, ... )
{
	static ntChar buff[1024];
	Crt::MemSet(buff, sizeof(buff));

	va_list argptr;
	va_start(argptr, msg);
	int ret = _vsnprintf_s(buff, _countof(buff), _TRUNCATE, msg, argptr);
	assert(ret != -1);
	va_end(argptr);

	OutputDebugStringA(buff);

	return ret;
}

#endif



//
//
//----------------------------------------------------------------------------
NtLog::NtLog()
	: m_hashTable(DEFAULT_SUBJECT_COUNT)
	, m_supportThread(false)
{
	
}

void NtLog::AddSubject(ntWchar* subject, LogType flag)
{
	if (Crt::IsNullOrEmpty(subject) == true)
	{
		return;
	}

	sLogItem** item = m_hashTable.Find(subject);
	if (*item == nullptr)
	{
		sLogItem* unit = (sLogItem*)g_liAllocator->Allocate(sizeof(sLogItem));
		unit->subject = subject;
		unit->logType = flag;
	}
	else
	{
		//*item->logType = flag;
	}
}

void NtLog::Print(const ntWchar* subject, const ntWchar* format, ...)
{
	UNREFERENCED_PARAMETER(subject);
	UNREFERENCED_PARAMETER(format);

	if (m_supportThread)
	{
		// making the log string

		// output
	}
}


}	// namespace LOG

}	// namespace nt
