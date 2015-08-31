
#pragma once

namespace nt { namespace FS {

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
	inline ntUint GetBytes() const;
	inline const ntUchar* GetData() const;
	inline ntUchar* GetData();

protected:
	NtBuffer();
	NtBuffer(ntInt num, ntSize size, eUsage usage);
	

private:
	NtBuffer(const NtBuffer& buf);
	NtBuffer& operator=(const NtBuffer& buf);


protected:
	eUsage	m_usage;
	ntUint	m_bytes;
	std::unique_ptr<ntUchar[]> m_data;
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

}	// namespace nt
