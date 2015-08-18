
#ifndef __NTVECTOR3_H
#define __NTVECTOR3_H

#include "NtMath.h"

namespace NT
{

    template <typename T>
    class NtVector3
    {
    public:
        NtVector3();
        NtVector3(const NtVector3& vec);
        NtVector3(T x, T y, T z);
		NtVector3(T arr[3]);

        // 각 좌표 접근
        operator const T* () const;
        operator T* ();
        T operator[] (int index) const;
        T& operator[] (int index);
        T X() const;
        T& X();
        T Y() const;
        T& Y();
        T Z() const;
        T& Z();

        // 할당
        NtVector3& operator=(const NtVector3& vec);

        // 비교
        bool operator==(const NtVector3& vec) const;
        bool operator!=(const NtVector3& vec) const;
        bool operator<(const NtVector3& vec) const;
        bool operator<=(const NtVector3& vec) const;
        bool operator>(const NtVector3& vec) const;
        bool operator>=(const NtVector3& vec) const;

        // 산술
        NtVector3 operator+(const NtVector3& vec) const;
        NtVector3 operator-(const NtVector3& vec) const;
        NtVector3 operator*(T value) const;
        NtVector3 operator/(T value) const;
        NtVector3 operator-() const;

        // 산술 할당
        NtVector3& operator+=(const NtVector3& vec);
        NtVector3& operator-=(const NtVector3& vec);
        NtVector3& operator*=(T value);
        NtVector3& operator/=(T value);

        T Length() const;
        T SquareLength() const;
        T DotProduct(const NtVector3& vec) const;
        T Normalize();

        NtVector3 CrossProduct(const NtVector3& vec) const;
        NtVector3 UnitCrossProduct(const NtVector3& vec) const;

        static void Orthonormalize(NtVector3& u, NtVector3& v, NtVector3& w);
        static void Orthonormalize(NtVector3* v);

        static const NtVector3 ZERO;
        static const NtVector3 UNIT_X;
        static const NtVector3 UNIT_Y;
        static const NtVector3 UNIT_Z;

    private:
        int CompareTuple(const NtVector3& vec) const;

    private:
        T m_tuple[3];
    };

template <typename T>
NtVector3<T> operator*(T scalar, const NtVector3<T>& vec);

#include "NtVector3.inl"

typedef NtVector3<float> NtVector3f;
typedef NtVector3<double> NtVector3d;
}

#endif