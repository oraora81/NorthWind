
#include "NtCoreLib.h"

namespace nt { namespace Crt {


const ntChar* MakeString(const ntChar* expr,...)
{
	static _declspec(thread) ntChar buff[256][1024];
	static _declspec(thread) ntInt index = 0;

	int ret;
	va_list argptr;

	va_start(argptr, expr);
	ret = _vsnprintf_s(buff[index & 255], sizeof(buff[index]), _TRUNCATE, expr, argptr);
	assert(ret < sizeof(*buff) && ret != STRUNCATE);
	va_end(argptr);

	return buff[index++ & 255];
}

const ntWchar* MakeString(const ntWchar* expr,...)
{
	static _declspec(thread) ntWchar buff[256][1024];
	static _declspec(thread) ntInt index = 0;

	int ret;
	va_list argptr;
	va_start(argptr, expr);
	ret = _vsnwprintf_s(buff[index & 255], sizeof(buff[index]), _TRUNCATE, expr, argptr);
	assert(ret < sizeof(*buff) && ret != STRUNCATE);
	va_end(argptr);

	return buff[index++ & 255];
}

ntChar* MakeBuffer()
{
	static _declspec(thread) ntChar buff[256][1024];
	static _declspec(thread) ntUint index = 0;
	ntChar* result = buff[index++ & 255];
	*result = '\0';
	return result;
}

ntWchar* MakeWBuffer()
{
	static _declspec(thread) ntWchar buff[256][1024];
	static _declspec(thread) ntUint index = 0;
	ntWchar* result = buff[index++ & 255];
	*result = L'\0';
	return result;
}

// String Function
ntSize StrLen(const ntChar* buf)
{
	return strlen(buf);
}

ntSize StrLen(const ntWchar* buf)
{
	return wcslen(buf);
}

ntInt StrCpy( ntChar* dest, const ntChar* source, ntSize destSize)
{
	return strcpy_s(dest, destSize, source);
}

ntInt StrCpy( ntWchar* dest, const ntWchar* source, ntSize destSize)
{
	return wcscpy_s(dest, destSize, source);
}

ntInt StrNCpy(ntChar* dest, const ntChar* source, ntSize destSize, ntSize maxSize)
{
	return strncpy_s(dest, destSize, source, maxSize);
}

ntInt StrNCpy( ntWchar* dest, const ntWchar* source, ntSize destSize, ntSize maxSize )
{
	return wcsncpy_s(dest, destSize, source, maxSize);
}

ntInt StrCmp( const ntChar* compare1, const ntChar* compare2 )
{
	return strcmp(compare1, compare2);
}

ntInt StrCmp( const ntWchar* compare1, const ntWchar* compare2 )
{
	return wcscmp(compare1, compare2);
}

ntInt StrCat(ntChar* dest,ntSize dstSize,const ntChar* source)
{
	return strcat_s(dest, dstSize, source);
}

ntInt StrNCat(ntChar* dest,ntSize dstSize,const ntChar* source,ntSize srcSize)
{
	return strncat_s(dest, dstSize, source, srcSize);
}


ntInt StrCat(ntWchar* dest,ntSize dstSize,const ntWchar* source)
{
	return wcscat_s(dest, dstSize, source);
}

ntInt StrNCat(ntWchar* dest,ntSize dstSize,const ntWchar* source,ntSize cpySize)
{
	return wcsncat_s(dest, dstSize, source, cpySize);
}

const ntChar* StrStr(const ntChar* src, const ntChar* strSearch )
{
	return strstr(src, strSearch);
}

const ntWchar* StrStr(const ntWchar* src, const ntWchar* strSearch )
{
	return wcsstr(src, strSearch);
}


const ntChar* StrChr(const ntChar* src,ntInt c)
{
	return strchr(src, c);
}


const ntWchar* StrChr(const ntWchar* src,ntWchar c)
{
	return wcschr(src, c);
}

bool IsNullOrEmpty(const ntChar* src)
{
	if (src == nullptr)
	{
		return true;
	}

	return Crt::StrLen(src) == 0 ? true : false;
}

bool IsNullOrEmpty(const ntWchar* src)
{
	if (src == nullptr)
	{
		return true;
	}

	return Crt::StrLen(src) == 0 ? true : false;
}

ntFloat Atof(const ntChar* src)
{
	return static_cast<ntFloat>(atof(src));
}


ntFloat Atof(const ntWchar* src)
{
	return static_cast<ntFloat>(_wtof(src));
}


ntInt ToLower(ntWchar* src, ntSize length)
{
	return _wcslwr_s(src, length + 1);
}

void MemCpy(void* dest, void* src, ntUint length)
{
	::memcpy(dest, src, length);
}

void MemSet(void* target, ntUint length)
{
	::memset(target, 0, length);
}

ERROR_CODE	FOpen(const ntWchar* fileName, const ntWchar* option, FILE*& fp)
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

ntLong	FTell(FILE* fp)
{
	return ftell(fp);
}

ERROR_CODE	FSeek(FILE* fp, ntLong offset, ntInt origin)
{
	ntInt res = fseek(fp, offset, origin);
	if (0 != res)
	{
		return ERR_FILE_SEEK_FAIL;
	}

	return ERR_SUCCESS;
}

void FClose(FILE*& fp)
{
	fclose(fp);
	fp = nullptr;
}

ntSize FSize(FILE*& fp)
{
	FSeek(fp, 0, SEEK_END);
	ntSize size = (ntSize)FTell(fp);
	FSeek(fp, 0, SEEK_SET);

	return size;
}

void WideStrToMultiStr(ntChar* dest, ntSize dstSize, const ntWchar* src)
{
	WideCharToMultiByte(CP_ACP, 0, src, -1, dest, (ntInt)dstSize, NULL, NULL);

	NtAsserte(dest[0] != NULL);
}

ntInt StringToNumber( const ntWchar* buffer )
{
	return _wtoi(buffer);
}

ntInt64 StringToNumber64( const ntWchar* buffer )
{
	return _wtoi64(buffer);
}

ntInt StringToNumber( const ntChar* buffer )
{
	return atoi(buffer);
}

ntInt64 StringToNumber64( const ntChar* buffer )
{
	return _atoi64(buffer);
}


ntWchar* GetCmdLine()
{
	return ::GetCommandLine();
}


NtErrorCode AllocEnvVariable(const ntWchar* envName, ntWchar** envValue)
{
	ntSize reqSize = 0;
	_wgetenv_s(&reqSize, NULL, 0, envName);

	*envValue = new ntWchar[reqSize];

	ntUint res = _wgetenv_s(&reqSize, *envValue, reqSize, envName);

	return res == 0 ? NtERROR_SUCCESS : NtERROR_FAIL;
}

void FreeEnvVariable(ntWchar* envValue)
{
	SAFE_DELETE_ARRAY(envValue);
}



} }

