
#pragma once

#include <memory>

namespace nt
{
	void SystemAbort(ntInt exitCode);
	void SystemExit(ntInt exitCode);

class NtSystem
{
public:
	static bool ReadFileSize(const ntWchar* fileName, ntUint& size);
    static bool LoadFile(const ntWchar* fileName, std::unique_ptr<ntUchar[]>& buffer, ntUint& size);
    static bool SaveFile(const ntWchar* fileName, const ntUchar* buffer, ntUint size);
    static bool AppendFile(const ntWchar* fileName, ntUchar* buffer, ntUint size);
    
    static int Read1Byte(const ntChar* buffer, int byteSize, void* getData);
    static int Write1Byte(ntChar* buffer, int byteSize, void* setData);

    static int Read2Byte(const ntChar* buffer, int byteSize, void* getData);
    static int Write2Byte(ntChar* buffer, int byteSize, void* setData);

    static int Read4Byte(const ntChar* buffer, int byteSize, void* getData);
    static int Write4Byte(ntChar* buffer, int byteSize, void* setData);

    static int Read8Byte(const ntChar* buffer, int byteSize, void* getData);
    static int Write8Byte(ntChar* buffer, int byteSize, void* setData);

	
private:
    enum { NTSYSTEM_MAX_PATH = 1024 };
    static char m_path[NTSYSTEM_MAX_PATH];
};

template <typename T>
static void AllocateMem(int column, int row, T** allArray);
template <typename T>
static void DeAllocateMem(T**& allArray);

#include "NtSystem.inl"

} // namespace nt
