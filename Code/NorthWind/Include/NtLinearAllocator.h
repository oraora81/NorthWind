
#pragma once

// 
// NtLinearAllocator
//----------------------------------------------------------------------------

namespace nt { namespace memory {

class NtLinearAllocator : public NtAllocator
{
	NT_DECLARE_OBJECT
	NT_PREVENT_OBJECT_COPY(NtLinearAllocator)

	enum { DEFAULT_ALIGN_SIZE = 8 };

protected:
	NtLinearAllocator();

public:
	virtual ~NtLinearAllocator();

	bool Initialize(ntSize baseAllocSize);

	void* Allocate(ntSize size, ntUchar alignment = DEFAULT_ALIGN_SIZE);
	
	void DeAllocate(void* ptr);

	void Reset();


private:
	void*	m_initialPosition;
	void*	m_currentPosition;
	ntSize	m_totalSize;

	
};

}
}	// namespace nt
