	
#include "NtCoreLib.h"
#pragma hdrstop

namespace NT
{
namespace FS
{

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

bool NtFile::Execute(const NtWChar* fileName, IO_MODE mode /*= IO_READ*/)
{
	const NtWChar* fullPath = g_resManager->GetWholePath(fileName);
	if (nullptr == fullPath)
	{
		return false;
	}

	m_fileName = fullPath;
	m_mode = mode;

	const NtWChar* option = NULL;
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


void NtFile::Write4Byte( NtWChar* buffer )
{
	fwrite(buffer, sizeof(NtWChar) * 4, 1, m_fp);
}


void NtFile::Read1Byte()
{
	Crt::MemSet(m_readBuffer, sizeof(m_readBuffer));

	fread_s(m_readBuffer, sizeof(m_readBuffer), sizeof(NtWChar) * 1, 1, m_fp);
}


void NtFile::Read4Byte()
{
	Crt::MemSet(m_readBuffer, sizeof(m_readBuffer));

	fread_s(m_readBuffer, sizeof(m_readBuffer), sizeof(NtWChar) * 4, 1, m_fp);
}

void NtFile::ReadBuffer( NtSize length /*= READ_BUF_SIZE*/ )
{
	if (length >= READ_BUF_SIZE)
	{
		return;
	}

	Crt::MemSet(m_readBuffer, sizeof(m_readBuffer));

	fread_s(m_readBuffer, sizeof(m_readBuffer), sizeof(NtWChar) * length, 1, m_fp);
}

bool NtFile::ReadLine()
{
	Crt::MemSet(m_readBuffer, sizeof(m_readBuffer));

	NtWChar* res = fgetws(m_readBuffer, _countof(m_readBuffer), m_fp);
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

	NtInt res = fwscanf_s(m_fp, L"%s", m_readBuffer, _countof(m_readBuffer));
	if (res == EOF)
	{
		return false;
	}
	NTRACE(L"%s\n", m_readBuffer);

	return true;
}

NT::NtInt NtFile::ReadInt()
{
	NtInt target = 0;
	NtInt res = fwscanf_s(m_fp, L"%d", &target, sizeof(target));
	if (res == EOF)
	{
		return false;
	}

	NTRACE(L"%d\n", target);
	return target;
}

NT::NtFloat NtFile::ReadFloat()
{
	NtFloat target = 0.0f;
	NtInt res = fwscanf_s(m_fp, L"%f", &target, sizeof(target));
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

}	// namespace FS
}	// namespace NT
