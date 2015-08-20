
inline nt::FS::NtBuffer::eUsage NtBuffer::GetUsage() const
{
	return m_usage;
}

inline ntUint NtBuffer::GetBytes() const
{
	return m_bytes;
}

inline const ntUchar* NtBuffer::GetData() const
{
	return m_data.get();
}

inline ntUchar* NtBuffer::GetData()
{
	return m_data.get();
}