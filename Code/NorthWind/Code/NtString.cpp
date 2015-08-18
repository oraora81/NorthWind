
#include "NtCoreLib.h"
#include "NtString.h"

namespace NT
{

NtString::sBuffer NtString::m_emptyBuffer;

bool NtString::InitEmptyBuffer()
{
	if (NULL == NtString::m_emptyBuffer.m_str)
	{
		NtString::m_emptyBuffer.m_str = new NtWChar [1];
		Crt::StrCpy(m_emptyBuffer.m_str, L"", 1);
		return true;
	}

	return false;
}

// NtString::NtCharProxy Implement
NtString::NtCharProxy::NtCharProxy(NtString& str, NtInt index)
	: refString(str)
	, charIndex(index)
{

}

NtString::NtCharProxy& NtString::NtCharProxy::operator =(const NtString::NtCharProxy& proxy)
{
	NtWChar* tempBuffer = new NtWChar[refString.Size() + 1];
	Crt::StrCpy(tempBuffer, refString.Buffer(), refString.Size() + 1);

	tempBuffer[charIndex] = proxy.refString.Buffer()[proxy.charIndex];

	refString.ReleaseBuffer();
	refString.InitBuffer();
	refString.AssignString(tempBuffer);

	return *this;
}

NtString::NtCharProxy& NtString::NtCharProxy::operator =(NtWChar ch)
{
	NtWChar* tempBuffer = new NtWChar[refString.Size() + 1];
	Crt::StrCpy(tempBuffer, refString.Buffer(), refString.Size() + 1);

	tempBuffer[charIndex] = ch;

	refString.ReleaseBuffer();
	refString.InitBuffer();
	refString.AssignString(tempBuffer);

	return *this;
}

NtString::NtCharProxy::operator NtWChar() const
{
	return refString.Buffer()[charIndex];
}

// NtString Implement
NtString::NtString()
: m_buffer(&m_emptyBuffer)
{

}

NtString::NtString(const NtString& str)
: m_buffer(&m_emptyBuffer)
{
	*this = str;
}

NtString::NtString(const NtWChar* str)
: m_buffer(&m_emptyBuffer)
{
	*this = str;
}

NtString::NtString(const NtChar* str)
: m_buffer(&m_emptyBuffer)
{
	*this = str;
}

NtString::NtString(const NtWChar* str, NtInt s, NtInt e)
: m_buffer(&m_emptyBuffer)
{
	AssignSubString(str, s, e);
}

NtString::~NtString()
{
	ReleaseBuffer();
}

// Operator
bool NtString::operator ==(const NtWChar* str)
{
	if (Crt::StrCmp(Buffer(), str) == 0)
	{
		return true;
	}

	return false;
}

bool NtString::operator ==(const NtString& str)
{
	if (Crt::StrCmp(Buffer(), str.m_buffer->m_str) == 0)
	{
		return true;
	}

	return false;
}

bool NtString::operator !=(const NtString &str)
{
	if (Crt::StrCmp(Buffer(), str.m_buffer->m_str) != 0)
	{
		return true;
	}

	return false;
}

bool NtString::operator <(const NtString& str)
{
	if (Crt::StrCmp(Buffer(), str.m_buffer->m_str) < 0)
	{
		return true;
	}

	return false;
}

bool NtString::operator >(const NtString& str)
{
	if (Crt::StrCmp(Buffer(), str.m_buffer->m_str) > 0)
	{
		return true;
	}

	return false;
}

bool NtString::operator <=(const NtString& str)
{
	if (Crt::StrCmp(Buffer(), str.m_buffer->m_str) <= 0)
	{
		return true;
	}

	return false;
}

bool NtString::operator >=(const NtString& str)
{
	if (Crt::StrCmp(Buffer(), str.m_buffer->m_str) >= 0)
	{
		return true;
	}

	return false;
}

NtString& NtString::operator +=(const NtWChar* str)
{
	NtSize passedSize = Crt::StrLen(str);
	NtSize size = Size() + passedSize + 1;
	NtWChar* tempBuffer = new NtWChar[size];
	Crt::StrCpy(tempBuffer, Buffer(), Size() + 1);
	Crt::StrNCat(tempBuffer, size, str, passedSize + 1);

	ReleaseBuffer();
	InitBuffer();
	AssignString(tempBuffer);

	return *this;
}

NtString& NtString::operator +=(const NtString& str)
{
	NtSize size = Size() + str.m_buffer->m_size + 1;
	NtWChar* tempBuffer = new NtWChar[size];
	Crt::StrCpy(tempBuffer, Buffer(), Size() + 1);
	Crt::StrNCat(tempBuffer, size, str.m_buffer->m_str, str.m_buffer->m_size + 1);

	ReleaseBuffer();
	InitBuffer();
	AssignString(tempBuffer);

	return *this;
}

NtString& NtString::operator =(const NtWChar* str)
{
	NtAsserte(NULL != str);

	if (*this == str)
	{
		return *this;
	}

	ReleaseBuffer();
	InitBuffer();

	Size(Crt::StrLen(str));

	NtWChar* tempBuffer = new NtWChar[Size() + 1];
	Crt::StrCpy(tempBuffer, str, Size() + 1);

	AssignString(tempBuffer);

	return *this;
}

NtString& NtString::operator =(const NtChar* str)
{
	NtAsserte(NULL != str);

	NtSize strSize = Crt::StrLen(str);
	NtWChar* tempBuffer = new NtWChar[strSize + 1];

	MultiByteToWideChar(CP_ACP, 0, str, strSize + 1, tempBuffer, strSize + 1);

	if (*this == (const NtWChar*)tempBuffer)
	{
		SAFE_DELETE_ARRAY(tempBuffer);
		return *this;
	}

	ReleaseBuffer();	
	InitBuffer();
	AssignString(tempBuffer);

	return *this;
}

NtString& NtString::operator =(const NtString& str)
{
	if (*this == str)
	{
		return *this;
	}

	// For MultiThread Guard Start
	ReleaseBuffer();

	m_buffer = str.m_buffer;
	Atom::Inc(str.m_buffer->m_refCount);
	// For MultiThread Guard End

	return *this;
}

NtString& NtString::operator =(NtInt integer)
{
	// Convert From integer to string
	ReleaseBuffer();
	InitBuffer();

	NtWChar* tempBuffer = new NtWChar[NTSTRING_BUFFER_SIZE + 1];
	Crt::MemSet(tempBuffer, NTSTRING_BUFFER_SIZE + 1);
	Crt::NumberToString<10>(integer, tempBuffer, NTSTRING_BUFFER_SIZE + 1);

	AssignString(tempBuffer);

	return *this;
}

const NtString::NtCharProxy NtString::operator [](NtInt index) const
{
	NtAsserte(index < (NtInt)Size());

	return NtString::NtCharProxy(const_cast<NtString&>(*this), index);
}

NtString::NtCharProxy NtString::operator [](NtInt index)
{
	NtAsserte(index < (NtInt)Size());

	return NtString::NtCharProxy(*this, index);
}


NtString::operator NtWChar* ()
{
	return m_buffer->m_str;
}

bool NtString::Empty()
{
	return Size() == 0 ? true : false;
}

void NtString::Clear()
{
	ReleaseBuffer();
	InitBuffer();
}

void NtString::AssignSubString(const NtWChar* str, NtInt s, NtInt e)
{
	if (str == nullptr)
	{
		return;
	}

	if ((e - s) <= 0)
	{
		return;
	}

	// self ch + null
	NtSize size = (e - s) + 2;
	NtWChar* newStr = new NtWChar[size];
	NtAsserte(newStr != nullptr);

	Crt::MemSet(newStr, sizeof(NtWChar) * size);

	NtWChar* target = newStr;
	while (s != e)
	{
		(*target) = str[s++];
		++target;
	}
	*target++ = str[e];
	*target = L'\0';

	ReleaseBuffer();
	InitBuffer();
	AssignString(newStr);
}


void NtString::AssignSubString(const NtWChar* s, const NtWChar* e)
{
	if ((e - s) <= 0)
	{
		return;
	}

	// null + [s, e)
	NtSize size = 2;

	const NtWChar* idx = s;
	while(idx++ != e)	++size;

	NtWChar* newStr = new NtWChar[size];
	NtAsserte(newStr != nullptr);

	Crt::MemSet(newStr, sizeof(NtWChar) * size);

	NtWChar* target = newStr;
	idx = s;
	while (idx != e)
	{
		(*target) = *idx++;
		++target;
	}
	*target++ = *e;
	*target = L'\0';

	ReleaseBuffer();
	InitBuffer();
	AssignString(newStr);
}


bool NtString::Remove(NtWChar ch)
{
	NtWChar* src = Buffer();
	NtWChar* temp = src;
	
	NtInt targetCount = 0;
	while(*temp)
	{
		if (*temp == ch)
		{
			++targetCount;
		}
		++temp;
	}

	if (targetCount == 0)
	{
		return false;
	}

	NtWChar* buf = new NtWChar[Size() - targetCount + 1];
	Crt::MemSet(buf, sizeof(NtWChar) * (Size() - targetCount + 1));

	while(*src)
	{
		if ((*src) != ch)
		{
			(*buf) = (*src); 
		}
		++buf;
		++src;
	}

	*src = L'\0';

	ReleaseBuffer();
	InitBuffer();
	AssignString(buf);

	return true;
}


void NtString::Replace(NtWChar src, NtWChar dst)
{
	NtInt res = FindFirst(src);
	if (res == -1)
	{
		return;
	}

	NtWChar* srcStr = Buffer();
	NtWChar* buf = new NtWChar[Size() + 1];
	Crt::MemSet(buf, sizeof(NtWChar) * (Size() + 1));

	NtWChar* target = buf;
	while(*srcStr)
	{
		if ((*srcStr) != src)
		{
			*target = *srcStr;
		}
		else
		{
			*target = dst;
		}

		++target;
		++srcStr;
	}
	
	buf[Size()] = L'\0';

	ReleaseBuffer();
	InitBuffer();
	AssignString(buf);
}


void NtString::Replace(NtWChar* src, NtWChar* dst)
{
	//
	NtWChar* srcStr = Buffer();
	NtWChar* cpStr = src;
	while (*srcStr)
	{
		if ((*srcStr) == (*cpStr))
		{
			bool isSameStr = true;
			NtWChar* temp = srcStr;
			// 계속 찾아본다.
			while (*cpStr)
			{
				if ((*temp) != (*cpStr))
				{
					isSameStr = false;
					break;
				}
				temp++;
				cpStr++;
			}

			if (isSameStr)
			{
				NtSize srcLength = Crt::StrLen(src);
				NtSize dstLength = Crt::StrLen(dst);
				NtSize newSize = Size() - srcLength + dstLength + 1;

				NtWChar* buf = new NtWChar[newSize];
				Crt::MemSet(buf, sizeof(NtWChar) * newSize);

				NtWChar* tempSrc = Buffer();
				NtInt i = 0;
				for (; tempSrc != srcStr; ++tempSrc)
				{
					buf[i] = *tempSrc;
					++i;
				}

				for (NtUInt j = 0; j < dstLength; ++j)
				{
					buf[i] = dst[j];
					++i;
				}

				tempSrc += srcLength;
				for (; *tempSrc != NULL; ++tempSrc)
				{
					buf[i] = *tempSrc;
					++i;
				}

				ReleaseBuffer();
				InitBuffer();
				AssignString(buf);
			}
		}

		srcStr++;
	}
}


NtInt NtString::FindFirst(NtWChar ch)
{
	NtWChar* buffer = Buffer();
	while ((*buffer) && (*buffer) != ch)
	{
		++buffer;
	}

	return	(NtUInt)(buffer - Buffer()) >= Size() ? -1 : (buffer - Buffer());
}


NtInt NtString::FindLast(NtWChar ch)
{
	NtWChar* buffer = Buffer() + Size()-1;
	while ((buffer >= Buffer()) && (*buffer) != ch)
	{
		--buffer;
	}

	return (buffer < Buffer()) ? -1 : (buffer - Buffer());
}


NtIndex NtString::Find(NtWChar* str)
{
	const NtWChar* dst = nullptr;
	const NtWChar* src = nullptr;
	for (NtUInt i = 0; i < Size(); ++i)
	{
		src = &(Buffer()[i]);
		dst = str;
		bool isValid = true;
		while ( (*src != L'\0') && (*dst != L'\0') )
		{
			if (*src != *dst)
			{
				isValid = false;
				break;
			}
			++src;
			++dst;
		}

		if (isValid)
		{
			return i;
		}
	}

	return -1;
}


void NtString::Uppercase()
{
	if (Empty())
	{
		return;
	}

	NtWChar* dupStr = new NtWChar[Size() + 1];
	Crt::StrCpy(dupStr, Buffer(), Size() + 1);
	if (0 != _wcsupr_s(dupStr, Size() + 1))
	{
		NtASSERT(false, L"123");
		SAFE_DELETE_ARRAY(dupStr);
		return;
	}

	ReleaseBuffer();
	InitBuffer();
	AssignString(dupStr);
}

void NtString::Lowercase()
{
	if (Empty())
	{
		return;
	}

	NtWChar* dupStr = new NtWChar[Size() + 1];
	Crt::StrCpy(dupStr, Buffer(), Size() + 1);
	if (0 != _wcslwr_s(dupStr, Size() + 1))
	{
		NtASSERT(false, L"fail _wcslwr_s call");
		SAFE_DELETE_ARRAY(dupStr);
		return;
	}

	ReleaseBuffer();
	InitBuffer();
	AssignString(dupStr);
}

void NtString::InitBuffer()
{
	m_buffer = new sBuffer;
	
	m_buffer->m_str = NULL;
	m_buffer->m_size = 0;
	m_buffer->m_refCount = 0;
}

void NtString::ReleaseBuffer()
{
	if (1 < RefCount())
	{
		// refCount가 1보다 큰 값이기 때문에 refCount만 감소시키고 참조를 해제하는 경우.
		Atom::Dec(RefCount());
		m_buffer = &NtString::m_emptyBuffer;
	}
	else
	{
		// Def String이 아니라면
		if (Crt::StrCmp(Buffer(), L"") != 0)
		{
			// refCount가 0이 되어 메모리가 해제되는 경우.
			Atom::Dec(m_buffer->m_refCount);
			SAFE_DELETE_ARRAY(Buffer());
			SAFE_DELETE(m_buffer);
			m_buffer = &NtString::m_emptyBuffer;
		}
	}
}

void NtString::AssignString(NtWChar* str)
{
	Size(Crt::StrLen(str));
	m_buffer->m_str = str;
	Atom::Inc(m_buffer->m_refCount);
}


bool operator ==(const NtString& str1,const NtString& str2)
{
	if (Crt::StrCmp(str1.m_buffer->m_str, str2.m_buffer->m_str) == 0)
	{
		return true;
	}

	return false;
}

bool operator !=(const NtString& str1,const NtString& str2)
{
	if (Crt::StrCmp(str1.m_buffer->m_str, str2.m_buffer->m_str) != 0)
	{
		return true;
	}

	return false;
}


bool operator < (const NtString& str1, const NtString& str2)
{
	if (Crt::StrCmp(str1.m_buffer->m_str, str2.m_buffer->m_str) < 0)
	{
		return true;
	}

	return false;
}

bool operator > (const NtString& str1, const NtString& str2)
{
	if (Crt::StrCmp(str1.m_buffer->m_str, str2.m_buffer->m_str) > 0 )
	{
		return true;
	}

	return false;
}


bool operator <=(const NtString& str1, const NtString& str2)
{
	if (Crt::StrCmp(str1.m_buffer->m_str, str2.m_buffer->m_str) <= 0)
	{
		return true;
	}

	return false;
}

bool operator >=(const NtString& str1, const NtString& str2)
{
	if (Crt::StrCmp(str1.m_buffer->m_str, str2.m_buffer->m_str) >= 0)
	{
		return true;
	}

	return false;
}

std::ostream& operator << (std::ostream& os, const NtString& str)
{
	NtChar* outStr = new NtChar[str.m_buffer->m_size + 1];
	WideCharToMultiByte( CP_ACP, 0, str.m_buffer->m_str, -1, outStr, str.m_buffer->m_size + 1, NULL, NULL );

	os << outStr;
	SAFE_DELETE_ARRAY(outStr);
	return os;
}

std::wostream& operator << (std::wostream& os, const NtString& str)
{
	os << str.m_buffer->m_str;
	return os;
}

} // namespace NT