// header
#pragma once

namespace NT
{

namespace Crt
{
	const NtChar*	MakeString(const NtChar* expr, ...);
	const NtWChar*	MakeString(const NtWChar* expr, ...);

	NtChar*			MakeBuffer();
	NtWChar*		MakeWBuffer();

	NtSize		StrLen(const NtChar* buf);
	NtSize		StrLen(const NtWChar* buf);

	NtInt		StrCpy( NtChar* dest, const NtChar* source, NtSize destSize);
	NtInt		StrCpy( NtWChar* dest, const NtWChar* source, NtSize destSize);
	
	NtInt		StrNCpy(NtChar* dest, const NtChar* source, NtSize destSize, NtSize maxSize = _TRUNCATE);
	NtInt		StrNCpy(NtWChar* dest, const NtWChar* source, NtSize destSize, NtSize maxSize = _TRUNCATE);
	
	NtInt		StrCmp(const NtChar* compare1, const NtChar*compare2);
	NtInt		StrCmp(const NtWChar* compare1, const NtWChar* compare2);

	NtInt		StrCat(NtChar* dest, NtSize dstSize, const NtChar* source);
	NtInt		StrNCat(NtChar* dest, NtSize dstSize, const NtChar* source, NtSize srcSize);

	NtInt		StrCat(NtWChar* dest, NtSize dstSize, const NtWChar* source);
	NtInt		StrNCat(NtWChar* dest, NtSize dstSize, const NtWChar* source, NtSize cpySize);

	const NtChar*	StrStr(const NtChar* src, const NtChar* strSearch);
	const NtWChar*	StrStr(const NtWChar* src, const NtWChar* strSearch);

	const NtChar*	StrChr(const NtChar* src, NtInt c);
	const NtWChar*	StrChr(const NtWChar* src, NtWChar c);

	NtFloat		Atof(const NtChar* src);
	NtFloat		Atof(const NtWChar* src);

	NtInt		ToLower(NtWChar* src, NtSize length);

	void		MemCpy(void* dest, void* src, NtUInt length);
	void		MemSet(void* target, NtUInt length);
	

	// file process
	ERROR_CODE	FOpen(const NtWChar* fileName, const NtWChar* option, FILE*& fp);
	NtLong		FTell(FILE* fp);
	ERROR_CODE	FSeek(FILE* fp, NtLong offset, NtInt origin);

	void WideStrToMultiStr(NtChar* dest, NtSize dstSize, const NtWChar* src);

	template <int RADIX>
	void	NumberToString(NtInt number, NtChar* inBuffer, NtInt bufferSize)
	{
		_itoa_s(number, inBuffer, bufferSize, RADIX);
	}

	template <int RADIX>
	void	NumberToString(NtInt number, NtWChar* inBuffer, NtInt bufferSize)
	{
		_itow_s(number, inBuffer, bufferSize, RADIX);
	}

	NtInt StringToNumber(const NtWChar* buffer);
	NtInt64 StringToNumber64(const NtWChar* buffer);

	NtInt StringToNumber(const NtChar* buffer);
	NtInt64 StringToNumber64(const NtChar* buffer);

	// win api
	NtWChar*	GetCmdLine();
	NtErrorCode	AllocEnvVariable(const NtWChar* envName, NtWChar** envValue);
	void	FreeEnvVariable(NtWChar* envValue);
	
}	// namespace Crt

}	// namespace NT