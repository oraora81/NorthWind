
template <typename T>
NtMatrix2<T>::NtMatrix2(bool isZero)
{
    if (isZero == true)
    {
        MakeZeroMatrix();
    }
    else
    {
        MakeIdentity();
    }
}

template <typename T>
NtMatrix2<T>::NtMatrix2(const NtMatrix2& mat)
{
    memcpy(m_tuple, mat.m_tuple, sizeof(T) * 4);
}

template <typename T>
NtMatrix2<T>::NtMatrix2(T i00, T i01, T i10, T i11)
{
    m_tuple[0] = i00;
    m_tuple[1] = i01;
    m_tuple[2] = i10;
    m_tuple[3] = i11;
}

// rowMajor - true  : tuples[0~3] = {00, 01, 10, 11}
//          - false : tuples[0~3] = {00, 10, 01, 11}
template <typename T>
NtMatrix2<T>::NtMatrix2(const T tuples[4], bool rowMajor)
{
    if (rowMajor == true)
    {
        memcpy(m_tuple, tuples, sizeof(T) * 4);
    }
    else
    {
        m_tuple[0] = tuples[0];
        m_tuple[1] = tuples[2];
        m_tuple[2] = tuples[1];
        m_tuple[3] = tuples[3];
    }
}

template <typename T>
NtMatrix2<T>::NtMatrix2(const NtVector2<T>& U, const NtVector2<T> V, bool col)
{
    if (col == true)
    {
        m_tuple[0] = U[0];
        m_tuple[1] = V[0];
        m_tuple[2] = U[1];
        m_tuple[3] = V[1];
    }
    else
    {
        m_tuple[0] = U[0];
        m_tuple[1] = U[1];
        m_tuple[2] = V[0];
        m_tuple[3] = V[1];
    }
}

template <typename T>
NtMatrix2<T>::NtMatrix2(const NtVector2<T>* V, bool isColumn)
{
    if (isColumn == true)
    {
        m_tuple[0] = V[0][0];
        m_tuple[1] = V[1][0];
        m_tuple[2] = V[0][1];
        m_tuple[3] = V[1][1];
    }
    else
    {
        m_tuple[0] = V[0][0];
        m_tuple[1] = V[0][1];
        m_tuple[2] = V[1][0];
        m_tuple[3] = V[1][1];        
    }
}

template <typename T>
NtMatrix2<T>::NtMatrix2(T i00, T i11)
{
    MakeDiagonal(i00, i11);
}

template <typename T>
NtMatrix2<T>::NtMatrix2(T angle)
{

}

// create tensor product
template <typename T>
NtMatrix2<T>::NtMatrix2(const NtVector2<T>& U, const NtVector2<T>& V);



template <typename T>
void NtMatrix2<T>::MakeZeroMatrix()
{
    ZeroMemory(m_tuple, sizeof(T) * 4);
}

template <typename T>
void NtMatrix2<T>::MakeIdentity()
{
    m_tuple[0] = (T)1.0;
    m_tuple[1] = (T)0.0;
    m_tuple[2] = (T)0.0;
    m_tuple[3] = (T)1.0;
}

template <typename T>
void NtMatrix2<T>::MakeDiagonal(T i00, T i11)
{
    m_tuple[0] = i00;
    m_tuple[1] = (T)0.0;
    m_tuple[2] = (T)0.0;
    m_tuple[3] = i11;
}