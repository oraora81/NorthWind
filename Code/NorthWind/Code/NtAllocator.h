
#pragma once


namespace NT
{

namespace Memory
{
	
inline void* NextAlignedAddress(void* address, ntUchar alignment)
{
	return (void*)( ((ntUint)address + (alignment - 1)) & ~(alignment - 1) );
}

inline ntUchar AlignAdjustment(void* address, ntUchar alignment)
{
	ntUchar adjustment = alignment - ( (ntUint)address & (alignment - 1) );

	if (adjustment == alignment)
	{
		return 0;
	}

	return adjustment;
}

inline ntUchar AlignAdjustmentWithHeader(void* address, ntUchar alignment, ntUchar headerSize)
{
	ntUchar adjustment = alignment - ((ntUint)address & (alignment - 1));

	if (adjustment == alignment)
	{
		return 0;
	}

	ntUchar needSize = headerSize;
	if (adjustment < needSize)
	{
		needSize -= adjustment;

		// increase adjustment to fit header
		adjustment += alignment * (needSize / alignment);
		if ( (needSize % alignment) > 0 )
		{
			adjustment += alignment;
		}
	}

	return adjustment;
}


#define MAKE_MEGA2BYTES(num) \
	1024*1024*num

#define MB2INBYTES  2097152 //2mb


//
class NtAllocator : public NtObject
{
protected:
	NtAllocator()
		: m_usedMemory(0)
		, m_allocatedCount(0)
	{

	}

public:
	virtual ~NtAllocator()
	{
		NtAsserte( m_usedMemory == 0 && m_allocatedCount == 0 );
	}

	virtual void* Allocate(ntSize size, ntUchar alignment) = 0;
	virtual void DeAllocate(void* ptr) = 0;

	template <typename T>
	T* AllocateNew();

	template <typename T>
	T* AllocateNew(const T& t);

	template <typename T>
	void DeAllocateDelete(T* obj);

	template <typename T>
	T* AllocateArray(ntSize length);

	template <typename T>
	void DeAllocateArray(void* ptr);


	ntSize		GetUsedMemory() const;
	ntSize		GetAllocatedCount() const;

protected:
	ntSize		m_usedMemory;
	ntSize		m_allocatedCount;
};

	
template <typename T>
bool isHeap(T& obj)
{
	const void* caster = dynamic_cast<const void*>(obj);
	if (nullptr == caster)
	{
		return false;
	}

	return true;
}

#include "NtAllocator.inl"

}	// namespace Memory

}	// namespace NT