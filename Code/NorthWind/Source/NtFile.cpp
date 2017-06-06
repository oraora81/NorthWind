	
#include "NtCoreLib.h"
#pragma hdrstop

namespace nt { namespace fs {

NtFile::NtFile()
	: m_mode(IO_READ)
	, m_size(0)
	, m_fileName()
	, m_fp(nullptr)
{
	
}

NtFile::~NtFile()
{
	if (m_fp != nullptr)
	{
		fclose(m_fp);
	}
}

bool NtFile::Execute(const ntWchar* fileName, IO_MODE mode /*= IO_READ*/)
{
	const ntWchar* fullPath = g_resManager->GetWholePath(fileName);
	if (nullptr == fullPath)
	{
		return false;
	}

	m_fileName = fullPath;
	m_mode = mode;

	const ntWchar* option = NULL;
	if (IO_READ == m_mode)
	{
		option = L"r";
	}
	else if (IO_READ_PLUS == m_mode)
	{
		option = L"r+";
	}
	else if (IO_WRITE == m_mode)
	{
		option = L"w";
	}
	else if (IO_WRITE_PLUS == m_mode)
	{
		option = L"w+";
	}
	else if (IO_APPEND == m_mode)
	{
		option = L"a";
	}
	else if (IO_APPEND_PLUS == m_mode)
	{
		option = L"a+";
	}

	if (option == NULL)
	{
		return false;
	}

	ERROR_CODE errCode = Crt::FOpen(m_fileName.Buffer(), option, m_fp);
	if (ERR_SUCCESS != errCode)
	{
		return false;
	}

	return true;
}


void NtFile::Write4Byte( ntWchar* buffer )
{
	fwrite(buffer, sizeof(ntWchar) * 4, 1, m_fp);
}


void NtFile::Read1Byte()
{
	Crt::MemSet(m_readBuffer, sizeof(m_readBuffer));

	fread_s(m_readBuffer, sizeof(m_readBuffer), sizeof(ntWchar) * 1, 1, m_fp);
}


void NtFile::Read4Byte()
{
	Crt::MemSet(m_readBuffer, sizeof(m_readBuffer));

	fread_s(m_readBuffer, sizeof(m_readBuffer), sizeof(ntWchar) * 4, 1, m_fp);
}

void NtFile::ReadBuffer( ntSize length /*= READ_BUF_SIZE*/ )
{
	if (length >= READ_BUF_SIZE)
	{
		return;
	}

	Crt::MemSet(m_readBuffer, sizeof(m_readBuffer));

	fread_s(m_readBuffer, sizeof(m_readBuffer), sizeof(ntWchar) * length, 1, m_fp);
}

bool NtFile::ReadLine()
{
	Crt::MemSet(m_readBuffer, sizeof(m_readBuffer));

	ntWchar* res = fgetws(m_readBuffer, _countof(m_readBuffer), m_fp);
	if (res == NULL)
	{
		// End of File
		return true;
	}
	NTRACE(L"%s\n", m_readBuffer);
	return true;
}

bool NtFile::ReadTag()
{
	Crt::MemSet(m_readBuffer, sizeof(m_readBuffer));

	ntInt res = fwscanf_s(m_fp, L"%s", m_readBuffer, _countof(m_readBuffer));
	if (res == EOF)
	{
		return false;
	}
	NTRACE(L"%s\n", m_readBuffer);

	return true;
}

ntInt NtFile::ReadInt()
{
	ntInt target = 0;
	ntInt res = fwscanf_s(m_fp, L"%d", &target);
	if (res == EOF)
	{
		return false;
	}

	NTRACE(L"%d\n", target);
	return target;
}

ntFloat NtFile::ReadFloat()
{
	ntFloat target = 0.0f;
	ntInt res = fwscanf_s(m_fp, L"%f", &target);
	if (res == EOF)
	{
		return false;
	}

	NTRACE(L"%f\n", target);
	return target;
}


bool NtFile::IsEOF()
{
	return feof(m_fp) != 0 ? true : false;
}


bool NtFile::IsOpen()
{
	return m_fp != NULL ? true : false;
}


void NtFile::ClearData()
{
	Crt::MemSet(m_readBuffer, sizeof(m_readBuffer));
}

}	// namespace fs
}	// namespace nt
