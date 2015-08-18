
#pragma once

#include <memory>

namespace NT
{
	void SystemAbort(NtInt exitCode);
	void SystemExit(NtInt exitCode);

class NtSystem
{
public:
	static bool ReadFileSize(const NtWChar* fileName, NtUInt& size);
    static bool LoadFile(const NtWChar* fileName, std::unique_ptr<NtUChar[]>& buffer, NtUInt& size);
    static bool SaveFile(const NtWChar* fileName, const NtUChar* buffer, NtUInt size);
    static bool AppendFile(const NtWChar* fileName, NtUChar* buffer, NtUInt size);
    
    static int Read1Byte(const NtChar* buffer, int byteSize, void* getData);
    static int Write1Byte(NtChar* buffer, int byteSize, void* setData);

    static int Read2Byte(const NtChar* buffer, int byteSize, void* getData);
    static int Write2Byte(NtChar* buffer, int byteSize, void* setData);

    static int Read4Byte(const NtChar* buffer, int byteSize, void* getData);
    static int Write4Byte(NtChar* buffer, int byteSize, void* setData);

    static int Read8Byte(const NtChar* buffer, int byteSize, void* getData);
    static int Write8Byte(NtChar* buffer, int byteSize, void* setData);

	
private:
    enum { NTSYSTEM_MAX_PATH = 1024 };
    static char m_path[NTSYSTEM_MAX_PATH];
};

template <typename T>
static void AllocateMem(int column, int row, T** allArray);
template <typename T>
static void DeAllocateMem(T**& allArray);

#include "NtSystem.inl"

}