
#ifndef __NTVECTOR2_H
#define __NTVECTOR2_H

#include "NtMath.h"

namespace nt {

template <typename T>
class NtVector2
{
public:
    // 생성자
    NtVector2();
    NtVector2(T x, T y);
    NtVector2(const NtVector2& vec);

    // 접근지정자
    operator const T* () const;
    operator T* ();
    T operator[] (int index) const;
    T& operator[] (int index);

    T X() const;
    T& X();
    T Y() const;
    T& Y();

    // 할당
    NtVector2& operator= (const NtVector2& vec);

    // 비교
    bool operator== (const NtVector2& vec) const;
    bool operator!= (const NtVector2& vec) const;
    bool operator<  (const NtVector2& vec) const;
    bool operator<= (const NtVector2& vec) const;
    bool operator>  (const NtVector2& vec) const;
    bool operator>= (const NtVector2& vec) const;

    // 산술
    NtVector2 operator+ (const NtVector2& vec) const;
    NtVector2 operator- (const NtVector2& vec) const;
    NtVector2 operator* (T scalar) const;
    NtVector2 operator/ (T scalar) const;
    NtVector2 operator- () const;

    NtVector2& operator+= (const NtVector2& vec);
    NtVector2& operator-= (const NtVector2& vec);
    NtVector2& operator*= (T scalar);
    NtVector2& operator/= (T scalar);

    // 벡터 연산
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

    // 삼각형(v0, v1, v2) 안의 좌표에 대한 무게중심을 반환한다.
    // P = b0*v0 + b1*v1 + b2*v2, 단 b0 + b1 + b2 = 1.
    //void GetBarycentrics(const NtVector2<T>& v0, const NtVector2<T>& v1, const NtVector2<T>& v2, T bary[3]) const;

    // Gram-Schmidt orthonormalization.
    static void Orthonormalize(NtVector2& u, NtVector2& v);

    // 입력되는 0이 아닌 벡터v가 입력되면, 출력은 Ortho 기반의(u, v) 가 된다.
    static void GenerateOrthonormalBase(NtVector2& u, NtVector2& v, bool unitLengthV);

    // 특수 벡터
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
