
#pragma once

#include "NtAssert.h"

namespace nt {

template <typename T>
class NtArray
{
public:
    explicit NtArray(ntSize maxSize);
    NtArray(const NtArray& rhsArray);
    ~NtArray();

    void Push_back(T value);
    void Insert(ntIndex index, T value);

    void Erase(ntIndex index);
    void Clear();

    T& operator[](ntIndex index);
	const T& operator[](ntIndex index) const;
    T& At(ntIndex index);

    ntSize	Size() const;
	ntSize	MaxSize() const;

    NtArray& operator =(const NtArray& rhsArray);

private:
    void Resize();

private:
    ntSize m_arraySize;
    ntSize m_maxArraySize;
    T*  m_array;
};

#include "NtArray.inl"

} // namespace nt
