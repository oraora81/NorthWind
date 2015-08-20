
#pragma once

namespace nt
{

namespace Atom
{
	ntLong Inc(volatile ntLong& target);
	ntLong Dec(volatile ntLong& target);
	ntLong Add(volatile ntLong& target, ntLong val);
	ntLong Sub(volatile ntLong& target, ntLong val);
}	// namespace Atom


class NtMutex
{
public:
	NtMutex();
	~NtMutex();
	
private:
	void Initialize();
	void Release();

private:
#ifdef _WINDOWS
	typedef HANDLE	MUTEX_HANDLE;
#else
	typedef int		MUTEX_HANDLE;
#endif

	MUTEX_HANDLE m_handle;
};

}	// namespace nt
