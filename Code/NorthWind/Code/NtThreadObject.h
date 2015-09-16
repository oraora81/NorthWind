
#pragma once

namespace nt
{

namespace Thread
{

class NtCriticalSecton
{
public:
	NtCriticalSecton(const CRITICAL_SECTION& cs)
	{
		EnterCriticalSection(&cs);
	}

	~NtCriticalSecton()
	{
		LeaveCriticalSection(&cs);
	}
};

} // Thread

} // nt
