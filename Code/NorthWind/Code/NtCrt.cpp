
#include "NtCoreLib.h"

namespace NT
{

namespace Crt
{

const NtChar* MakeString(const NtChar* expr,...)
{
	static _declspec(thread) NtChar buff[256][1024];
	static _declspec(thread) NtInt index = 0;

	int ret;
	va_list argptr;

	va_start(argptr, expr);
	ret = _vsnprintf_s(buff[index & 255], sizeof(buff[index]), _TRUNCATE, expr, argptr);
	assert(ret < sizeof(*buff) && ret != STRUNCATE);
	va_end(argptr);

	return buff[index++ & 255];
}

const NtWChar* MakeString(const NtWChar* expr,...)
{
	static _declspec(thread) NtWChar buff[256][1024];
	static _declspec(thread) NtInt index = 0;

	int ret;
	va_list argptr;
	va_start(argptr, expr);
	ret = _vsnwprintf_s(buff[index & 255], sizeof(buff[index]), _TRUNCATE, expr, argptr);
	assert(ret < sizeof(*buff) && ret != STRUNCATE);
	va_end(argptr);

	return buff[index++ & 255];
}

NtChar* MakeBuffer()
{
	static _declspec(thread) NtChar buff[256][1024];
	static _declspec(thread) NtUInt index = 0;
	NtChar* result = buff[index++ & 255];
	*result = '\0';
	return result;
}

NtWChar* MakeWBuffer()
{
	static _declspec(thread) NtWChar buff[256][1024];
	static _declspec(thread) NtUInt index = 0;
	NtWChar* result = buff[index++ & 255];
	*result = L'\0';
	return result;
}

// String Function
NtSize StrLen(const NtChar* buf)
{
	return strlen(buf);
}

NtSize StrLen(const NtWChar* buf)
{
	return wcslen(buf);
}

NtInt StrCpy( NtChar* dest, const NtChar* source, NtSize destSize)
{
	return strcpy_s(dest, destSize, source);
}

NtInt StrCpy( NtWChar* dest, const NtWChar* source, NtSize destSize)
{
	return wcscpy_s(dest, destSize, source);
}

NtInt StrNCpy(NtChar* dest, const NtChar* source, NtSize destSize, NtSize maxSize)
{
	return strncpy_s(dest, destSize, source, maxSize);
}

NtInt StrNCpy( NtWChar* dest, const NtWChar* source, NtSize destSize, NtSize maxSize )
{
	return wcsncpy_s(dest, destSize, source, maxSize);
}

NtInt StrCmp( const NtChar* compare1, const NtChar* compare2 )
{
	return strcmp(compare1, compare2);
}

NtInt StrCmp( const NtWChar* compare1, const NtWChar* compare2 )
{
	return wcscmp(compare1, compare2);
}

NtInt StrCat(NtChar* dest,NtSize dstSize,const NtChar* source)
{
	return strcat_s(dest, dstSize, source);
}

NtInt StrNCat(NtChar* dest,NtSize dstSize,const NtChar* source,NtSize srcSize)
{
	return strncat_s(dest, dstSize, source, srcSize);
}


NtInt StrCat(NtWChar* dest,NtSize dstSize,const NtWChar* source)
{
	return wcscat_s(dest, dstSize, source);
}

NtInt StrNCat(NtWChar* dest,NtSize dstSize,const NtWChar* source,NtSize cpySize)
{
	return wcsncat_s(dest, dstSize, source, cpySize);
}

const NtChar* StrStr(const NtChar* src, const NtChar* strSearch )
{
	return strstr(src, strSearch);
}

const NtWChar* StrStr(const NtWChar* src, const NtWChar* strSearch )
{
	return wcsstr(src, strSearch);
}


const NtChar* StrChr(const NtChar* src,NtInt c)
{
	return strchr(src, c);
}


const NtWChar* StrChr(const NtWChar* src,NtWChar c)
{
	return wcschr(src, c);
}


NtFloat Atof(const NtChar* src)
{
	return static_cast<NtFloat>(atof(src));
}


NtFloat Atof(const NtWChar* src)
{
	return static_cast<NtFloat>(_wtof(src));
}


NtInt ToLower(NtWChar* src, NtSize length)
{
	return _wcslwr_s(src, length + 1);
}

void MemCpy(void* dest,void* src,NtUInt length)
{
	::memcpy(dest, src, length);
}

void MemSet(void* target,NtUInt length)
{
	::memset(target, 0, length);
}

ERROR_CODE	FOpen(const NtWChar* fileName, const NtWChar* option, FILE*& fp)
{
	FILE* res = NULL;
	errno_t err = _wfopen_s(&res, fileName, option);
	if (0 != err)
	{
		return ERR_FILE_OPEN_FAIL;
	}

	if (NULL == res)
	{
		return ERR_FILE_POINT_OPEN_FAIL;
	}

	fp = res;

	return ERR_SUCCESS;
}

NtLong	FTell(FILE* fp)
{
	return ftell(fp);
}

ERROR_CODE	FSeek(FILE* fp, NtLong offset, NtInt origin)
{
	NtInt res = fseek(fp, offset, origin);
	if (0 != res)
	{
		return ERR_FILE_SEEK_FAIL;
	}

	return ERR_SUCCESS;
}

void WideStrToMultiStr(NtChar* dest, NtSize dstSize, const NtWChar* src)
{
	WideCharToMultiByte(CP_ACP, 0, src, -1, dest, dstSize, NULL, NULL);

	NtAsserte(dest[0] != NULL);
}

NT::NtInt StringToNumber( const NtWChar* buffer )
{
	return _wtoi(buffer);
}

NT::NtInt64 StringToNumber64( const NtWChar* buffer )
{
	return _wtoi64(buffer);
}

NT::NtInt StringToNumber( const NtChar* buffer )
{
	return atoi(buffer);
}

NT::NtInt64 StringToNumber64( const NtChar* buffer )
{
	return _atoi64(buffer);
}


NtWChar* GetCmdLine()
{
	return ::GetCommandLine();
}


NtErrorCode AllocEnvVariable(const NtWChar* envName, NtWChar** envValue)
{
	NtUInt reqSize = 0;
	_wgetenv_s(&reqSize, NULL, 0, envName);

	*envValue = new NtWChar[reqSize];

	NtUInt res = _wgetenv_s(&reqSize, *envValue, reqSize, envName);

	return res == 0 ? NtERROR_SUCCESS : NtERROR_FAIL;
}

void FreeEnvVariable(NtWChar* envValue)
{
	SAFE_DELETE_ARRAY(envValue);
}



}	// namespace Crt

}	// namespace NT
