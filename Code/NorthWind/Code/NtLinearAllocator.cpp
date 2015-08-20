
#include "NtCoreLib.h"
#pragma hdrstop

namespace nt
{
namespace Memory
{

NT_IMPLEMENT_OBJECT(NtLinearAllocator, NtAllocator)
NT_IMPLEMENT_FACTORY(NtLinearAllocator)
NT_REGISTER_FACTORY(NtLinearAllocator)

NtLinearAllocator::NtLinearAllocator()
: NtAllocator()
, m_initialPosition(nullptr)
, m_currentPosition(nullptr)
, m_totalSize(0)
{

}

NtLinearAllocator::~NtLinearAllocator()
{
	SAFE_DELETE(m_initialPosition);
}

bool NtLinearAllocator::Initialize(ntSize baseAllocSize)
{
	if (baseAllocSize < 1)
	{
		return false;
	}

	m_initialPosition = new ntUchar*[MAKE_MEGA2BYTES(2)];
	m_currentPosition = m_initialPosition;
	m_totalSize = MAKE_MEGA2BYTES(2);

	return true;
}


void* NtLinearAllocator::Allocate(ntSize size, ntUchar alignment)
{
	NtAsserte(size != 0);

	ntUchar adjustment = AlignAdjustment(m_currentPosition, alignment);
	if ( (m_usedMemory + adjustment + size) > m_totalSize )
	{
		return nullptr;
	}

	ntUint alignedAddress = (ntUint)((ntUint)m_currentPosition + adjustment);

	m_currentPosition = reinterpret_cast<void*>(alignedAddress + size);

	m_usedMemory += (size + adjustment);
	++m_allocatedCount;

	return reinterpret_cast<void*>(alignedAddress);
}

void NtLinearAllocator::DeAllocate(void* ptr)
{
	NtAsserte(false && "please Use Reset() Function");
}


void NtLinearAllocator::Reset()
{
	m_allocatedCount = 0;
	m_usedMemory = 0;
	m_currentPosition = m_initialPosition;
}

}	// namespace Memory
}	// namespace nt
