
#pragma once

#include <cmath>

namespace nt {
	
template <typename real>
class NtMath
{
public:
	// 일반 삼각 함수
	static real Sin(real value);

	static real Cos(real value);

	static real Tan(real value);

	// 역함수
	static real ASin(real value);

	static real ACos(real value);

	static real ATan(real value);

	static real ATan2(real yValue, real xValue);

	//
	static real Angle(float x, float y);

	// 정수화 함수
	static real Floor(real value);

	static real Ceil(real value);

	static real FMod(real xValue, real yValue);

	// 지수 함수
	static real Pow(real originValue, real exponent);

	static real Sqr(real value);

	static real Sqrt(real value);

	static real InvSqrt(real value);

	static real Log(real value);

	static real Exp(real value);

	// 
	static real Abs(real val);
	
	static bool IsZero(real val);

	static real Min(real a, real b);

	static real Max(real a, real b);

	static real Clamp(real current, real min, real max);

    static bool Approximately(real a, real b);

	// return random [0 ~ 1)
	static real Rand();

	static real Rand(real a, real b);

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

typedef NtMath<float> NtMathf;

}
