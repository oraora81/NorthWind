
#pragma once

namespace NT
{
namespace Atom
{
	NtLong Inc(volatile NtLong& target);
	NtLong Dec(volatile NtLong& target);
	NtLong Add(volatile NtLong& target, NtLong val);
	NtLong Sub(volatile NtLong& target, NtLong val);
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

}	// namespace NT
