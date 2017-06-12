
#pragma once

namespace nt { namespace fs {

enum eIOMode
{
	IO_READ,
	IO_READ_PLUS,
	IO_WRITE,
	IO_WRITE_PLUS,
	IO_APPEND,
	IO_APPEND_PLUS,
};
typedef eIOMode IO_MODE;


class NtFileBuffer;

class NtFile
{
public:
	NtFile();
	virtual ~NtFile();

	bool Execute(const ntWchar* fileName, IO_MODE mode = IO_READ);

	void Write4Byte(ntWchar* buffer);
	void Read1Byte();
	void Read4Byte();
	void ReadBuffer(ntSize length = READ_BUF_SIZE);

	bool	ReadLine();
	bool	ReadTag();
	ntInt	ReadInt();
	ntFloat ReadFloat();

	bool IsEOF();
	bool IsOpen();
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

