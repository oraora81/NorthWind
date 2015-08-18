
#pragma once

namespace NT
{

namespace FS
{

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

	bool Execute(const NtWChar* fileName, IO_MODE mode = IO_READ);

	void Write4Byte(NtWChar* buffer);
	void Read1Byte();
	void Read4Byte();
	void ReadBuffer(NtSize length = READ_BUF_SIZE);

	bool	ReadLine();
	bool	ReadTag();
	NtInt	ReadInt();
	NtFloat ReadFloat();

	bool IsEOF();
	bool IsOpen();
	void ClearData();
	const NtWChar* GetData()	{ return m_readBuffer; }
	NtBuffer* GetBuffer()		{ return m_buffer; }

	enum { READ_BUF_SIZE = 256 };

private:
	IO_MODE			m_mode;
	NtSize			m_size;
	NtFileBuffer*	m_buffer;
	NtString		m_fileName;

	FILE*			m_fp;
	NtWChar			m_readBuffer[READ_BUF_SIZE];
};


}	// namespace FS
}	// namespace NT
