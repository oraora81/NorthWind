
inline nt::fs::NtBuffer::eUsage NtBuffer::GetUsage() const
{
	return m_usage;
}

inline ntSize NtBuffer::GetBytes() const
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
