
#include "NtCoreLib.h"
#include <sys/stat.h>
#include <crtdbg.h>

namespace nt {

void SystemAbort(ntInt exitCode)
{
    ::exit(exitCode);
}

void SystemExit(ntInt exitCode)
{
    ::PostQuitMessage(exitCode);
}


bool NtSystem::ReadFileSize(const ntWchar* fileName, ntSize& size)
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


bool NtSystem::LoadFile(const ntWchar* fileName, std::unique_ptr<ntUchar[]>& buffer, ntSize& size)
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
    buffer.reset(new ntUchar[size]);
    nt::Crt::MemSet(buffer.get(), sizeof(ntUchar) * size);

    ntUint readSize = (ntUint)fread(buffer.get(), sizeof(ntUchar), size, fp);
    if (fclose(fp) != NULL || readSize != size)
    {
        buffer.reset(nullptr);
        size = 0;
        return false;
    }

    return true;
}

bool NtSystem::SaveFile(const ntWchar* fileName, const ntUchar* buffer, ntSize size)
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

    size_t writeSize = fwrite(buffer, sizeof(ntUchar), size, fp);
    if (fclose(fp) != 0 || writeSize != size)
    {
        return false;
    }

    return true;
}

bool NtSystem::AppendFile(const ntWchar* fileName, ntUchar* buffer, ntSize size)
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

    size_t writeSize = fwrite(buffer, sizeof(ntUchar), size, fp);
    if (fclose(fp) != 0 || writeSize != size)
    {
        return false;
    }

    return true;
}

int NtSystem::Read1Byte(const ntChar* buffer, int byteSize, void* getData)
{
    _ASSERT(buffer && byteSize > 0 && getData);
    memcpy(getData, buffer, byteSize);
    return byteSize;
}

int NtSystem::Write1Byte(ntChar *buffer, int byteSize, void *setData)
{
    _ASSERT(buffer && byteSize > 0 && setData);
    memcpy(buffer, setData, byteSize);
    return byteSize;
}

int NtSystem::Read2Byte(const ntChar* buffer, int byteSize, void* getData)
{
    _ASSERT(buffer && byteSize > 0 && getData);
    int cpySize = 2 * byteSize;
    memcpy(getData, buffer, cpySize);
    return cpySize;
}

int NtSystem::Write2Byte(ntChar *buffer, int byteSize, void *setData)
{
    _ASSERT(buffer && byteSize > 0 && setData);
    int cpySize = 2 * byteSize;
    memcpy(buffer, setData, byteSize);
    return cpySize;
}

int NtSystem::Read4Byte(const ntChar* buffer, int byteSize, void* getData)
{
    _ASSERT(buffer && byteSize > 0 && getData);
    int cpySize = 4 * byteSize;
    memcpy(getData, buffer, cpySize);
    return cpySize;
}

int NtSystem::Write4Byte(ntChar *buffer, int byteSize, void *setData)
{
    _ASSERT(buffer && byteSize > 0 && setData);
    int cpySize = 4 * byteSize;
    memcpy(buffer, setData, byteSize);
    return cpySize;
}

int NtSystem::Read8Byte(const ntChar* buffer, int byteSize, void* getData)
{
    _ASSERT(buffer && byteSize > 0 && getData);
    int cpySize = 8 * byteSize;
    memcpy(getData, buffer, cpySize);
    return cpySize;
}

int NtSystem::Write8Byte(ntChar *buffer, int byteSize, void *setData)
{
    _ASSERT(buffer && byteSize > 0 && setData);
    int cpySize = 8 * byteSize;
    memcpy(buffer, setData, byteSize);
    return cpySize;
}

}
