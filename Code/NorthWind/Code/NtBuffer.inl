
inline NT::FS::NtBuffer::eUsage NtBuffer::GetUsage() const
{
	return m_usage;
}

inline NtUInt NtBuffer::GetBytes() const
{
	return m_bytes;
}

inline const NtUChar* NtBuffer::GetData() const
{
	return m_data.get();
}

inline NtUChar* NtBuffer::GetData()
{
	return m_data.get();
}