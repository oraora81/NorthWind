
#pragma once

namespace NT
{

namespace _thread
{

class NtCriticalSecton
{
public:
	NtCriticalSecton()
	{
		InitializeCriticalSection(&m_cs);
		EnterCriticalSection(&m_cs);
	}

	~NtCriticalSecton()
	{
		LeaveCriticalSection(&m_cs);
	}

private:
	CRITICAL_SECTION m_cs;
};

}

}