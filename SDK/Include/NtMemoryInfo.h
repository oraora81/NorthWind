
#pragma once

namespace NT
{

class NtMemoryInfo
{
public:
	struct ntAllocUnit
	{
		NtSize		m_bytes;
		NtUInt		m_pool;
		NtUInt		m_line;
		NtString	m_fileName;
		NtString	m_function;

		ntAllocUnit() : m_bytes(0), m_line(0) {}
		ntAllocUnit(NtSize size, NtUInt pool, const NtWChar* fileName, NtUInt line, const NtWChar* func)
			: m_bytes(size)
			, m_pool(pool)
			, m_line(line)
		{
			if (fileName)
			{
				m_fileName = fileName;
			}
			if (func)
			{
				m_function = func;
			}
		}

		// NtString m_leakFileName;
		bool m_dumpToStdout;


	};
};

}