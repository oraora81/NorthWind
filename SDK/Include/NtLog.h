
#pragma once

namespace nt
{

namespace LOG
{

// http://msdn.microsoft.com/en-us/library/yt0c3wdh.aspx
#ifdef _DEBUG
int DebugTrace(const ntWchar* msg, ...);
int DebugTrace(const ntChar* msg, ...);
#endif

// 
// Common Library
//----------------------------------------------------------------------------
class NtLog
{
public:

	enum NtLogType
	{
		TYPE_NONE = 0x00,
		TYPE_DBGSTR = 0x01,
		TYPE_CONSOLE = 0x02,
		TYPE_FILE = 0x04,
		//TYPE_WINDOW = 0x04,
		TYPE_MAX,
	};

	enum NtLogLevel
	{
		LEVEL_WARNING,		// 경고			> 콘솔
		LEVEL_ERROR,		// 오류			> 파일 + 콘솔
		LEVEL_CRITICAL,		// 치명적오류	> 파일 + 콘솔 + 종료
	};

	enum { DEFAULT_SUBJECT_COUNT = 4 };

	struct sLogItem
	{
		NtLogType logType;
		ntWchar* subject;
	};

public:
	NtLog();

	void AddSubject(ntWchar* subject);
	void Print(const ntWchar* subject, const ntWchar* format, ...);

private:
	NtLogType m_outputType;
	nt::NtHashTable<ntWchar*, sLogItem*> m_hashTable;

	bool m_supportThread;
};

}	// namespace LOG

}	// namespace nt

#ifdef _DEBUG
#define  NTRACE(msg, ...) \
	nt::LOG::DebugTrace(msg, __VA_ARGS__)
#else
__noop
#endif

