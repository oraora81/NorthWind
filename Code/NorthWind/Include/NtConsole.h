
#pragma once

namespace nt { namespace LOG {

class NtConsole
{
public:
	static bool Initialize(const ntWchar* subject);
	static void Print(const NtString& str);

private:
	static nt::FS::NtFile* ms_file;
	static HANDLE ms_handle;
};

}	// namespace LOG
}	// namespace nt
