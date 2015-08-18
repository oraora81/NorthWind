
#pragma once

namespace NT
{

namespace FS
{

class NtBuffer
{

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

	void ReleaseData();

	inline eUsage GetUsage() const;
	inline NtUInt GetBytes() const;
	inline const NtUChar* GetData() const;
	inline NtUChar* GetData();

protected:
	NtBuffer();
	NtBuffer(NtInt num, NtSize size, eUsage usage);
	

private:
	NtBuffer(const NtBuffer& buf);
	NtBuffer& operator=(const NtBuffer& buf);


protected:
	eUsage	m_usage;
	NtUInt	m_bytes;
	std::unique_ptr<NtUChar[]> m_data;
};


/**
*/
class NtFileBuffer : public NtBuffer
{
public:
	NtFileBuffer(const NtString& fileName);
	virtual ~NtFileBuffer();
};

#include "NtBuffer.inl"

}	// namespace FS

}	// namespace NT
