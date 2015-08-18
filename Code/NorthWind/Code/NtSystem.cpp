
#include "NtCoreLib.h"
#include <sys/stat.h>
#include <crtdbg.h>


namespace NT
{

void SystemAbort(NtInt exitCode)
{
	::exit(exitCode);
}

void SystemExit(NtInt exitCode)
{
	::PostQuitMessage(exitCode);
}


bool NtSystem::ReadFileSize(const NtWChar* fileName,NtUInt& size)
{
	struct _stat64i32 fileStat;
	if (_wstat(fileName, &fileStat) != 0)
	{
		size = 0;
		return false;
	}

	size = fileStat.st_size;

	return true;
}


bool NtSystem::LoadFile(const NtWChar* fileName, std::unique_ptr<NtUChar[]>& buffer, NtUInt& size)
{
	struct _stat64i32 fileStat;
	if (_wstat(fileName, &fileStat) != 0)
	{
		size = 0;
		return false;
	}

	FILE* fp = NULL;
	errno_t err = _wfopen_s(&fp, fileName, L"rb");
	if (0 != err)
	{
		size = 0;
		return false;
	}

	if (NULL == fp)
	{
		size = 0;
		return false;
	}

	size = fileStat.st_size;
	buffer.reset(new NtUChar[size]);
	NT::Crt::MemSet(buffer.get(), sizeof(NtUChar) * size);

	NtUInt readSize = (NtUInt)fread(buffer.get(), sizeof(NtUChar), size, fp);
	if (fclose(fp) != NULL || readSize != size)
	{
		buffer.reset(nullptr);
		size = 0;
		return false;
	}

	return true;
}

bool NtSystem::SaveFile(const NtWChar* fileName, const NtUChar* buffer, NtUInt size)
{
    if (buffer == 0 || size <= 0)
    {
		return false;
	}


    FILE* fp = NULL;
	errno_t err = _wfopen_s(&fp, fileName, L"wb");
	if (0 != err || NULL == fp)
	{
		return false;
	}

    size_t writeSize = fwrite(buffer, sizeof(NtUChar), size, fp);
    if (fclose(fp) != 0 || writeSize != size)
    {
        return false;
    }

    return true;
}

bool NtSystem::AppendFile(const NtWChar* fileName, NtUChar* buffer, NtUInt size)
{
    if (buffer == NULL || size <= 0)
    {
        return false;
    }

    FILE* fp = NULL;
	errno_t err = _wfopen_s(&fp, fileName, L"ab");
    if (0 != err || fp == NULL)
    {
        return false;
    }

    size_t writeSize = fwrite(buffer, sizeof(NtUChar), size, fp);
    if (fclose(fp) != 0 || writeSize != size)
    {
        return false;
    }

    return true;
}

int NtSystem::Read1Byte(const NtChar* buffer, int byteSize, void* getData)
{
    _ASSERT(buffer && byteSize > 0 && getData);
    memcpy(getData, buffer, byteSize);
    return byteSize;
}

int NtSystem::Write1Byte(NtChar *buffer, int byteSize, void *setData)
{
    _ASSERT(buffer && byteSize > 0 && setData);
    memcpy(buffer, setData, byteSize);
    return byteSize;
}

int NtSystem::Read2Byte(const NtChar* buffer, int byteSize, void* getData)
{
    _ASSERT(buffer && byteSize > 0 && getData);
    int cpySize = 2 * byteSize;
    memcpy(getData, buffer, cpySize);
    return cpySize;
}

int NtSystem::Write2Byte(NtChar *buffer, int byteSize, void *setData)
{
    _ASSERT(buffer && byteSize > 0 && setData);
    int cpySize = 2 * byteSize;
    memcpy(buffer, setData, byteSize);
    return cpySize;
}

int NtSystem::Read4Byte(const NtChar* buffer, int byteSize, void* getData)
{
    _ASSERT(buffer && byteSize > 0 && getData);
    int cpySize = 4 * byteSize;
    memcpy(getData, buffer, cpySize);
    return cpySize;
}

int NtSystem::Write4Byte(NtChar *buffer, int byteSize, void *setData)
{
    _ASSERT(buffer && byteSize > 0 && setData);
    int cpySize = 4 * byteSize;
    memcpy(buffer, setData, byteSize);
    return cpySize;
}

int NtSystem::Read8Byte(const NtChar* buffer, int byteSize, void* getData)
{
    _ASSERT(buffer && byteSize > 0 && getData);
    int cpySize = 8 * byteSize;
    memcpy(getData, buffer, cpySize);
    return cpySize;
}

int NtSystem::Write8Byte(NtChar *buffer, int byteSize, void *setData)
{
	_ASSERT(buffer && byteSize > 0 && setData);
	int cpySize = 8 * byteSize;
    memcpy(buffer, setData, byteSize);
    return cpySize;
}

}