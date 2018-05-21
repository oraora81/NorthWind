
#pragma once

namespace nt { namespace fs {

enum eIOMode
{
	IO_READ,
	IO_WRITE,
	IO_APPEND,
	IO_READ_B,
	IO_WRITE_B,
};
typedef eIOMode IO_MODE;


class NtFileBuffer;

class NtFile
{
public:
	NtFile();
	
	virtual ~NtFile();

	bool Execute(const ntWchar* fileName, IO_MODE mode = IO_READ);

	bool Open(const ntWchar* fileName, IO_MODE mode);

	void Write4Byte(ntWchar* buffer);

	void Read1Byte();

	void Read4Byte();

	void ReadBuffer(ntSize length = READ_BUF_SIZE);

	bool ReadLine();

	bool ReadTag();

	ntInt ReadInt();

	float ReadFloat();

	bool IsEOF() const;

	bool IsOpen() const;

	void ClearData();

	const ntWchar* GetData()	{ return m_readBuffer; }

	NtBuffer* GetBuffer()		{ return m_buffer; }

	enum { READ_BUF_SIZE = 256 };

private:
	IO_MODE			m_mode;
	ntSize			m_size;
	NtFileBuffer*	m_buffer;
	NtString		m_fileName;

	FILE*			m_fp;
	ntWchar			m_readBuffer[READ_BUF_SIZE];
};


} }
