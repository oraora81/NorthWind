
#include "stdafx.h"
#pragma hdrstop

#include "NtBuffer.h"

using namespace NT;

namespace NT
{
namespace _dx11
{

NT_IMPLEMENT_OBJECT(NtBuffer, NtObject)
NT_IMPLEMENT_ABSTRACT_FACTORY(NtBuffer)

NtBuffer::NtBuffer()
	: m_numElem(0)
	, m_elemSize(0)
	, m_usage(NtBuffer::BUFFER_QUANTITY)
	, m_bytes(0)
	, m_data(nullptr)
{

}

NtBuffer::NtBuffer(NtInt num, NtInt size, eUsage usage)
	: m_numElem(num)
	, m_elemSize(size)
	, m_usage(usage)
	, m_bytes(num * size)
{
	NtASSERTE(m_numElem > 0);
	NtASSERTE(m_elemSize > 0);

	m_data = new NtChar [m_bytes];
}

NtBuffer::~NtBuffer()
{
	SAFE_DELETE_ARRAY(m_data);
}

}
}