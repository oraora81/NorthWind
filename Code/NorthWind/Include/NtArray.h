
#pragma once

#include "NtAssert.h"

namespace nt {

template <typename T>
class NtArray
{
	enum { DEFAULT_ARRAY_LENGTH = 8 };

public:
	typedef T value_type;
	typedef NtArray<T> MyVec;
	typedef value_type& reference;
	typedef value_type* pointer;

public:
	NtArray()
	{
		m_tuple = new T[DEFAULT_ARRAY_LENGTH];
		Crt::MemSet(m_tuple, sizeof(T) * DEFAULT_ARRAY_LENGTH);

		m_first = &m_tuple[0];
		m_last = m_first;
		m_end = &m_tuple[DEFAULT_ARRAY_LENGTH - 1];
	}

	explicit NtArray(ntSize maxSize)
	{
		int needSize;
		if (maxSize <= DEFAULT_ARRAY_LENGTH)
		{
			needSize = DEFAULT_ARRAY_LENGTH;
		}
		else
		{
			needSize = maxSize;
		}

		m_tuple = new T[needSize];

		m_first = &m_tuple[0];
		m_last = m_first;
		m_end = &m_tuple[needSize - 1];
	}

	NtArray(const NtArray& rhsArray)
	{
		*this = rhsArray;
	}

	~NtArray()
	{
		Clear();
	}

private:
	void Resize()
	{
		if (m_end > ((T*)m_last + 1))
		{
			return;
		}

		ntSize length = ((m_end+1) - m_first);
		length = length << 1;

		if (length != 0)
		{
			T* newTuple = new T[length];
			Crt::MemCpy(newTuple, m_tuple, (ntUint)(sizeof(T) * (length >> 1)));

			Clear();
			m_tuple = newTuple;
			m_first = &m_tuple[0];
			m_last = (m_first + (length >> 1) - 1);
			m_end = &m_tuple[length - 1];
		}
	}

public:
	void Pushback(const T& value)
	{
		Resize();
		*m_last = value;
		++m_last;
	}
	
	void Pushback(T&& value)
	{
		Resize();

		*m_last = value;
		++m_last;
	}

	//void Insert(ntIndex index, T value)
	//{
	//	Resize();

	//	NtAsserte(index >= 0 && index < m_arraySize);
	//	// 하나씩 뒤로 밀어준다.
	//	for (ntIndex end = Size(); end > index; --end)
	//	{
	//		m_tuple[end] = m_tuple[end - 1];
	//	}
	//	m_tuple[index] = value;
	//	++m_arraySize;
	//}

	//void Erase(ntIndex index)
	//{
	//	NtAsserte(index >= 0 && index < m_arraySize);

	//	// 하나씩 당겨준다.
	//	for (ntIndex i = index; i < m_arraySize - 1; ++i)
	//	{
	//		m_tuple[i] = m_tuple[i + 1];
	//	}
	//	--m_arraySize;
	//}

	void Clear()
	{
		SAFE_DELETE_ARRAY(m_tuple);
	}

	T& operator[](ntIndex index)
	{
		NtAsserte(0 <= index && m_arraySize > index);

		return m_tuple[index];
	}

	const T& operator[](ntIndex index) const
	{
		NtAsserte(0 <= index && m_arraySize > index);

		return m_tuple[index];
	}

	T& At(ntIndex index)
	{
		NtAsserte(0 <= index && m_arraySize > index);

		return m_tuple[index];
	}

	ntSize	Size() const
	{
		return m_arraySize;
	}

	NtArray& operator=(const NtArray& rhs)
	{
		if (*this == rhs)
		{
			return *this;
		}

		Clear();

		m_arraySize = rhs.Size();
		m_tuple = m_arraySize ? new T[m_arraySize] : nullptr;

		for (ntIndex i = 0; i < m_arraySize; ++i)
		{
			m_tuple[i] = rhs.m_tuple[i];
		}

		return *this;
	}

private:
	T*  m_tuple;
	pointer m_first;
	pointer m_last;
	pointer m_end;
	ntSize	m_arraySize;
};

} // namespace nt
