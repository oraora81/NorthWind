
#pragma once


namespace nt { namespace Memory {

	
inline void* NextAlignedAddress(void* address, ntUchar alignment)
{
	return (void*)( ((ULONG_PTR)address + (alignment - 1)) & ~(alignment - 1) );
}

inline ntUchar AlignAdjustment(void* address, ntUchar alignment)
{
	ntUchar adjustment = alignment - ( (ULONG_PTR)address & (alignment - 1) );

	if (adjustment == alignment)
	{
		return 0;
	}

	return adjustment;
}

inline ntUchar AlignAdjustmentWithHeader(void* address, ntUchar alignment, ntUchar headerSize)
{
	ntUchar adjustment = alignment - ((ULONG_PTR)address & (alignment - 1));

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
	T* AllocateNew()
	{
		return new (Allocate(sizeof(T), __alignof(T))) T;
	}

	template <typename T>
	T* AllocateNew(const T& t)
	{
		return new (Allocate(sizeof(T), __alignof(T))) T(t);
	}

	template <typename T>
	void DeAllocateDelete(T* obj)
	{
		if (obj != nullptr)
		{
			obj->~T();
			DeAllocate(obj);
		}
	}

	template <typename T>
	T* AllocateArray(ntSize length)
	{
		if (length == 0)
		{
			return nullptr;
		}

		ntSize headerSize = sizeof(ntSize) / sizeof(T);
		if ((sizeof(ntSize) % sizeof(T)) > 0)
		{
			headerSize += 1;
		}

		// allocate extra space to store array length in the bytes before the array
		T* ptr = ((T*)Allocate(sizeof(T) * (length + headerSize), __alignof(T))) + headerSize;

		*(((ntUint*)ptr) - 1) = length;

		for (ntSize i = 0; i < length; ++i)
		{
			new (&ptr[i]) T;
		}

		return ptr;

	}

	template <typename T>
	void DeAllocateArray(void* ptr)
	{
		if (ptr == nullptr)
		{
			return;
		}

		ntSize length = *(((ntUint*)ptr) - 1);

		for (ntSize i = 0; i < size; ++i)
		{
			ptr[i].~T();
		}

		// calcuate how much extra memory was allocated to store the length before the array
		ntSize headerSize = sizeof(ntSize) / sizeof(T);
		if ((sizeof(ntSize) % sizeof(T)) > 0)
		{
			headerSize += 1;
		}

		DeAllocate(ptr - headerSize);
	}


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

 
}	// namespace Memory

}	// namespace nt
