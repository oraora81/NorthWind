
#include "NtCoreLib.h"

namespace NT
{
namespace Atom
{
	NtLong Inc(volatile NtLong& target)
	{
		return InterlockedIncrement(&target);
	}

	NtLong Dec(volatile NtLong& target)
	{
		return InterlockedDecrement(&target);
	}

	NtLong Add(volatile NtLong& target,NtLong val)
	{
		return InterlockedExchangeAdd(&target, val);
	}

	NtLong Sub(volatile NtLong& target,NtLong val)
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
