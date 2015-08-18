
#pragma once

#include "NtObject.h"

namespace NT
{

namespace _dx11
{

class NtBuffer : public NtObject
{
	NT_DECLARE_OBJECT

public:
	enum Lock
	{
		BUF_LOCK_READ_ONLY,
		BUF_LOCK_WRITE_ONLY,
		BUF_LOCK_READ_WRITE,
		BUF_LOCK_QUANTITY,
	};

	enum eUsage
	{
		BUFFER_STATIC,
		BUFFER_DYNAMIC,
		BUFFER_RENDERTARGET,
		BUFFER_DEPTHSTENCIL,
		BUFFER_TEXTURE,
		BUFFER_QUANTITY,
	};

public:
	virtual ~NtBuffer();

	inline NtInt GetNumElem() const;
	inline NtInt GetElemSize() const;
	inline eUsage GetUsage() const;

	inline NtInt GetBytes() const;
	inline const NtChar* GetData() const;

protected:
	NtBuffer();
	NtBuffer(NtInt num, NtInt size, eUsage usage);

protected:
	NtInt	m_numElem;
	NtInt	m_elemSize;
	eUsage	m_usage;
	NtInt	m_bytes;
	NtChar* m_data;
};

NT_REGISTER_FACTORY(NtBuffer)
#include "NtBuffer.inl"

}	// namespace _dx11

}	// namespace NT
