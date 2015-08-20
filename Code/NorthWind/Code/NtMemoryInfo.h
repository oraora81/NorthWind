
#pragma once

namespace nt
{

class NtMemoryInfo
{
public:
	struct ntAllocUnit
	{
		ntSize		m_bytes;
		ntUint		m_pool;
		ntUint		m_line;
		NtString	m_fileName;
		NtString	m_function;

		ntAllocUnit() : m_bytes(0), m_line(0) {}
		ntAllocUnit(ntSize size, ntUint pool, const ntWchar* fileName, ntUint line, const ntWchar* func)
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