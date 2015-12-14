
#pragma once

namespace nt {

class NtString
{
	struct sBuffer
	{
		ntWchar*	m_str;
		ntSize		m_size;
		ntLong		m_refCount;
	};

	enum { NTSTRING_BUFFER_SIZE = 24 };

	// 하나의 문자를 융통성이 있게 접근하기 위한 Proxy
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

	// 
	NtString();
	NtString(const NtString& str);
	NtString(const ntWchar* str);
	NtString(const ntChar* str);
	NtString(const ntWchar* str, ntInt s, ntInt e);
	~NtString();

	// 연산자 오버로딩
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
	
	// 
	const NtString::NtCharProxy operator [](ntInt index) const;
	NtString::NtCharProxy operator [](ntInt index);

	// 캐스팅 연산자 오버로딩
	operator ntWchar* ();

	// 외부 연산자 오버로딩
	friend bool	operator ==(const NtString& str1, const NtString& str2);
	friend bool	operator !=(const NtString& str1, const NtString& str2);
	friend bool	operator < (const NtString& str1, const NtString& str2);
	friend bool	operator > (const NtString& str1, const NtString& str2);
	friend bool	operator <=(const NtString& str1, const NtString& str2);
	friend bool	operator >=(const NtString& str1, const NtString& str2);
	// for output stream
	friend std::ostream& operator <<(std::ostream& os, const NtString& str);
	friend std::wostream& operator << (std::wostream& os, const NtString& str);

	//
	void InitBuffer();
	//
	bool Empty();
	//
	void Clear();
	//
	void AssignSubString(const ntWchar* str, ntInt s, ntInt e);
	//
	void AssignSubString(const ntWchar* s, const ntWchar* e);
	//
	void Uppercase();
	//
	void Lowercase();
	//
	bool Remove(ntWchar ch);
	//
	void Replace(ntWchar src, ntWchar dst);
	//
	void Replace(ntWchar* src, ntWchar* dst);
	//
	ntInt FindFirst(ntWchar ch);
	//
	ntInt FindLast(ntWchar ch);
	//
	ntIndex	Find(ntWchar* str);


	// get, set
	NtInline const ntLong	RefCount() const	{ return m_buffer->m_refCount; }
	NtInline ntLong&		RefCount()			{ return m_buffer->m_refCount; }
	NtInline		 ntWchar*Buffer()			{ return m_buffer->m_str; }
	NtInline const ntWchar*Buffer() const		{ return const_cast<const ntWchar*>(m_buffer->m_str); }
	NtInline const ntUint	Size() const		{ return m_buffer->m_size; }
	NtInline void			Size(ntSize size)	{ m_buffer->m_size = size; }
	
	// 빈 문자열용 buffer
	static sBuffer	m_emptyBuffer;

private:
	void	ReleaseBuffer();
	void	AssignString(ntWchar* str);
	
private:
	sBuffer*	m_buffer;
};

} // namespace nt
