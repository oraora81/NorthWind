
#pragma once


namespace NT
{

namespace Memory
{
	
inline void* NextAlignedAddress(void* address, NtUChar alignment)
{
	return (void*)( ((NtUInt)address + (alignment - 1)) & ~(alignment - 1) );
}

inline NtUChar AlignAdjustment(void* address, NtUChar alignment)
{
	NtUChar adjustment = alignment - ( (NtUInt)address & (alignment - 1) );

	if (adjustment == alignment)
	{
		return 0;
	}

	return adjustment;
}

inline NtUChar AlignAdjustmentWithHeader(void* address, NtUChar alignment, NtUChar headerSize)
{
	NtUChar adjustment = alignment - ((NtUInt)address & (alignment - 1));

	if (adjustment == alignment)
	{
		return 0;
	}

	NtUChar needSize = headerSize;
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

	virtual void* Allocate(NtSize size, NtUChar alignment) = 0;
	virtual void DeAllocate(void* ptr) = 0;

	template <typename T>
	T* AllocateNew();

	template <typename T>
	T* AllocateNew(const T& t);

	template <typename T>
	void DeAllocateDelete(T* obj);

	template <typename T>
	T* AllocateArray(NtSize length);

	template <typename T>
	void DeAllocateArray(void* ptr);


	NtSize		GetUsedMemory() const;
	NtSize		GetAllocatedCount() const;

protected:
	NtSize		m_usedMemory;
	NtSize		m_allocatedCount;
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