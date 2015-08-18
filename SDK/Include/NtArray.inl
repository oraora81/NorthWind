
template <typename T>
NtArray<T>::NtArray(ntSize maxSize)
{
    NtAsserte(maxSize >= 0);

    m_arraySize = 0;
    m_maxArraySize = maxSize;

    m_array = m_maxArraySize ? new T[m_maxArraySize] : NULL;
}

template <typename T>
NtArray<T>::NtArray(const NtArray& rhsArray)
{
    *this = rhsArray;
}

template <typename T>
NtArray<T>::~NtArray()
{
    Clear();
}

template <typename T>
NtArray<T>& NtArray<T>::operator=(const NtArray& rhsArray)
{
    if (*this == rhsArray)
    {
        return *this;
    }

    Clear();

    m_maxArraySize  = rhsArray.MaxSize();
    m_arraySize     = rhsArray.Size();
    m_array         = m_maxArraySize ? new T [m_maxArraySize] : nullptr;

    for (ntIndex i = 0; i < m_arraySize; ++i)
    {
        m_array[i] = rhsArray.m_array[i];
    }

    return *this;
}

template <typename T>
T& NtArray<T>::operator [](ntIndex index)
{
    NtAsserte(0 <= index && m_arraySize > index);

    return m_array[index];
}

template <typename T>
const T& NtArray::operator[](ntIndex index) const
{
	NtAsserte(0 <= index && m_arraySize > index);

	return m_array[index];
}

template <typename T>
T& NtArray<T>::At(ntIndex index)
{
    NtAsserte(0 <= index && m_arraySize > index);

    return m_array[index];
}

template <typename T>
void NtArray<T>::Clear()
{
	Safe_Delete_Array(m_array);
    m_arraySize     = 0;
    m_maxArraySize  = 0;
}

template <typename T>
void NtArray<T>::Resize()
{
    if ((m_arraySize+1) < m_maxArraySize)
    {
        return;
    }

    m_maxArraySize *= 2;
    T* tempArray = new T [m_maxArraySize];

    for (ntInt i = 0; i < m_arraySize; ++i)
    {
        tempArray[i] = m_array[i];
    }

    Clear();

    m_array = tempArray;
}

template <typename T>
void NtArray<T>::Push_back(T value)
{
    Resize();
    
    m_array[m_arraySize++] = value;
}

template <typename T>
void NtArray<T>::Insert(ntIndex index, T value)
{
    Resize();

    NtAsserte(index >= 0 && index < m_arraySize);
    // 하나씩 뒤로 밀어준다.
    for (ntIndex end = m_arraySize; end > index; --end)
    {
        m_array[end] = m_array[end-1];
    }
    m_array[index] = value;
    ++m_arraySize;
}

template <typename T>
void NtArray<T>::Erase(ntIndex index)
{
	NtAsserte(index >= 0 && index < m_arraySize);

    // 하나씩 당겨준다.
    for (ntIndex i = index; i < m_arraySize-1; ++i)
    {
        m_array[i] = m_array[i+1];
    }
    --m_arraySize;
}