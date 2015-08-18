
template <typename T>
NtVector3<T>::NtVector3()
{

}

template <typename T>
NtVector3<T>::NtVector3(const NtVector3& vec)
{
    m_tuple[0] = vec.m_tuple[0];
    m_tuple[1] = vec.m_tuple[1];
    m_tuple[2] = vec.m_tuple[2];
}

template <typename T>
NtVector3<T>::NtVector3(T x, T y, T z)
{
    m_tuple[0] = x;
    m_tuple[1] = y;
    m_tuple[2] = z;
}

template <typename T>
NtVector3<T>::NtVector3(T arr[3])
{
	m_tuple[0] = arr[0];
	m_tuple[1] = arr[1];
	m_tuple[2] = arr[2];
}


// 접근
template <typename T>
NtVector3<T>::operator const T*() const
{
    return m_tuple;
}

template <typename T>
NtVector3<T>::operator T*()
{
    return m_tuple;
}

template <typename T>
T NtVector3<T>::operator[](int index) const
{
    return m_tuple[index];
}

template <typename T>
T& NtVector3<T>::operator[](int index)
{
    return m_tuple[index];
}

template <typename T>
T NtVector3<T>::X() const
{
    return m_tuple[0];
}

template <typename T>
T& NtVector3<T>::X()
{
    return m_tuple[0];
}

template <typename T>
T NtVector3<T>::Y() const
{
    return m_tuple[1];
}

template <typename T>
T& NtVector3<T>::Y()
{
    return m_tuple[1];
}

template <typename T>
T NtVector3<T>::Z() const
{
    return m_tuple[2];
}

template <typename T>
T& NtVector3<T>::Z()
{
    return m_tuple[2];
}

// 할당
template <typename T>
NtVector3<T>& NtVector3<T>::operator=(const NtVector3& vec)
{
    m_tuple[0] = vec.m_tuple[0];
    m_tuple[1] = vec.m_tuple[1];
    m_tuple[2] = vec.m_tuple[2];

    return *this;
}

template <typename T>
int NtVector3<T>::CompareTuple(const NtVector3& vec) const
{
    return memcmp(m_tuple, vec.m_tuple, sizeof(T)*3);
}

template <typename T>
bool NtVector3<T>::operator==(const NtVector3& vec) const
{
    return CompareTuple(vec) == 0;
}

template <typename T>
bool NtVector3<T>::operator!=(const NtVector3& vec) const
{
    return CompareTuple(vec) != 0;
}

template <typename T>
bool NtVector3<T>::operator<(const NtVector3& vec) const
{
    return CompareTuple(vec) < 0;
}

template <typename T>
bool NtVector3<T>::operator<=(const NtVector3& vec) const
{
    return CompareTuple(vec) <= 0;
}

template <typename T>
bool NtVector3<T>::operator>(const NtVector3& vec) const
{
    return CompareTuple(vec) > 0;
}

template <typename T>
bool NtVector3<T>::operator>=(const NtVector3& vec) const
{
    return CompareTuple(vec) >= 0;
}

template <typename T>
NtVector3<T> NtVector3<T>::operator+(const NtVector3& vec) const
{
    return NtVector3(m_tuple[0] + vec.m_tuple[0],
        m_tuple[1] + vec.m_tuple[1],
        m_tuple[2] + vec.m_tuple[2]);
}

template <typename T>
NtVector3<T> NtVector3<T>::operator-(const NtVector3& vec) const
{
    return NtVector3(m_tuple[0] - vec.m_tuple[0],
        m_tuple[1] - vec.m_tuple[1],
        m_tuple[2] - vec.m_tuple[2]);
}

template <typename T>
NtVector3<T> NtVector3<T>::operator*(T value) const
{
    return NtVector3(m_tuple[0] * value, m_tuple[1] * value, m_tuple[2] * value);
}

template <typename T>
NtVector3<T> NtVector3<T>::operator/(T value) const
{
    NtVector3 vec;
    if (value != (T)0.0)
    {
        T invValue = ((T)1.0) / value;
        vec.m_tuple[0] = invValue * m_tuple[0];
        vec.m_tuple[1] = invValue * m_tuple[1];
        vec.m_tuple[2] = invValue * m_tuple[2];
    }
    else
    {
        vec.m_tuple[0] = NtMath<T>::MAX_VALUE;
        vec.m_tuple[1] = NtMath<T>::MAX_VALUE;
        vec.m_tuple[2] = NtMath<T>::MAX_VALUE;
    }

    return vec;
}

