
#ifndef __NTMATRIX2_H
#define __NTMATRIX2_H

#include "NtVector2.h"

namespace nt
{
    template <typename T>
    class NtMatrix2
    {
    public:
        NtMatrix2(bool isZero = true);

        NtMatrix2(const NtMatrix2& mat);

        NtMatrix2(T i00, T i01, T i10, T i11);

        // rowMajor - true  : tuples[0~3] = {00, 01, 10, 11}
        //          - false : tuples[0~3] = {00, 10, 01, 11}
        NtMatrix2(const T tuples[4], bool rowMajor);

        NtMatrix2(const NtVector2<T>& U, const NtVector2<T> V, bool col);

        NtMatrix2(const NtVector2<T>* V, bool isColumn);

        NtMatrix2(T i00, T i11);

        NtMatrix2(T angle);

        // create tensor product
        NtMatrix2(const NtVector2<T>& U, const NtVector2<T>& V);

        static const NtMatrix2 ZERO;
        static const NtMatrix2 IDENTITY;
    private:
        void MakeZeroMatrix();

        void MakeIdentity();

        void MakeDiagonal(T i00, T i11);
    private:
        T m_tuple[4];
    };

#include "NtMatrix2.inl"

    typedef NtMatrix2<float> NtMatrix2f;
    typedef NtMatrix2<double> NtMatrix2d;
}

#endif