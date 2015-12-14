
//
template <typename T>
T* DynamicCast(NtObject* obj)
{
	return obj && obj->IsDerived(T::m_RTTI) ? (T*)obj : nullptr;
}

template <typename T>
const T* DynamicCast(const NtObject* obj)
{
	return obj && obj->IsDerived(T::m_RTTI) ? (const T*)obj : nullptr;
}