template <typename T>
NtVector3<T> NtVector3<T>::operator-() const
{
    return NtVector3( -m_tuple[0],
        -m_tuple[1],
        -m_tuple[2]);
}


template <typename T>
NtVector3<T>& NtVector3<T>::operator+=(const NtVector3& vec)
{
    m_tuple[0] += vec.m_tuple[0];
    m_tuple[1] += vec.m_tuple[1];
    m_tuple[2] += vec.m_tuple[2];

    return *this;
}

template <typename T>
NtVector3<T>& NtVector3<T>::operator-=(const NtVector3& vec)
{
    m_tuple[0] = vec.m_tuple[0];
    m_tuple[1] = vec.m_tuple[1];
    m_tuple[2] = vec.m_tuple[2];

    return *this;
}

template <typename T>
NtVector3<T>& NtVector3<T>::operator*=(T value)
{
    m_tuple[0] *= value;
    m_tuple[1] *= value;
    m_tuple[2] *= value;

    return *this;
}

template <typename T>
NtVector3<T>& NtVector3<T>::operator/=(T value)
{
    if (value != (T)0.0)
    {
        T invValue = ((T)1.0) / value;
        m_tuple[0] *= invValue;
        m_tuple[1] *= invValue;
        m_tuple[2] *= invValue;
    }
    else
    {
        m_tuple[0] = NtMath<T>::MAX_VALUE;
        m_tuple[1] = NtMath<T>::MAX_VALUE;
        m_tuple[2] = NtMath<T>::MAX_VALUE;
    }

    return *this;
}

template <typename T>
T NtVector3<T>::Length() const
{
    return NtMath<T>::Sqrt( m_tuple[0] * m_tuple[0] +
        m_tuple[1] * m_tuple[1] +
        m_tuple[2] * m_tuple[2]);
}

template <typename T>
T NtVector3<T>::SquareLength() const
{
    return m_tuple[0] * m_tuple[0] +
        m_tuple[1] * m_tuple[1] +
        m_tuple[2] * m_tuple[2];
}

template <typename T>
T NtVector3<T>::DotProduct(const NtVector3& vec) const
{
    return m_tuple[0] * vec.m_tuple[0] + 
        m_tuple[1] * vec.m_tuple[1] +
        m_tuple[2] * vec.m_tuple[2];
}

template <typename T>
T NtVector3<T>::Normalize()
{
    T length = this->Length();
    if (length > NtMath<T>::TOLERANCE)
    {
        T invLength = ((T)1.0) / length;
        m_tuple[0] *= invLength;
        m_tuple[1] *= invLength;
        m_tuple[2] *= invLength;
    }
    else
    {
        length = (T)0.0;
        m_tuple[0] = (T)0.0;
        m_tuple[1] = (T)0.0;
        m_tuple[2] = (T)0.0;
    }

    return length;
}

template <typename T>
NtVector3<T> NtVector3<T>::CrossProduct(const NtVector3& vec) const
{
    return NtVector3( m_tuple[1] * vec.m_tuple[2] - m_tuple[2] * vec.m_tuple[1],
        m_tuple[2] * m_tuple[0] - m_tuple[0] * m_tuple[2],
        m_tuple[0] * m_tuple[1] - m_tuple[1] * m_tuple[0]);
}

template <typename T>
NtVector3<T> NtVector3<T>::UnitCrossProduct(const NtVector3& vec) const
{
    NtVector3 cross(
        m_tuple[1] * vec.m_tuple[2] - m_tuple[2] * vec.m_tuple[1],
        m_tuple[2] * m_tuple[0] - m_tuple[0] * m_tuple[2],
        m_tuple[0] * m_tuple[1] - m_tuple[1] * m_tuple[0]);

    cross.Normalize();
    return cross;
}

template <typename T>
void NtVector3<T>::Orthonormalize(NtVector3& u, NtVector3& v, NtVector3& w)
{
    //   u0 = v0/|v0|
    //   u1 = (v1-(u0*v1)u0)/|v1-(u0*v1)u0|
    //   u2 = (v2-(u0*v2)u0-(u1*v2)u1)/|v2-(u0*v2)u0-(u1*v2)u1|

    // u0
    u.Normalize();

    // u1
    T dotU = u.DotProduct(v);
    v -= dotU * u;
    v.Normalize();

    // u2
    T dotV = v.DotProduct(w);
    dotU = u.DotProduct(w);
    w -= dotU * u + dotV * v;
    w.Normalize();
}

template <typename T>
void NtVector3<T>::Orthonormalize(NtVector3* vec)
{
    Orthonormalize(vec[0], vec[1], vec[2]);
}
