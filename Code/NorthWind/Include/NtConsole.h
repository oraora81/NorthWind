
#pragma once

namespace nt { namespace log {

class NtConsole
{
public:
	static bool Initialize(const ntWchar* subject);
	static void Print(const NtString& str);

private:
	static nt::fs::NtFile* ms_file;
	static HANDLE ms_handle;
};

}	// namespace log
}	// namespace nt
