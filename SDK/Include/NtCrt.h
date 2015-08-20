// header
#pragma once

namespace nt
{

namespace Crt
{
	const ntChar*	MakeString(const ntChar* expr, ...);
	const ntWchar*	MakeString(const ntWchar* expr, ...);

	ntChar*			MakeBuffer();
	ntWchar*		MakeWBuffer();

	ntSize		StrLen(const ntChar* buf);
	ntSize		StrLen(const ntWchar* buf);

	ntInt		StrCpy( ntChar* dest, const ntChar* source, ntSize destSize);
	ntInt		StrCpy( ntWchar* dest, const ntWchar* source, ntSize destSize);
	
	ntInt		StrNCpy(ntChar* dest, const ntChar* source, ntSize destSize, ntSize maxSize = _TRUNCATE);
	ntInt		StrNCpy(ntWchar* dest, const ntWchar* source, ntSize destSize, ntSize maxSize = _TRUNCATE);
	
	ntInt		StrCmp(const ntChar* compare1, const ntChar*compare2);
	ntInt		StrCmp(const ntWchar* compare1, const ntWchar* compare2);

	ntInt		StrCat(ntChar* dest, ntSize dstSize, const ntChar* source);
	ntInt		StrNCat(ntChar* dest, ntSize dstSize, const ntChar* source, ntSize srcSize);

	ntInt		StrCat(ntWchar* dest, ntSize dstSize, const ntWchar* source);
	ntInt		StrNCat(ntWchar* dest, ntSize dstSize, const ntWchar* source, ntSize cpySize);

	const ntChar*	StrStr(const ntChar* src, const ntChar* strSearch);
	const ntWchar*	StrStr(const ntWchar* src, const ntWchar* strSearch);

	const ntChar*	StrChr(const ntChar* src, ntInt c);
	const ntWchar*	StrChr(const ntWchar* src, ntWchar c);

	ntFloat		Atof(const ntChar* src);
	ntFloat		Atof(const ntWchar* src);

	ntInt		ToLower(ntWchar* src, ntSize length);

	void		MemCpy(void* dest, void* src, ntUint length);
	void		MemSet(void* target, ntUint length);
	

	// file process
	ERROR_CODE	FOpen(const ntWchar* fileName, const ntWchar* option, FILE*& fp);
	ntLong		FTell(FILE* fp);
	ERROR_CODE	FSeek(FILE* fp, ntLong offset, ntInt origin);

	void WideStrToMultiStr(ntChar* dest, ntSize dstSize, const ntWchar* src);

	template <int RADIX>
	void	NumberToString(ntInt number, ntChar* inBuffer, ntInt bufferSize)
	{
		_itoa_s(number, inBuffer, bufferSize, RADIX);
	}

	template <int RADIX>
	void	NumberToString(ntInt number, ntWchar* inBuffer, ntInt bufferSize)
	{
		_itow_s(number, inBuffer, bufferSize, RADIX);
	}

	ntInt StringToNumber(const ntWchar* buffer);
	ntInt64 StringToNumber64(const ntWchar* buffer);

	ntInt StringToNumber(const ntChar* buffer);
	ntInt64 StringToNumber64(const ntChar* buffer);

	// win api
	ntWchar*	GetCmdLine();
	NtErrorCode	AllocEnvVariable(const ntWchar* envName, ntWchar** envValue);
	void	FreeEnvVariable(ntWchar* envValue);
	
}	// namespace Crt

}	// namespace nt
