
#pragma once

namespace NT
{
namespace Memory
{

class NtLinearAllocator : public NtAllocator
{
	NT_DECLARE_OBJECT
	NT_PREVENT_OBJECT_COPY(NtLinearAllocator)

protected:
	NtLinearAllocator();

public:
	virtual ~NtLinearAllocator();

	bool Initialize(NtSize baseAllocSize);

	void* Allocate(NtSize size, NtUChar alignment);
	
	void DeAllocate(void* ptr);

	void Reset();


private:
	void*	m_initialPosition;
	void*	m_currentPosition;
	NtSize	m_totalSize;
};

}
}	// namespace NT
