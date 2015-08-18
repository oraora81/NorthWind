
#pragma once

#include <cmath>

namespace NT
{
    
template <typename real>
class NtMath
{
public:
    // �Ϲ� �ﰢ �Լ�
    static real Sin(real value);
    static real Cos(real value);
    static real Tan(real value);

    // ���Լ�
    static real ASin(real value);
    static real ACos(real value);
    static real ATan(real value);
    static real ATan2(real yValue, real xValue);

    // ����ȭ �Լ�
    static real Floor(real value);
    static real Ceil(real value);
    static real FMod(real xValue, real yValue);

    // ���� �Լ�
    static real Pow(real originValue, real exponent);
    static real Sqr(real value);
    static real Sqrt(real value);
    static real InvSqrt(real value);
    static real Log(real value);
    static real Exp(real value);

	static real Abs(real val);
	static bool IsZero(real val);

    static const real EPSILON;
    static const real TOLERANCE;
    static const real MAX_VALUE;
    static const real PI;
    static const real HALF_PI;
    static const real TWO_PI;
    static const real INV_PI;
    static const real DEG_TO_RAD;
    static const real RAD_TO_DEG;
};

#include "NtMath.inl"

}
