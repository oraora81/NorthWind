
#pragma once

// TODO : 일반 String 기능 적용
//			TLS 적용


namespace nt {

class NtString// : public Memory::NtHeapAlloc
{
	struct sBuffer
	{
		ntWchar*	m_str;
		ntSize		m_size;
		ntLong		m_refCount;
	};

	enum { NTSTRING_BUFFER_SIZE = 24 };

	class NtCharProxy
	{
	public:
		NtCharProxy(NtString& str, ntInt index);
		NtCharProxy& operator =(const NtCharProxy& proxy);
		NtCharProxy& operator =(ntWchar ch);
		operator ntWchar() const;

	private:
		NtString& refString;
		ntInt charIndex;
	};

	friend class NtCharProxy;

public:
	static bool InitEmptyBuffer();

	NtString();
	NtString(const NtString& str);
	NtString(const ntWchar* str);
	NtString(const ntChar* str);
	NtString(const ntWchar* str, ntInt s, ntInt e);
	~NtString();

	// operator overloading
	bool operator ==(const ntWchar* str);
	bool operator ==(const NtString& str);
	bool operator !=(const NtString& str);
	bool operator < (const NtString& str);
	bool operator > (const NtString& str);
	bool operator <=(const NtString& str);
	bool operator >=(const NtString& str);
	
	NtString& operator +=(const ntWchar* str);
	NtString& operator +=(const NtString& str);
	NtString& operator =(const ntWchar* str);
	NtString& operator =(const ntChar* str);
	NtString& operator =(const NtString& str);
	NtString& operator =(ntInt integer);

	const NtString::NtCharProxy operator [](ntInt index) const;
	NtString::NtCharProxy operator [](ntInt index);

	operator ntWchar* ();

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
	void			AssignSubString(const ntWchar* str, ntInt s, ntInt e);
	void			AssignSubString(const ntWchar* s, const ntWchar* e);
	bool			Remove(ntWchar ch);
	void			Replace(ntWchar src, ntWchar dst);
	void			Replace(ntWchar* src, ntWchar* dst);
	ntInt			FindFirst(ntWchar ch);
	ntInt			FindLast(ntWchar ch);
	ntIndex			Find(ntWchar* str);

	// get, set
	inline const ntLong	RefCount() const	{ return m_buffer->m_refCount; }
	inline ntLong&		RefCount()			{ return m_buffer->m_refCount; }
	inline		 ntWchar*Buffer()			{ return m_buffer->m_str; }
	inline const ntWchar*Buffer() const		{ return const_cast<const ntWchar*>(m_buffer->m_str); }
	inline const ntUint	Size() const		{ return m_buffer->m_size; }
	inline void			Size(ntSize size)	{ m_buffer->m_size = size; }
	

	static sBuffer	m_emptyBuffer;

private:
	void	ReleaseBuffer();
	void	AssignString(ntWchar* str);
	
private:
	sBuffer*	m_buffer;
};

} // namespace nt
