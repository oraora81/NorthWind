
#pragma once

namespace NT
{

namespace LOG
{

// http://msdn.microsoft.com/en-us/library/yt0c3wdh.aspx
#ifdef _DEBUG
int DebugTrace(const ntWchar* msg, ...);
int DebugTrace(const ntChar* msg, ...);
#endif


enum NtLogType
{
	TYPE_CONSOLE,
	TYPE_FILE,
	TYPE_WINDOW,
	TYPE_MAX,
};

enum NtLogLevel
{
	LEVEL_WARNING,		// 경고			> 콘솔
	LEVEL_ERROR,		// 오류			> 파일 + 콘솔
	LEVEL_CRITICAL,		// 치명적오류	> 파일 + 콘솔 + 종료
};

typedef NtLogType LOG_TYPE;


/*
	
 */
class NtRealWriter
{
public:
	NtRealWriter() {}
	~NtRealWriter() {}

	virtual void Write() = 0;
};

class NtLogWriter
{
protected:
	NtLogWriter()
		: m_writer(nullptr)
	{

	}
	virtual	~NtLogWriter() {}

public:
	void Textout()
	{
		NtAsserte(m_writer != nullptr);
		m_writer->Write();
	}

	NtInline	void			WriteName(ntWchar* name)	{	NtAsserte(name != nullptr);		m_writeName = name; }
	NtInline	const ntWchar*	WriteName() const			{	return m_writeName.c_str();		}
	NtInline	void			Writer(NtRealWriter* writer){	NtAsserte(writer != nullptr);	m_writer = writer;	}

private:
	std::wstring	m_writeName;
	NtRealWriter*	m_writer;
};


/*
	
 */
class NtLOGConsole : public NtLogWriter
{
	enum { MAX_BUFFER = 100 };
public:
	NtLOGConsole();
	~NtLOGConsole();

private:
	const ntWchar*	m_textBuffer[MAX_BUFFER];
	ntInt			m_bufferIndex;
};


class NtLOGFile : public NtLogWriter
{
public:
	NtLOGFile();
	~NtLOGFile();
};
//
//
class NtLOG
{
public:
	NtLOG();
	~NtLOG();

	void	WriteLog();
private:
};

}	// namespace LOG

}	// namespace NT

#ifdef _DEBUG
#define  NTRACE(msg, ...) \
	NT::LOG::DebugTrace(msg, __VA_ARGS__)
#else
__noop
#endif

