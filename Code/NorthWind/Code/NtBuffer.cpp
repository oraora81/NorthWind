
#include "NtCoreLib.h"
#include "NtBuffer.h"

namespace NT
{
namespace FS
{

NtBuffer::NtBuffer()
	: m_usage(NtBuffer::BUFFER_QUANTITY)
	, m_bytes(0)
{

}

NtBuffer::NtBuffer(NtInt num, NtSize size, eUsage usage)
	: m_usage(usage)
	, m_bytes(num * size)
	, m_data(new NtUChar [m_bytes])
{
	NT::Crt::MemSet(m_data.get(), sizeof(NtUChar) * m_bytes);
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
	NT::NtSystem::LoadFile(fileName.Buffer(), m_data, m_bytes);
	m_usage = BUFFER_DYNAMIC;
}

NtFileBuffer::~NtFileBuffer()
{

}



}	// namespace FS

}	// namespace NT
