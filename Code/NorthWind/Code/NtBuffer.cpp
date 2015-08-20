
#include "NtCoreLib.h"
#include "NtBuffer.h"

namespace nt
{
namespace FS
{

NtBuffer::NtBuffer()
	: m_usage(NtBuffer::BUFFER_QUANTITY)
	, m_bytes(0)
{

}

NtBuffer::NtBuffer(ntInt num, ntSize size, eUsage usage)
	: m_usage(usage)
	, m_bytes(num * size)
	, m_data(new ntUchar [m_bytes])
{
	nt::Crt::MemSet(m_data.get(), sizeof(ntUchar) * m_bytes);
}

NtBuffer::NtBuffer( const NtBuffer& buf )
{
	if (this == &buf)
	{
		NtAsserte(0 && "invalid assign!");
		return;
	}

	*this = buf;
}

NtBuffer::~NtBuffer()
{
	
}

void NtBuffer::ReleaseData()
{
	m_data.reset(nullptr);
}

NtBuffer& NtBuffer::operator=( const NtBuffer& buf )
{
	return *this;
}




//////////////////////////////////////////////////////////////////////////
NtFileBuffer::NtFileBuffer( const NtString& fileName )
	: NtBuffer()
{
	nt::NtSystem::LoadFile(fileName.Buffer(), m_data, m_bytes);
	m_usage = BUFFER_DYNAMIC;
}

NtFileBuffer::~NtFileBuffer()
{

}



}	// namespace FS

}	// namespace nt
