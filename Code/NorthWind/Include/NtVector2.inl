
template <typename T>
NtVector2<T>::NtVector2()
{

}

template <typename T>
NtVector2<T>::NtVector2(T x, T y)
{
    m_tuple[0] = x;
    m_tuple[1] = y;
}

template <typename T>
NtVector2<T>::NtVector2(const NtVector2& vec)
{
    m_tuple[0] = vec.m_tuple[0];
    m_tuple[1] = vec.m_tuple[1];
}

template <typename T>
NtVector2<T>::operator const T*() const
{
    return m_tuple;
}

template <typename T>
NtVector2<T>::operator T*()
{
    return m_tuple;
}

template <typename T>
T NtVector2<T>::operator [](int index) const
{
    assert(0 <= index && index < 2);
    return m_tuple[index];
}

template <typename T>
T& NtVector2<T>::operator [](int index)
{
    assert(0 <= index && index < 2);
    return m_tuple[index];
}

template <typename T>
T NtVector2<T>::X() const
{
    return m_tuple[0];
}

template <typename T>
T& NtVector2<T>::X()
{
    return m_tuple[0];
}

template <typename T>
T NtVector2<T>::Y() const
{
    return m_tuple[1];
}

template <typename T>
T& NtVector2<T>::Y()
{
    return m_tuple[1];
}

template <typename T>
NtVector2<T>& NtVector2<T>::operator =(const NtVector2<T>& vec)
{
    m_tuple[0] = vec.m_tuple[0];
    m_tuple[1] = vec.m_tuple[1];

    return *this;
}

template <typename T>
int NtVector2<T>::CompareArray(const NtVector2& vec) const
{
    return memcmp(m_tuple, vec.m_tuple, sizeof(T)*2);
}

template <typename T>
bool NtVector2<T>::operator ==(const NtVector2& vec) const
{
    return CompareArray(vec) == 0;
}

template <typename T>
bool NtVector2<T>::operator !=(const NtVector2& vec) const
{
    return CompareArray(vec) != 0;
}

template <typename T>
bool NtVector2<T>::operator <(const NtVector2& vec) const
{
    return CompareArray(vec) < 0;
}

template <typename T>
bool NtVector2<T>::operator <=(const NtVector2& vec) const
{
    return CompareArray(vec) <= 0;
}

template <typename T>
bool NtVector2<T>::operator >(const NtVector2& vec) const
{
    return CompareArray(vec) > 0;
}

template <typename T>
bool NtVector2<T>::operator >=(const NtVector2& vec) const
{
    return CompareArray(vec) >= 0;
}

template <typename T>
NtVector2<T> NtVector2<T>::operator +(const NtVector2& vec) const
{
    return NtVector2(m_tuple[0] + vec.m_tuple[0], m_tuple[1] + vec.m_tuple[1]);
}

template <typename T>
NtVector2<T> NtVector2<T>::operator -(const NtVector2& vec) const
{
    return NtVector2(m_tuple[0] - vec.m_tuple[0], m_tuple[1] - vec.m_tuple[1]);
}

template <typename T>
NtVector2<T> NtVector2<T>::operator *(T scalar) const
{
    return NtVector2(m_tuple[0] * scalar, m_tuple[1] * scalar);
}

template <typename T>
NtVector2<T> NtVector2<T>::operator /(T scalar) const
{
    NtVector2 tpVec;
    if (scalar != (T)0.0)
    {
        T invScalar = ((T)1.0) / scalar;
        tpVec.m_tuple[0] = invScalar * m_tuple[0];
        tpVec.m_tuple[1] = invScalar * m_tuple[1];
    }
    else
    {
        tpVec.m_tuple[0] = NtMath<T>::MAX_VALUE;
        tpVec.m_tuple[1] = NtMath<T>::MAX_VALUE;
    }

    return tpVec;
}

template <typename T>
NtVector2<T> NtVector2<T>::operator -() const
{
    return NtVector2(-m_tuple[0], -m_tuple[1]);
}

template <typename T>
NtVector2<T>& NtVector2<T>::operator +=(const NtVector2& vec)
{
    m_tuple[0] += vec.m_tuple[0];
    m_tuple[1] += vec.m_tuple[1];
    return *this;
}

template <typename T>
NtVector2<T>& NtVector2<T>::operator -=(const NtVector2<T>& vec)
{
    m_tuple[0] -= vec.m_tuple[0];
    m_tuple[1] -= vec.m_tuple[1];
    return *this;
}

template <typename T>
NtVector2<T>& NtVector2<T>::operator *=(T scalar)
{
    m_tuple[0] *= scalar;
    m_tuple[1] *= scalar;
    return *this;
}

template <typename T>
NtVector2<T>& NtVector2<T>::operator /=(T scalar)
{
    if (scalar != (T)0.0)
    {
        T invScalar = ((T)1.0) / scalar;
        m_tuple[0] *= invScalar;
        m_tuple[1] *= invScalar;
    }
    else
    {
        m_tuple[0] = NtMath<T>::MAX_VALUE;
        m_tuple[1] = NtMath<T>::MAX_VALUE;
    }
}

//

template <typename T>
T NtVector2<T>::Length() const
{
    return NtMath<T>::Sqrt(m_tuple[0] * m_tuple[0] + m_tuple[1] * m_tuple[1]);
}

template <typename T>
T NtVector2<T>::SquareLength() const
{
    return m_tuple[0] * m_tuple[0] + m_tuple[1] * m_tuple[1];
}

template <typename T>
T NtVector2<T>::DotProduct(const NtVector2& vec) const
{
    return m_tuple[0] * vec.m_tuple[0] + m_tuple[1] * vec.m_tuple[1];
}

template <typename T>
T NtVector2<T>::Normalize()
{
    T length = Length();
    if (length > NtMath<T>::TOLERANCE)
    {
        T invLength = ((T)1.0) / length;
        m_tuple[0] *= invLength;
        m_tuple[1] *= invLength;
    }
    else
    {
        length = (T)0.0;
        m_tuple[0] = (T)0.0;
        m_tuple[1] = (T)0.0;
    }

    return length;
}

template <typename T>
NtVector2<T> NtVector2<T>::Perp() const
{
    return NtVector2(m_tuple[1], -m_tuple[0]);
}

template <typename T>
NtVector2<T> NtVector2<T>::UnitPerp() const
{
    NtVector2 perp(m_tuple[1], -m_tuple[0]);
    perp.Normalize();

    return perp;
}

template <typename T>
T NtVector2<T>::DotProductPerp(const NtVector2& vec) const
{
    return m_tuple[0] * vec.m_tuple[1] - m_tuple[1] * vec.m_tuple[0];
}

template <typename T>
void NtVector2<T>::Orthonormalize(NtVector2& u, NtVector2& v)
{
    // 입력 벡터 v0, v1가 있다면 그람슈미츠 평면정규화를 통해 다음의 두 벡터를
    // 만들어 낼 수 있다.
    // u0 = v0 / |v0|
    // u1 = (v1 - (u0*v1)u0) / |v1 - (u0*v1)u0|
    // 절대값 |A| 벡터 A의 길이를 가리키며, A*B는 벡터 A와 B의 내적을 나타낸다.

    //
    u.Normalize();

    T dotProduct = u.DotProduct(v);
    v -= u*dotProduct;
    v.Normalize();
}

template <typename T>
void NtVector2<T>::GenerateOrthonormalBase(NtVector2<T>& u, NtVector2<T>& v, bool unitLengthV)
{
    if (false == unitLengthV)
    {
        v.Normalize();
    }
    u = v.Perp();
}