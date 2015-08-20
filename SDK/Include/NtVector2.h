
#ifndef __NTVECTOR2_H
#define __NTVECTOR2_H

#include "NtMath.h"

namespace nt
{

template <typename T>
class NtVector2
{
public:
    // ������
    NtVector2();
    NtVector2(T x, T y);
    NtVector2(const NtVector2& vec);

    // ����������
    operator const T* () const;
    operator T* ();
    T operator[] (int index) const;
    T& operator[] (int index);

    T X() const;
    T& X();
    T Y() const;
    T& Y();

    // �Ҵ�
    NtVector2& operator= (const NtVector2& vec);

    // ��
    bool operator== (const NtVector2& vec) const;
    bool operator!= (const NtVector2& vec) const;
    bool operator<  (const NtVector2& vec) const;
    bool operator<= (const NtVector2& vec) const;
    bool operator>  (const NtVector2& vec) const;
    bool operator>= (const NtVector2& vec) const;

    // ���
    NtVector2 operator+ (const NtVector2& vec) const;
    NtVector2 operator- (const NtVector2& vec) const;
    NtVector2 operator* (T scalar) const;
    NtVector2 operator/ (T scalar) const;
    NtVector2 operator- () const;

    NtVector2& operator+= (const NtVector2& vec);
    NtVector2& operator-= (const NtVector2& vec);
    NtVector2& operator*= (T scalar);
    NtVector2& operator/= (T scalar);

    // ���� ����
    T Length() const;
    T SquareLength() const;
    T DotProduct(const NtVector2& vec) const;
    T Normalize();

    // returns (y, -x)
    NtVector2 Perp() const;

    // returns (y, -x)/sqrt(x*x+y*y)
    NtVector2 UnitPerp() const;

    // returns DotProductPerp((x, y), (vec.x, vec.y)) = x * vec.y - y * vec.x
    T DotProductPerp(const NtVector2& vec) const;

    // �ﰢ��(v0, v1, v2) ���� ��ǥ�� ���� �����߽��� ��ȯ�Ѵ�.
    // P = b0*v0 + b1*v1 + b2*v2, �� b0 + b1 + b2 = 1.
    //void GetBarycentrics(const NtVector2<T>& v0, const NtVector2<T>& v1, const NtVector2<T>& v2, T bary[3]) const;

    // Gram-Schmidt orthonormalization.
    static void Orthonormalize(NtVector2& u, NtVector2& v);

    // �ԷµǴ� 0�� �ƴ� ����v�� �ԷµǸ�, ����� Ortho �����(u, v) �� �ȴ�.
    static void GenerateOrthonormalBase(NtVector2& u, NtVector2& v, bool unitLengthV);

    // Ư�� ����
    static const NtVector2 ZERO;
    static const NtVector2 UNIT_X;
    static const NtVector2 UNIT_Y;

private:
    int CompareArray(const NtVector2& vec) const;

private:
    T m_tuple[2];
};

template <typename T>
NtVector2<T> operator* (T scalar, const NtVector2<T>& vec);

#include "NtVector2.inl"

//typedef NtVector2<float> NtVector2f;
//typedef NtVector2<double> NtVector2d;

}

#endif