
#pragma once

namespace nt
{

namespace Thread
{

class NtCriticalSecton
{
public:
	NtCriticalSecton(const CRITICAL_SECTION& cs)
		: m_cs(cs)
	{
		EnterCriticalSection(&m_cs);
	}

	~NtCriticalSecton()
	{
		LeaveCriticalSection(&m_cs);
		DeleteCriticalSection(&m_cs);
	}

private:
	CRITICAL_SECTION m_cs;
};

} // Thread

} // nt
