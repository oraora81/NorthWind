
#pragma once

#include "NtAssert.h"

namespace NT
{

template <typename T>
class NtArray
{
public:
    explicit NtArray(NtSize maxSize);
    NtArray(const NtArray& rhsArray);
    ~NtArray();

    void Push_back(T value);
    void Insert(NtIndex index, T value);

    void Erase(NtIndex index);
    void Clear();

    T& operator[](NtIndex index);
	const T& operator[](NtIndex index) const;
    T& At(NtIndex index);

    NtSize	Size() const;
	NtSize	MaxSize() const;

    NtArray& operator =(const NtArray& rhsArray);

private:
    void Resize();

private:
    NtSize m_arraySize;
    NtSize m_maxArraySize;
    T*  m_array;
};

#include "NtArray.inl"

} // namespace NT