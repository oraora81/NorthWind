
#pragma once

// TODO : 일반 String 기능 적용
//			TLS 적용


namespace NT
{

class NtString// : public Memory::NtHeapAlloc
{
	struct sBuffer
	{
		NtWChar*	m_str;
		NtSize		m_size;
		NtLong		m_refCount;
	};

	enum { NTSTRING_BUFFER_SIZE = 24 };

	class NtCharProxy
	{
	public:
		NtCharProxy(NtString& str, NtInt index);
		NtCharProxy& operator =(const NtCharProxy& proxy);
		NtCharProxy& operator =(NtWChar ch);
		operator NtWChar() const;

	private:
		NtString& refString;
		NtInt charIndex;
	};

	friend class NtCharProxy;

public:
	static bool InitEmptyBuffer();

	NtString();
	NtString(const NtString& str);
	NtString(const NtWChar* str);
	NtString(const NtChar* str);
	NtString(const NtWChar* str, NtInt s, NtInt e);
	~NtString();

	// operator overloading
	bool operator ==(const NtWChar* str);
	bool operator ==(const NtString& str);
	bool operator !=(const NtString& str);
	bool operator < (const NtString& str);
	bool operator > (const NtString& str);
	bool operator <=(const NtString& str);
	bool operator >=(const NtString& str);
	
	NtString& operator +=(const NtWChar* str);
	NtString& operator +=(const NtString& str);
	NtString& operator =(const NtWChar* str);
	NtString& operator =(const NtChar* str);
	NtString& operator =(const NtString& str);
	NtString& operator =(NtInt integer);

	const NtString::NtCharProxy operator [](NtInt index) const;
	NtString::NtCharProxy operator [](NtInt index);

	operator NtWChar* ();

	// outer operator
	friend bool		operator ==(const NtString& str1, const NtString& str2);
	friend bool		operator !=(const NtString& str1, const NtString& str2);
	friend bool		operator < (const NtString& str1, const NtString& str2);
	friend bool		operator > (const NtString& str1, const NtString& str2);
	friend bool		operator <=(const NtString& str1, const NtString& str2);
	friend bool		operator >=(const NtString& str1, const NtString& str2);

	friend std::ostream& operator <<(std::ostream& os, const NtString& str);
	friend std::wostream& operator << (std::wostream& os, const NtString& str);

	// general
	void 			Uppercase();
	void 			Lowercase();

	void			InitBuffer();
	bool			Empty();
	void			Clear();
	void			AssignSubString(const NtWChar* str, NtInt s, NtInt e);
	void			AssignSubString(const NtWChar* s, const NtWChar* e);
	bool			Remove(NtWChar ch);
	void			Replace(NtWChar src, NtWChar dst);
	void			Replace(NtWChar* src, NtWChar* dst);
	NtInt			FindFirst(NtWChar ch);
	NtInt			FindLast(NtWChar ch);
	NtIndex			Find(NtWChar* str);

	// get, set
	inline const NtLong	RefCount() const	{ return m_buffer->m_refCount; }
	inline NtLong&		RefCount()			{ return m_buffer->m_refCount; }
	inline		 NtWChar*Buffer()			{ return m_buffer->m_str; }
	inline const NtWChar*Buffer() const		{ return const_cast<const NtWChar*>(m_buffer->m_str); }
	inline const NtUInt	Size() const		{ return m_buffer->m_size; }
	inline void			Size(NtSize size)	{ m_buffer->m_size = size; }
	

	static sBuffer	m_emptyBuffer;

private:
	void	ReleaseBuffer();
	void	AssignString(NtWChar* str);
	
private:
	sBuffer*	m_buffer;
};

} // namespace NT