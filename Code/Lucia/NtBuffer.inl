
inline NtInt NtBuffer::GetNumElem() const
{
	return m_numElem;
}

inline NtInt NtBuffer::GetElemSize() const
{
	return m_elemSize;
}

inline NT::_dx11::NtBuffer::eUsage NtBuffer::GetUsage() const
{
	return m_usage;
}

inline NtInt NtBuffer::GetBytes() const
{
	return m_bytes;
}

inline const NtChar* NtBuffer::GetData() const
{
	return m_data;
}
