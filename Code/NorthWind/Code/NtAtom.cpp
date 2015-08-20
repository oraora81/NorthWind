
#include "NtCoreLib.h"

namespace nt
{

namespace Atom
{
	ntLong Inc(volatile ntLong& target)
	{
		return InterlockedIncrement(&target);
	}

	ntLong Dec(volatile ntLong& target)
	{
		return InterlockedDecrement(&target);
	}

	ntLong Add(volatile ntLong& target,ntLong val)
	{
		return InterlockedExchangeAdd(&target, val);
	}

	ntLong Sub(volatile ntLong& target,ntLong val)
	{
		return InterlockedExchangeAdd(&target, -val);
	}
}

//----------------------------------------------------------------------------

NtMutex::NtMutex()
	: m_handle(nullptr)
{
	Initialize();
}

NtMutex::~NtMutex()
{
	Release();
}


void NtMutex::Initialize()
{
	m_handle = ::CreateMutex(NULL, TRUE, NULL);
}

void NtMutex::Release()
{
	if (m_handle != nullptr)
	{
		::ReleaseMutex(m_handle);
		::CloseHandle(m_handle);
	}
}


}
