

template <typename T>
T* NtAllocator::AllocateNew()
{
	return new (Allocate(sizeof(T), __alignof(T))) T;
}

template <typename T>
T* NtAllocator::AllocateNew(const T& t)
{
	return new (Allocate(sizeof(T), __alignof(T))) T(t);
}

template <typename T>
void NtAllocator::DeAllocateDelete(T* obj)
{
	if ( obj != nullptr )
	{
		obj->~T();
		DeAllocate(obj);
	}
}

template <typename T>
T* NtAllocator::AllocateArray(NtSize length)
{
	if (length == 0)
	{
		return nullptr;
	}

	NtSize headerSize = sizeof(NtSize) / sizeof(T);
	if ( (sizeof(NtSize) % sizeof(T)) > 0 )
	{
		headerSize += 1;
	}

	// allocate extra space to store array length in the bytes before the array
	T* ptr = ( (T*)Allocate(sizeof(T) * (length + headerSize), __alignof(T)) ) + headerSize;

	*( ((NtUInt*)ptr) - 1 ) = length;

	for (NtSize i = 0; i < length; ++i)
	{
		new (&ptr[i]) T;
	}

	return ptr;
}

template <typename T>
void NtAllocator::DeAllocateArray(void* ptr)
{
	if ( ptr == nullptr )
	{
		return;
	}

	NtSize length = *( ((NtUInt*)ptr) - 1 );

	for (NtSize i = 0; i < size; ++i)
	{
		ptr[i].~T();
	}

	// calcuate how much extra memory was allocated to store the length before the array
	NtSize headerSize = sizeof(NtSize) / sizeof(T);
	if ( (sizeof(NtSize) % sizeof(T)) > 0 )
	{
		headerSize += 1;
	}

	DeAllocate(ptr - headerSize);
}
